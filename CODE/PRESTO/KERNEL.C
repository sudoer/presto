
#include <stdlib.h>
#include <hc11.h>        // register definitions
#include "hc11regs.h"
#include "system.h"
#include "presto.h"
#include "presto\kernel.h"

////////////////////////////////////////////////////////////////////////////////

#define CYCLES_PER_MS     2000
#define MS_PER_TICK       5
#define CYCLES_PER_TICK   CYCLES_PER_MS*MS_PER_TICK
#define IDLE_PRIORITY     0
#define IDLE_STACK_SIZE   150

////////////////////////////////////////////////////////////////////////////////

// GLOBAL VARIABLES
// These are used to pass arguments to asm routines.

BYTE * presto_asm_new_sp=NULL;
BYTE ** presto_asm_old_sp_p=NULL;
void (*presto_asm_new_fn)(void)=NULL;
BYTE presto_asm_swap;

////////////////////////////////////////////////////////////////////////////////

// STATIC GLOBAL VARIABLES

static PRESTO_TCB_T * current_tcb_p=NULL;
static PRESTO_TCB_T * tcb_head_p=NULL;
static PRESTO_TCB_T * free_tcb_p=NULL;
static PRESTO_TCB_T tcb_list[MAX_TASKS];

static PRESTO_TIME_T presto_master_clock;
static BYTE presto_initialized=0;

// idle task stuff
static BYTE idle_stack[IDLE_STACK_SIZE];
static PRESTO_TCB_T * idle_tcb_p;
static BYTE idle_tid;

// mail stuff
static PRESTO_MESSAGE_T * free_mail_p=NULL;
static PRESTO_MESSAGE_T * po_mail_p=NULL;
static PRESTO_MESSAGE_T mail_list[MAX_MESSAGES];

////////////////////////////////////////////////////////////////////////////////

// FUNCTION PROTOTYPES

static PRESTO_TCB_T * presto_next_tcb_to_run(void);
static void presto_start_master_timer(void);
static void presto_restart_master_timer(void);
static void idle_task(void);
static BYTE deliver_mail(void);
static PRESTO_TCB_T * tid_to_tcbptr(BYTE tid);
static void print_tcb_list(void);
static void print_mail_list(void);
static void idle_task(void);

////////////////////////////////////////////////////////////////////////////////
//   I N T E R F A C E
////////////////////////////////////////////////////////////////////////////////

void presto_init(void) {
   BYTE count;

   // initialize once and only once
   if(presto_initialized) return;
   presto_initialized++;

   BITSET(DDRD,4);    // LED is output
   BITSET(DDRD,5);    // LED is output
   BITSET(PORTD,5);   // light off
   BITSET(PORTD,4);   // light off

   // initialize master clock
   presto_master_clock=clock_reset();

   // initialize TCB list
   for(count=0;count<MAX_TASKS;count++) {
      tcb_list[count].next=&tcb_list[count+1];
      tcb_list[count].task_id=count;
      tcb_list[count].state=STATE_INACTIVE;
   }
   tcb_list[MAX_TASKS-1].next=NULL;
   free_tcb_p=&tcb_list[0];

   // initialize mail list
   for(count=0;count<MAX_MESSAGES;count++) {
      mail_list[count].next=&mail_list[count+1];
      mail_list[count].serial_number=count;
   }
   mail_list[MAX_MESSAGES-1].next=NULL;
   free_mail_p=&mail_list[0];

   // initialize idle task
   // must be done after presto_initialized++ to break out of recursion
   idle_tid=presto_create_task(idle_task,idle_stack,IDLE_STACK_SIZE,IDLE_PRIORITY);
   idle_tcb_p=tid_to_tcbptr(idle_tid);
}

////////////////////////////////////////////////////////////////////////////////

