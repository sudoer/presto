
#include "hc11regs.h"
#include "system.h"
#include "presto.h"
#include "kernel\kernel.h"

////////////////////////////////////////////////////////////////////////////////

#define CYCLES_PER_MS     2000
#define MS_PER_TICK       100
#define CYCLES_PER_TICK   CYCLES_PER_MS*MS_PER_TICK
#define IDLE_PRIORITY     0
#define IDLE_STACK_SIZE   50

////////////////////////////////////////////////////////////////////////////////

#define DISABLE_CCR_INTERRUPT_BIT      asm("oraa #0x10");
#define ENABLE_CCR_INTERRUPT_BIT      asm("anda ~#0x10");

////////////////////////////////////////////////////////////////////////////////

// GLOBAL VARIABLES
// These are used to pass arguments to inline assembly routines

/*static*/ BYTE * global_new_sp=NULL;
/*static*/ BYTE ** global_old_sp_p=NULL;
/*static*/ void (*global_new_fn)(void)=NULL;
/*static*/ BYTE * global_save_sp;     // do not put this on the stack (BOOM)

////////////////////////////////////////////////////////////////////////////////

// STATIC GLOBAL VARIABLES

/*static*/ PRESTO_TCB_T * current_tcb_p=NULL;
/*static*/ PRESTO_TID_T current_tid=0;
/*static*/ PRESTO_TCB_T * tcb_head_p=NULL;
/*static*/ PRESTO_TCB_T * free_tcb_p=NULL;
/*static*/ PRESTO_TCB_T tcb_list[MAX_TASKS];

/*static*/ PRESTO_TIME_T presto_master_clock;
/*static*/ BYTE presto_initialized=0;

// idle task stuff
/*static*/ BYTE idle_stack[IDLE_STACK_SIZE];
/*static*/ PRESTO_TCB_T * idle_tcb_p;
/*static*/ BYTE idle_tid;

// mail stuff
/*static*/ PRESTO_MESSAGE_T * free_mail_p=NULL;
/*static*/ PRESTO_MESSAGE_T * po_mail_p=NULL;
/*static*/ PRESTO_MESSAGE_T mail_list[MAX_MESSAGES];

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

void presto_system_isr_wrapper(void);
void presto_system_isr(void);
void context_switch_wrapper(void);
void context_switch(void);

////////////////////////////////////////////////////////////////////////////////
//   I N I T I A L I Z A T I O N
////////////////////////////////////////////////////////////////////////////////

void presto_init(void) {
   BYTE count;

   // initialize once and only once
   if(presto_initialized) return;
   presto_initialized++;

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
   // must be done after presto_initialized++ because of initialization check
   idle_tid=presto_create_task(idle_task,idle_stack,IDLE_STACK_SIZE,IDLE_PRIORITY);
   idle_tcb_p=tid_to_tcbptr(idle_tid);
}

////////////////////////////////////////////////////////////////////////////////

void presto_start_scheduler(void) {

   if(presto_initialized==0) presto_fatal_error();

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
      presto_fatal_error();
   }
   current_tid=current_tcb_p->task_id;

   // SET UP A NEW STACK AND START EXECUTION USING IT

   // these parameters will be used in inline assembly...
   // must be put in global space, not on stack
   global_new_sp=current_tcb_p->stack_ptr;

   asm("lds _global_new_sp");

   // Clear interrupt mask bit (to enable ints) in the CC register on the stack.
   // That way, the new task will have interrupts enabled when it wakes up.
   asm("pula");
   ENABLE_CCR_INTERRUPT_BIT;
   asm("psha");

   // Normally, this function would end with an RTS, but we want to act EXACTLY
   // the same as if we had just been inside of an interrupt.  So we manually
   // call RTI here to pop the registers and "run" the new task.
   asm("rti");

   // we never get here
   presto_fatal_error();
}

////////////////////////////////////////////////////////////////////////////////
//   T A S K   M A N A G E M E N T
////////////////////////////////////////////////////////////////////////////////

