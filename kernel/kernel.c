
#include "hc11regs.h"
#include "system.h"
#include "presto.h"
#include "intvect.h"
#include "debug.h"
#include "kernel\kernel.h"

////////////////////////////////////////////////////////////////////////////////

#define CYCLES_PER_MS       2000        // based on hardware
#define CYCLES_PER_CLOCK    2           // based on hardware
#define CLOCK_PRESCALE      16          // set in TMSK2 register
#define MS_PER_TICK         100         // how often do you want?
#define CLOCKS_PER_MS       CYCLES_PER_MS*CYCLES_PER_CLOCK/CLOCK_PRESCALE
#define CLOCKS_PER_TICK     CLOCKS_PER_MS*MS_PER_TICK
#define IDLE_PRIORITY       0
#define IDLE_STACK_SIZE     50

////////////////////////////////////////////////////////////////////////////////

#define DISABLE_CCR_INTERRUPT_BIT     asm("oraa #0x10");
#define ENABLE_CCR_INTERRUPT_BIT      asm("anda #~0x10");

////////////////////////////////////////////////////////////////////////////////

// GLOBAL VARIABLES

BYTE flag_mirror;

////////////////////////////////////////////////////////////////////////////////

// STATIC GLOBAL VARIABLES

/*static*/ PRESTO_TCB_T * current_tcb_p=NULL;
/*static*/ PRESTO_TID_T current_tid=0;
/*static*/ PRESTO_TCB_T * tcb_head_p=NULL;
/*static*/ PRESTO_TCB_T * free_tcb_p=NULL;
/*static*/ PRESTO_TCB_T tcb_list[MAX_TASKS];

/*static*/ PRESTO_TIME_T presto_master_clock;

// idle task stuff
/*static*/ BYTE idle_stack[IDLE_STACK_SIZE];
/*static*/ PRESTO_TCB_T * idle_tcb_p;
/*static*/ BYTE idle_tid;

// mail stuff
/*static*/ PRESTO_MESSAGE_T * free_mail_p=NULL;
/*static*/ PRESTO_MESSAGE_T * po_mail_p=NULL;
/*static*/ PRESTO_MESSAGE_T mail_list[MAX_MESSAGES];

// These are used to pass arguments to inline assembly routines
/*static*/ BYTE * global_new_sp=NULL;
/*static*/ BYTE ** global_old_sp_p=NULL;
/*static*/ void (*global_new_fn)(void)=NULL;
/*static*/ BYTE * global_save_sp;     // do not put this on the stack (BOOM)

////////////////////////////////////////////////////////////////////////////////

// FUNCTION PROTOTYPES

/*static*/ PRESTO_TCB_T * presto_next_tcb_to_run(void);
/*static*/ void presto_start_master_timer(void);
/*static*/ void presto_restart_master_timer(void);
/*static*/ void idle_task(void);
/*static*/ BYTE deliver_mail(void);
/*static*/ PRESTO_TCB_T * tid_to_tcbptr(BYTE tid);
/*static*/ void print_tcb_list(void);
/*static*/ void print_mail_list(void);
/*static*/ void idle_task(void);

void presto_system_isr(void) __attribute__((interrupt));
void context_switch(void) __attribute__((interrupt));

////////////////////////////////////////////////////////////////////////////////
//   I N I T I A L I Z A T I O N
////////////////////////////////////////////////////////////////////////////////

void presto_init(void) {
   BYTE count;
   BYTE flags;

   INTR_SAVE(flags);
   INTR_RESTORE(flags);

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
      mail_list[count].next=&mail_list[count+1];  // goes past end of array - OK
      mail_list[count].serial_number=count;
   }
   mail_list[MAX_MESSAGES-1].next=NULL;
   free_mail_p=&mail_list[0];

   // initialize idle task
   idle_tid=presto_create_task(idle_task,idle_stack,IDLE_STACK_SIZE,IDLE_PRIORITY);
   idle_tcb_p=tid_to_tcbptr(idle_tid);
}