PRESTO_TID_T presto_create_task( void (*func)(void), BYTE * stack, short stack_size, BYTE priority ) {

   PRESTO_TCB_T * new_tcb_p;

   // lazy initialization
   if(presto_initialized==0) presto_init();

   if(free_tcb_p==NULL) {
      // There are no more TCB's left.
      return -1;
   }

   // allocate TCB for new task
   new_tcb_p=free_tcb_p;
   free_tcb_p=free_tcb_p->next;

   // we're about to mess with tasks, TCB's... interrupts off
   INTR_OFF();

   // initialize TCB elements
   // new_tcb_p->task_id is already assigned
   new_tcb_p->stack_top=stack+stack_size-1;
   new_tcb_p->stack_bottom=stack;
   new_tcb_p->stack_ptr=new_tcb_p->stack_top;
   new_tcb_p->priority=priority;
   new_tcb_p->state=STATE_READY;
   new_tcb_p->mailbox_head=NULL;
   new_tcb_p->mailbox_tail=NULL;

   // call asm routine to set up new stack
   presto_asm_new_sp=new_tcb_p->stack_ptr;
   presto_asm_new_fn=func;
   presto_setup_new_task();
   new_tcb_p->stack_ptr=presto_asm_new_sp;

   // insert new TCB into list in priority order
   if(tcb_head_p==NULL) {
      // we are the first TCB in the list
      tcb_head_p=new_tcb_p;
      new_tcb_p->next=NULL;
   } else if((new_tcb_p->priority)>(tcb_head_p->priority)) {
      // advance to the head of the class!
      new_tcb_p->next=tcb_head_p;
      tcb_head_p=new_tcb_p;
   } else {
      PRESTO_TCB_T * ptr=tcb_head_p;
      while(ptr->next!=NULL) {
         if((new_tcb_p->priority)>(ptr->next->priority)) break;
         ptr=ptr->next;
      }

      // ptr->next is either NULL or lower priority than us
      // either way, we want to get inserted between ptr and ptr->next
      new_tcb_p->next=ptr->next;
      ptr->next=new_tcb_p;
   }

   // we're done messing with the task list... interrupts back on
   INTR_ON();

   return new_tcb_p->task_id;
}

////////////////////////////////////////////////////////////////////////////////

void presto_start_scheduler(void) {

   // lazy initialization
   if(presto_initialized==0) presto_init();

   // we're about to switch to our first task... interrupts off
   INTR_OFF();

   set_interrupt(INTR_TOC2, presto_system_isr);

   // start timer interrupts for pre-emption
   presto_start_master_timer();

   // pick next task to run
   // first task in list is highest priority and is ready
   current_tcb_p=tcb_head_p;

   // call asm routine to set up new stack and run from it
   presto_asm_new_sp=current_tcb_p->stack_ptr;
   presto_start_task_switching();

   // we never get here
   presto_fatal_error();
}

////////////////////////////////////////////////////////////////////////////////

void presto_sleep(void) {

   // we're about to switch to a new task... interrupts off
   INTR_OFF();

   // check to see if we've clobbered our stack
   if(((current_tcb_p->stack_ptr)>(current_tcb_p->stack_top))
   ||((current_tcb_p->stack_ptr)<(current_tcb_p->stack_bottom)))
      presto_fatal_error();

   // we will only sleep if there are no messages in our queue
   if(current_tcb_p->mailbox_head==NULL) {
      // no mail, so we can sleep
      current_tcb_p->state=STATE_BLOCKED;

      // the asm routine will save old SP in old TCB
      presto_asm_old_sp_p=&(current_tcb_p->stack_ptr);

      // pick next task to run
      current_tcb_p=presto_next_tcb_to_run();

      // call asm routine to set up new stack
      // when we return, we'll be another process
      // the asm routine will re-enable interrupts
      presto_asm_new_sp=current_tcb_p->stack_ptr;
      presto_switch_tasks();
   } else {
      // you have mail... we can't sleep when we have mail
      // hmmm, can't think of anything to do here... yet
   }
   INTR_ON();
}

////////////////////////////////////////////////////////////////////////////////

void presto_kill_self(void) {
   // TODO - remove TCB from list
   presto_fatal_error();
}

////////////////////////////////////////////////////////////////////////////////
//   I N T E R R U P T
////////////////////////////////////////////////////////////////////////////////