PRESTO_TID_T presto_create_task( void (*func)(void), BYTE * stack, short stack_size, BYTE priority ) {

   PRESTO_TCB_T * new_tcb_p;

   if(presto_initialized==0) presto_fatal_error();

   if(free_tcb_p==NULL) {
      // There are no more TCB's left.
      presto_fatal_error();
      return -1;
   }

   // we're about to mess with tasks, TCB's... interrupts off
   INTR_OFF();

   // allocate TCB for new task
   new_tcb_p=free_tcb_p;
   free_tcb_p=free_tcb_p->next;

   // initialize TCB elements
   // new_tcb_p->task_id is already assigned
   new_tcb_p->stack_top=stack+stack_size-1;
   new_tcb_p->stack_bottom=stack;
   new_tcb_p->stack_ptr=new_tcb_p->stack_top;
   new_tcb_p->priority=priority;
   new_tcb_p->state=STATE_READY;
   new_tcb_p->mailbox_head=NULL;
   new_tcb_p->mailbox_tail=NULL;

   // SET UP NEW STACK USING ASSEMBLY LANGUAGE

   // these parameters will be used in inline assembly...
   // must be put in global space, not on stack
   global_new_sp=new_tcb_p->stack_ptr;
   global_new_fn=func;

   // store our own SP so we can work on the new task
   asm("sts _global_save_sp");

   // load empty SP from task so we can initialize it
   asm("lds _global_new_sp");

   // Set presto_fatal_error as the "return pc" of a new task.  If some bozo
   // tries to return out of his task's main function, we will cause an alarm.
   asm("ldd #_presto_fatal_error");
   asm("pshb");
   asm("psha");

   // push the actual function call on the stack
   asm("ldd _global_new_fn");
   asm("pshb");
   asm("psha");

   // push any old stinkin' registers onto the stack
   // they'll be pulled off when we start running
   // we push in interrupt-stack order
   asm("ldaa #0");
   asm("psha"); // Y(L) register
   asm("psha"); // Y(H) register
   asm("psha"); // X(L) register
   asm("psha"); // X(H) register
   asm("psha"); // A register
   asm("psha"); // B register
   asm("psha"); // Initial Condition Codes (I bit cleared)

   // save task SP in TCB
   asm("sts _global_new_sp");
   // re-load our own SP so we can return
   asm("lds _global_save_sp");

   // recover the altered stack pointer and save it in the TCB
   new_tcb_p->stack_ptr=global_new_sp;

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
   INTR_ON();

   return new_tcb_p->task_id;
}

////////////////////////////////////////////////////////////////////////////////

void presto_kill_self(void) {
   // TODO - remove TCB from list
   presto_fatal_error();
}

////////////////////////////////////////////////////////////////////////////////
//   C O N T E X T   S W I T C H I N G   ( I N T E R R U P T )
////////////////////////////////////////////////////////////////////////////////

#pragma interrupt presto_system_isr_wrapper
void presto_system_isr_wrapper(void) {

   // The ICC compiler adds a "jsr __enterb" at the beginning of my interrupt
   // service routine.  Apparently, it is concerned with preserving the state
   // of the X register, and it tries to push it onto the stack and then do some
   // funny math.  At the end of the ISR, it tries to undo all of the mess, and
   // it even ends the ISR with a jump instruction.  Yikes!  I use this label
   // to by-pass this destructive behavior at the top, and later I use an
   // inline "RTI" instruction to by-pass the stuff at the bottom.
   asm("_presto_system_isr::");

   INTR_OFF();

   // take care of clock things
   presto_master_clock=clock_add(presto_master_clock,MS_PER_TICK);
   presto_restart_master_timer();

   // check mail
   if(deliver_mail()>0) {

      // check to see if we've clobbered our stack
      if(((current_tcb_p->stack_ptr)>(current_tcb_p->stack_top))
      ||((current_tcb_p->stack_ptr)<(current_tcb_p->stack_bottom)))
         presto_fatal_error();

      // these parameters will be used in inline assembly...
      // must be put in global space, not on stack

      // the ISR will save old SP in old TCB
      global_old_sp_p=&(current_tcb_p->stack_ptr);

      // pick next task to run
      current_tcb_p=presto_next_tcb_to_run();
      current_tid=current_tcb_p->task_id;

      // end of ISR will set up new stack
      global_new_sp=current_tcb_p->stack_ptr;

      // swap the stack pointers
      asm("ldy _global_old_sp_p");
      asm("sts 0,y");
      asm("lds _global_new_sp");
   }

   // Clear interrupt mask bit (to enable ints) in the CC register on the stack.
   // That way, the new task will have interrupts enabled when it wakes up.
   asm("pula");
   ENABLE_CCR_INTERRUPT_BIT;
   asm("psha");

   // The end of this function SHOULD be an RTI (instead of RTS), because it is
   // an interrupt.  But the ICC compiler adds a lot of stuff at the beginning
   // and the end of interrupt service routines.  Specifically, it is messing
   // with the X register (pushing it onto the stack) because it uses that as
   // a frame pointer.  So I will add my RTI here explicitly, to force the
   // behavior that I want.
   // Now we will pop the stack and "run" the new task.
   asm("rti");

   // we never get here
   presto_fatal_error();
}