////////////////////////////////////////////////////////////////////////////////
//   T A S K   M A N A G E M E N T
////////////////////////////////////////////////////////////////////////////////

PRESTO_TID_T presto_create_task( void (*func)(void), BYTE * stack, short stack_size, BYTE priority ) {

   PRESTO_TCB_T * new_tcb_p;
   BYTE * sp;
   MISCWORD xlate;    // to split a word into two bytes
   BYTE flags;

   if(free_tcb_p==NULL) {
      // There are no more TCB's left.
      presto_fatal_error(0x06);
      return -1;
   }

   // we're about to mess with tasks, TCB's... interrupts off
   INTR_SAVE(flags);

   // allocate TCB for new task
   new_tcb_p=free_tcb_p;
   free_tcb_p=free_tcb_p->next;

   // initialize TCB elements
   // new_tcb_p->task_id is already assigned
   new_tcb_p->stack_top=stack+stack_size-1;
   new_tcb_p->stack_bottom=stack;
   new_tcb_p->priority=priority;
   new_tcb_p->state=STATE_READY;
   new_tcb_p->mailbox_head=NULL;
   new_tcb_p->mailbox_tail=NULL;

   // SET UP NEW STACK

   sp=new_tcb_p->stack_top;
   xlate.w=(WORD)func;
   *sp--=xlate.b.l;    // function pointer(L)
   *sp--=xlate.b.h;    // function pointer(H)
   *sp--=0x66;         // Y(L) register
   *sp--=0x55;         // Y(H) register
   *sp--=0x44;         // X(L) register
   *sp--=0x33;         // X(H) register
   *sp--=0x11;         // A register
   *sp--=0x22;         // B register
   *sp--=0x00;         // condition codes (I bit cleared)
   *sp--=0xFF;         // _.xy  0004(L)
   *sp--=0xEE;         // _.xy  0004(H)
   *sp--=0xDD;         // _.z   0002(L)
   *sp--=0xCC;         // _.z   0002(H)
   *sp--=0xBB;         // _.tmp 0000(L)
   *sp--=0xAA;         // _.tmp 0000(H)

   new_tcb_p->stack_ptr=sp;

   // INSERT NEW TCB INTO LIST IN PRIORITY ORDER

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
   INTR_RESTORE(flags);

   return new_tcb_p->task_id;
}

////////////////////////////////////////////////////////////////////////////////
//   S T A R T   M U L T I - T A S K I N G
////////////////////////////////////////////////////////////////////////////////

void presto_start_scheduler(void) {

   // we're about to switch to our first task... interrupts off
   INTR_OFF();

   //set_interrupt(INTR_TOC2, presto_system_isr_wrapper);
   set_interrupt(INTR_TOC2, presto_system_isr);
   set_interrupt(INTR_SWI, context_switch);

   // start timer interrupts for pre-emption
   presto_start_master_timer();

   // pick next task to run
   // first task in list is highest priority and is ready
   current_tcb_p=tcb_head_p;
   if(current_tcb_p==NULL) {
      presto_fatal_error(0x03);
   }
   current_tid=current_tcb_p->task_id;

   // SET UP A NEW STACK AND START EXECUTION USING IT

   // these parameters will be used in inline assembly...
   // must be put in global space, not on stack
   global_new_sp=current_tcb_p->stack_ptr;

   asm("lds global_new_sp");

   asm("pulx");  // _.xy
   asm("pulx");  // _.z
   asm("pulx");  // _.tmp
   asm("rti");

   // we never get here
   presto_fatal_error(0x04);
}

////////////////////////////////////////////////////////////////////////////////
//   C O N T E X T   S W I T C H I N G   ( I N T E R R U P T )
////////////////////////////////////////////////////////////////////////////////