void presto_service_timer_interrupt(void) {
   // we're changing tasks, dealing with task list and mail list
   // don't want to get interrupted
   // INTR_OFF();  already off

   // take care of clock things
   presto_master_clock=clock_add(presto_master_clock,MS_PER_TICK);
   presto_restart_master_timer();

   // check mail
   if(deliver_mail()>0) {

      // check to see if we've clobbered our stack
      if(((current_tcb_p->stack_ptr)>(current_tcb_p->stack_top))
      ||((current_tcb_p->stack_ptr)<(current_tcb_p->stack_bottom)))
         presto_fatal_error();

      // the ISR will save old SP in old TCB
      presto_asm_old_sp_p=&(current_tcb_p->stack_ptr);
      current_tcb_p->state=STATE_READY;

      // pick next task to run
      current_tcb_p=presto_next_tcb_to_run();

      // end of ISR will set up new stack
      presto_asm_new_sp=current_tcb_p->stack_ptr;

      // signal to ISR for it to do the task swapping
      presto_asm_swap=1;
   } else {
      // signal to ISR for it NOT to do the task swapping
      presto_asm_swap=0;
   }
}

////////////////////////////////////////////////////////////////////////////////
//   S C H E D U L I N G
////////////////////////////////////////////////////////////////////////////////

static PRESTO_TCB_T * presto_next_tcb_to_run(void) {
   // pick highest priority ready task to run
   PRESTO_TCB_T * ptr=tcb_head_p;
   while(ptr!=NULL) {
      if(ptr->state==STATE_READY) return ptr;
      ptr=ptr->next;
   }
   // should never get here
   presto_fatal_error();
   return NULL;
}

////////////////////////////////////////////////////////////////////////////////
//   M E S S A G E S   A N D   T I M E R S
////////////////////////////////////////////////////////////////////////////////

BYTE presto_send_message(PRESTO_TID_T to, PRESTO_MAIL_T payload) {
   return presto_timer(to,0,payload);
}

////////////////////////////////////////////////////////////////////////////////

BYTE presto_timer(PRESTO_TID_T to, unsigned short delay, PRESTO_MAIL_T payload) {
   PRESTO_MESSAGE_T * new_mail_p;

   // we're going to mess with the PO mail list... interrupts off
   INTR_OFF();

   // check to see if there's room
   if(free_mail_p==NULL) {
      INTR_ON();
      return 1;
   }

   // check to see that the recipient is an alive task
   if(tid_to_tcbptr(to)==NULL) {
      INTR_ON();
      return 2;
   }

   // allocate space for a new message
   new_mail_p=free_mail_p;
   free_mail_p=free_mail_p->next;

   // fill in the blanks
   // new_mail_p->serial_number;   already set
   new_mail_p->from_tid=current_tcb_p->task_id;
   new_mail_p->to_tcb_p=tid_to_tcbptr(to);
   new_mail_p->delivery_time=clock_add(presto_master_clock,delay);
   new_mail_p->payload=payload;

   // insert new message into list in time order
   if(po_mail_p==NULL) {

      // we are the first message in PO
      po_mail_p=new_mail_p;
      new_mail_p->next=NULL;

   } else if(clock_compare(po_mail_p->delivery_time,new_mail_p->delivery_time)>0) {

      // advance to the head of the class!
      new_mail_p->next=po_mail_p;
      po_mail_p=new_mail_p;

   } else {

      // we are one of many messages in the PO
      PRESTO_MESSAGE_T * msg_p=po_mail_p;
      while(msg_p->next!=NULL) {
         if(clock_compare(msg_p->next->delivery_time,new_mail_p->delivery_time)>0) break;
         msg_p=msg_p->next;
      }

      // msg_p->next is either NULL or later delivery time than us
      // either way, we want to get inserted between msg_p and msg_p->next
      new_mail_p->next=msg_p->next;
      msg_p->next=new_mail_p;
   }

   // we're done messing with the PO mail list... interrupts back on
   INTR_ON();
   return 0;
}

////////////////////////////////////////////////////////////////////////////////

static BYTE deliver_mail(void) {
   BYTE count=0;
   PRESTO_MESSAGE_T * msg_p;
   PRESTO_TCB_T * tcb_p;
   while((po_mail_p!=NULL)&&(clock_compare(po_mail_p->delivery_time,presto_master_clock)<=0)) {
      // we're going to use this a lot, so de-reference once
      tcb_p=po_mail_p->to_tcb_p;
      if(tcb_p==NULL) presto_fatal_error();

      // make receiver task ready
      tcb_p->state=STATE_READY;

      // remove message from PO list
      msg_p=po_mail_p;
      po_mail_p=po_mail_p->next;

      // move the message to the task's mail list
      if(tcb_p->mailbox_head==NULL) {

         // we are the only message in the list
         tcb_p->mailbox_head=msg_p;
         tcb_p->mailbox_tail=msg_p;

      } else {

         // we are one of many, add to the tail of the list
         tcb_p->mailbox_tail->next=msg_p;
         tcb_p->mailbox_tail=msg_p;

      }
      // no matter what, we are the last in the task's message list
      msg_p->next=NULL;

      // indicate that we moved one mail message
      count++;
   }
   return count;
}