////////////////////////////////////////////////////////////////////////////////

void context_switch_wrapper(void) {

   asm("_context_switch::");

   // check to see if the old task has clobbered its stack
   if(((current_tcb_p->stack_ptr)>(current_tcb_p->stack_top))
   ||((current_tcb_p->stack_ptr)<(current_tcb_p->stack_bottom)))
      presto_fatal_error();

   // the inline asm will save old SP in old TCB
   global_old_sp_p=&(current_tcb_p->stack_ptr);

   // pick next task to run
   current_tcb_p=presto_next_tcb_to_run();
   current_tid=current_tcb_p->task_id;

   // check to see if the new task has clobbered its stack
   if(((current_tcb_p->stack_ptr)>(current_tcb_p->stack_top))
   ||((current_tcb_p->stack_ptr)<(current_tcb_p->stack_bottom)))
      presto_fatal_error();

   // call asm routine to set up new stack
   // when we return, we'll be another process
   // the asm routine will re-enable interrupts
   global_new_sp=current_tcb_p->stack_ptr;

   // save the registers (in the same order that an interrupt does)
   asm("pshy");  // 2 bytes (Low, then High)
   asm("pshx");  // 2 bytes (Low, then High)
   asm("psha");  // 1 byte
   asm("pshb");  // 1 byte
   asm("tpa");
   ENABLE_CCR_INTERRUPT_BIT;  // enable interrupts in pushed CC register
   asm("psha");  // 1 byte, the condition codes

   // swap the stack pointers
   asm("ldy _global_old_sp_p");
   asm("sts 0,y");
   asm("lds _global_new_sp");

   // Clear interrupt mask bit (to enable ints) in the CC register on the stack.
   // That way, the new task will have interrupts enabled when it wakes up.
   asm("pula");
   ENABLE_CCR_INTERRUPT_BIT;
   asm("psha");

   // Normally, this function would end with an RTS, but we want to act EXACTLY
   // the same as if we had just been inside of an interrupt.  So we manually
   // call RTI here to pop the registers and "run" the new task.
   asm("rti");

   // we never get here
   presto_fatal_error();
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
   PRESTO_TCB_T * tcb_ptr;

   // we're going to mess with the PO mail list... interrupts off
   INTR_OFF();

   // check to see if there's room
   if(free_mail_p==NULL) {
      presto_fatal_error();
   }

   // check to see that the recipient is an alive task
   tcb_ptr=tid_to_tcbptr(to);
   if(tcb_ptr==NULL) {
      presto_fatal_error();
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
   INTR_ON();

   return 0;
}

////////////////////////////////////////////////////////////////////////////////

BYTE presto_wait_for_message(PRESTO_MAIL_T * payload_p) {
   PRESTO_MESSAGE_T * msg_p;

   // we're about to switch to a new task... interrupts off
   INTR_OFF();

   // we will only sleep if there are no messages in our queue
   if(current_tcb_p->mailbox_head==NULL) {
      // no mail, so we can sleep
      current_tcb_p->state=STATE_BLOCKED;
      asm("swi");
      // When we wake up, we'll be ready to recieve our mail.
      // Interrupts will be enabled.
   }

   // we're about to mess with the mail list... interrupts off
   INTR_OFF();
   // we're going to use this a lot, so dereference now
   msg_p=current_tcb_p->mailbox_head;

   // get one message from the task's mail queue
   if(msg_p==NULL) {
      // there are no messages in the task's mail list
      presto_fatal_error();
   }

   // are we being paranoid?
   if((msg_p->to_tcb_p)!=current_tcb_p) {
      presto_fatal_error();
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
   INTR_ON();

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
   if(tid>=MAX_TASKS) presto_fatal_error();
   if(tcb_list[tid].state==STATE_INACTIVE) return NULL;
   return &tcb_list[tid];
}

////////////////////////////////////////////////////////////////////////////////
//   H A R D W A R E   T I M E R / C O U N T E R
////////////////////////////////////////////////////////////////////////////////

/*static*/ void presto_start_master_timer(void) {
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

/*static*/ void presto_restart_master_timer(void) {
   // store (last plus CYCLES_PER_TICK)
   TOC2 = TOC2 + CYCLES_PER_TICK;
   // clear the OUTPUT COMPARE flag
   // writing O's makes no change, writing 1's clears the bit
   TFLG1 = TFLG1_OC2F;
}

////////////////////////////////////////////////////////////////////////////////