void presto_system_isr(void) {

   // registers are pushed when timer interrupt is executed

   // interrupts are disabled at this time

   // take care of clock things
   presto_master_clock=clock_add(presto_master_clock,MS_PER_TICK);
   presto_restart_master_timer();

   // check mail
   if(deliver_mail()>0) {

      // check to see if we've clobbered our stack
      if(((current_tcb_p->stack_ptr)>(current_tcb_p->stack_top))
      ||((current_tcb_p->stack_ptr)<(current_tcb_p->stack_bottom)))
         presto_fatal_error(0x08);

      // these parameters will be used in inline assembly...
      // must be put in global space, not on stack

      // the ISR will save old SP in old TCB
      global_old_sp_p=&(current_tcb_p->stack_ptr);

      // pick next task to run
      current_tcb_p=presto_next_tcb_to_run();
      current_tid=current_tcb_p->task_id;

      // end of ISR will set up new stack
      global_new_sp=current_tcb_p->stack_ptr;

      // store the old stack pointer
      asm("ldy global_old_sp_p");
      asm("sts 0,y");
      // load the new stack pointer
      asm("lds global_new_sp");
   }

/*
   // Clear interrupt mask bit (to enable ints) in the CC register on the stack.
   // That way, the new task will have interrupts enabled when it wakes up.
   asm("pula");
   ENABLE_CCR_INTERRUPT_BIT;
   asm("psha");
*/
}

////////////////////////////////////////////////////////////////////////////////

void context_switch(void) {

   // registers are pushed when SWI is executed

/*
   // check to see if the old task has clobbered its stack
   if(((current_tcb_p->stack_ptr)>(current_tcb_p->stack_top))
   ||((current_tcb_p->stack_ptr)<(current_tcb_p->stack_bottom)))
      presto_fatal_error(0x09);
*/

   // the inline asm will save old SP in old TCB
   global_old_sp_p=&(current_tcb_p->stack_ptr);

   // pick next task to run
   current_tcb_p=presto_next_tcb_to_run();
   current_tid=current_tcb_p->task_id;

/*
   // check to see if the new task has clobbered its stack
   if(((current_tcb_p->stack_ptr)>(current_tcb_p->stack_top))
   ||((current_tcb_p->stack_ptr)<(current_tcb_p->stack_bottom)))
      presto_fatal_error(0x0A);
*/

   // call asm routine to set up new stack
   // when we return, we'll be another process
   // the asm routine will re-enable interrupts
   global_new_sp=current_tcb_p->stack_ptr;

   // store the old stack pointer
   asm("ldy global_old_sp_p");
   asm("sts 0,y");
   // load the new stack pointer
   asm("lds global_new_sp");

}

////////////////////////////////////////////////////////////////////////////////
//   S C H E D U L I N G
////////////////////////////////////////////////////////////////////////////////

/*static*/ PRESTO_TCB_T * presto_next_tcb_to_run(void) {
   // pick highest priority ready task to run
   PRESTO_TCB_T * ptr=tcb_head_p;
   while(ptr!=NULL) {
      if(ptr->state==STATE_READY) return ptr;
      ptr=ptr->next;
   }
   // should never get here
   presto_fatal_error(0x0B);
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
   PRESTO_TCB_T * tcb_ptr;
   BYTE flags;

   // we're going to mess with the PO mail list... interrupts off
   INTR_SAVE(flags);

   // check to see if there's room
   if(free_mail_p==NULL) {
      presto_fatal_error(0x0C);
   }

   // check to see that the recipient is an alive task
   tcb_ptr=tid_to_tcbptr(to);
   if(tcb_ptr==NULL) {
      presto_fatal_error(0x0D);
   }

   // allocate space for a new message
   new_mail_p=free_mail_p;
   free_mail_p=free_mail_p->next;

   // fill in the blanks
   // new_mail_p->serial_number;   already set
   new_mail_p->from_tid=current_tcb_p->task_id;
   new_mail_p->to_tcb_p=tcb_ptr;
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
   INTR_RESTORE(flags);

   return 0;
}