////////////////////////////////////////////////////////////////////////////////

BYTE presto_mail_waiting(void) {
   return (current_tcb_p->mailbox_head!=NULL);
}

////////////////////////////////////////////////////////////////////////////////

BYTE presto_get_message(PRESTO_MAIL_T * payload_p) {
   PRESTO_MESSAGE_T * msg_p;
   // we're about to mess with the mail list... interrupts off
   INTR_OFF();
   // we're going to use this a lot, so dereference now
   msg_p=current_tcb_p->mailbox_head;
   // get one message from the task's mail queue
   if(msg_p==NULL) {
      // there are no messages in the task's mail list
      INTR_ON();
      return 0;
   } else if (msg_p==current_tcb_p->mailbox_tail) {
      // there is only one item in the list, take it
      current_tcb_p->mailbox_head=NULL;
      current_tcb_p->mailbox_tail=NULL;
   } else {
      // there are many messages, take first
      current_tcb_p->mailbox_head=msg_p->next;
   }
   // record the message id before we can get interrupted
   if(payload_p!=NULL) *payload_p=msg_p->payload;
   // return the message to the free list
   msg_p->next=free_mail_p;
   free_mail_p=msg_p;
   // done messing with mail lists... interrupts back on
   INTR_ON();
   // return the number of messages retrieved
   return 1;
}

////////////////////////////////////////////////////////////////////////////////
//   I D L E   T A S K
////////////////////////////////////////////////////////////////////////////////

static void idle_task(void) {
   while(1) {
      // do nothing
   }
}

////////////////////////////////////////////////////////////////////////////////
//   U T I L I T I E S
////////////////////////////////////////////////////////////////////////////////

static PRESTO_TCB_T * tid_to_tcbptr(BYTE tid) {
   if(tid>=MAX_TASKS) presto_fatal_error();
   if(tcb_list[tid].state==STATE_INACTIVE) return NULL;
   return &tcb_list[tid];
}

////////////////////////////////////////////////////////////////////////////////
//   H A R D W A R E   T I M E R / C O U N T E R
////////////////////////////////////////////////////////////////////////////////

static void presto_start_master_timer(void) {

   // store (current plus CYCLES_PER_TICK)
   TOC2 = TCNT + CYCLES_PER_TICK;

   // request output compare interrupt
   TMSK1 |= TMSK1_OC2I;

   // clear the OUTPUT COMPARE flag
   // writing O's makes no change, writing 1's clears the bit
   TFLG1 = TFLG1_OC2F;

   // counter disconnected from output pin logic
   TCTL1 &= ~(TCTL1_OM2|TCTL1_OL2);
}

////////////////////////////////////////////////////////////////////////////////

static void presto_restart_master_timer(void) {
   // store (last plus CYCLES_PER_TICK)
   TOC2 = TOC2 + CYCLES_PER_TICK;

   // clear the OUTPUT COMPARE flag
   // writing O's makes no change, writing 1's clears the bit
   TFLG1 = TFLG1_OC2F;
}

////////////////////////////////////////////////////////////////////////////////
//   S A F E T Y   C H E C K
////////////////////////////////////////////////////////////////////////////////

void presto_fatal_error(void) {
   // should never get here
   WORD delay=0;
   INTR_OFF();

   motor_speed(0,-1);
   motor_speed(1,-1);
   motor_speed(2,-1);
   motor_speed(3,-1);

   BITSET(DDRD,4);              // LED is an output
   while(1) {
      BITNOT(PORTA,3);          // toggle speaker
      BITCLR(PORTD,4);          // LED on
      while(delay!=0) delay++;
   }
}

////////////////////////////////////////////////////////////////////////////////