////////////////////////////////////////////////////////////////////////////////

BYTE presto_wait_for_message(PRESTO_MAIL_T * payload_p) {
   PRESTO_MESSAGE_T * msg_p;
   BYTE flags;

   // we're about to switch to a new task... interrupts off
   INTR_SAVE(flags);

   // we will only sleep if there are no messages in our queue
   if(current_tcb_p->mailbox_head==NULL) {
      // no mail, so we can sleep
      current_tcb_p->state=STATE_BLOCKED;
      asm("swi");
      // When we wake up, we'll be ready to recieve our mail.
      // Interrupts will be enabled.
   }

   // we're about to mess with the mail list... interrupts off
   INTR_SAVE(flags);
   // we're going to use this a lot, so dereference now
   msg_p=current_tcb_p->mailbox_head;

   // AIGH! - this is where the error happens

   // get one message from the task's mail queue
   if(msg_p==NULL) {
      // there are no messages in the task's mail list
      presto_fatal_error(0x0E);
   }

   // are we being paranoid?
   if((msg_p->to_tcb_p)!=current_tcb_p) {
      presto_fatal_error(0x0F);
   }

   // there is at least one message, get one
   if (msg_p==current_tcb_p->mailbox_tail) {
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
   INTR_RESTORE(flags);

   // return the number of messages retrieved
   return 1;
}

////////////////////////////////////////////////////////////////////////////////

/*static*/ BYTE deliver_mail(void) {
   BYTE count=0;
   PRESTO_MESSAGE_T * msg_p;
   PRESTO_TCB_T * tcb_p;
   while((po_mail_p!=NULL)&&(clock_compare(po_mail_p->delivery_time,presto_master_clock)<=0)) {
      // we're going to use this a lot, so de-reference once
      tcb_p=po_mail_p->to_tcb_p;
      if(tcb_p==NULL) presto_fatal_error(0x10);

      // make receiver task ready
      tcb_p->state=STATE_READY;

      // remove message from PO list
      msg_p=po_mail_p;                      // we know that po_mail_p!=NULL
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
//   I D L E   T A S K
////////////////////////////////////////////////////////////////////////////////

/*static*/ void idle_task(void) {
   while(1) {
      // do nothing
   }
}

////////////////////////////////////////////////////////////////////////////////
//   U T I L I T I E S
////////////////////////////////////////////////////////////////////////////////

/*static*/ PRESTO_TCB_T * tid_to_tcbptr(BYTE tid) {
   if(tid>=MAX_TASKS) presto_fatal_error(0x11);
   if(tcb_list[tid].state==STATE_INACTIVE) return NULL;
   return &tcb_list[tid];
}

////////////////////////////////////////////////////////////////////////////////
//   H A R D W A R E   T I M E R / C O U N T E R
////////////////////////////////////////////////////////////////////////////////

/*static*/ void presto_start_master_timer(void) {
   // store (current plus CYCLES_PER_TICK)
   TOC2 = (WORD)(TCNT + CLOCKS_PER_TICK);
   // request output compare interrupt
   TMSK1 |= TMSK1_OC2I;
   // clear the OUTPUT COMPARE flag
   // writing O's makes no change, writing 1's clears the bit
   TFLG1 = TFLG1_OC2F;
   // counter disconnected from output pin logic
   TCTL1 &= ~(TCTL1_OM2|TCTL1_OL2);
}

////////////////////////////////////////////////////////////////////////////////

/*static*/ void presto_restart_master_timer(void) {
   // store (last plus CLOCKS_PER_TICK)
   TOC2 = (WORD)(TOC2 + CLOCKS_PER_TICK);
   // clear the OUTPUT COMPARE flag
   // writing O's makes no change, writing 1's clears the bit
   TFLG1 = TFLG1_OC2F;
}

////////////////////////////////////////////////////////////////////////////////
