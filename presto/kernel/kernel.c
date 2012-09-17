////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "presto.h"
#include "error.h"
#include "chip/hc11regs.h"
#include "chip/boot.h"
#include "chip/intvect.h"
#include "chip/locks.h"
#include "kernel/clock.h"
#include "kernel/kernel.h"
#include "kernel/mail.h"


////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////

// DEBUG
#define STATIC // static

// CONFIGURATION
#define MAX_TASKS           MAX_USER_TASKS+1
#define IDLE_PRIORITY       0
#define IDLE_STACK_SIZE     50

// TIMING, COMPUTED FROM VALUES IN KERNEL.H
#define CLOCKS_PER_MS       CYCLES_PER_MS*CYCLES_PER_CLOCK/CLOCK_PRESCALE
#define CLOCKS_PER_TICK     CLOCKS_PER_MS*MS_PER_TICK

////////////////////////////////////////////////////////////////////////////////
//   D A T A   T Y P E S
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N   P R O T O T Y P E S
////////////////////////////////////////////////////////////////////////////////

// system timer
STATIC void systimer_Start(void);
STATIC void systimer_Restart(void);
STATIC void systimer_isr(void) __attribute__((interrupt));

// scheduling
STATIC KERNEL_TCB_T * scheduler_FindNextReadyTask(void);
STATIC void idle_task(void);

// context switching
STATIC void context_switch_isr(void) __attribute__((interrupt));

// utilities
STATIC KERNEL_TCB_T * tid_to_tcbptr(BYTE tid);


////////////////////////////////////////////////////////////////////////////////
//   K E R N E L - O N L Y   G L O B A L   D A T A
////////////////////////////////////////////////////////////////////////////////

KERNEL_TCB_T * current_tcb_p=NULL;
PRESTO_TIME_T kernel_clock;

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   G L O B A L   D A T A
////////////////////////////////////////////////////////////////////////////////

// task control blocks
STATIC KERNEL_TCB_T * tcb_head_p=NULL;
STATIC KERNEL_TCB_T * free_tcb_p=NULL;
STATIC KERNEL_TCB_T tcb_list[MAX_TASKS];

// idle task stuff
STATIC BYTE idle_stack[IDLE_STACK_SIZE];
STATIC KERNEL_TCB_T * idle_tcb_p;
STATIC BYTE idle_tid;

// miscellaneous
STATIC BYTE presto_initialized=0;

// These are used to pass arguments to inline assembly routines.
// Do not put these on the stack (BOOM).
STATIC KERNEL_TCB_T * old_tcb_p;
STATIC BYTE * global_new_sp=NULL;
STATIC BYTE * global_old_sp=NULL;
STATIC BYTE ** old_task_stack_pointer_p;

////////////////////////////////////////////////////////////////////////////////
//   K E R N E L - O N L Y   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void kernel_flag_set(KERNEL_TCB_T * tcb_p, KERNEL_FLAG_T flag) {
   tcb_p->flags |= flag;
}

////////////////////////////////////////////////////////////////////////////////
//   E X T E R N A L   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void presto_init(void) {
   BYTE count;
   BYTE flags;

   if(presto_initialized) return;

   // initialize master clock
   clock_reset(&kernel_clock);

   // initialize TCB list
   for(count=0;count<MAX_TASKS;count++) {
      tcb_list[count].next=&tcb_list[count+1];
      tcb_list[count].task_id=count;
      tcb_list[count].in_use=FALSE;
   }
   tcb_list[MAX_TASKS-1].next=NULL;
   free_tcb_p=&tcb_list[0];

   kernel_mail_init();

   // must be done before creating idle task
   presto_initialized=1;

   // initialize idle task
   idle_tid=presto_create_task(idle_task,idle_stack,IDLE_STACK_SIZE,IDLE_PRIORITY);
   idle_tcb_p=tid_to_tcbptr(idle_tid);
}

////////////////////////////////////////////////////////////////////////////////

KERNEL_TID_T presto_create_task( void (*func)(void), BYTE * stack, short stack_size, BYTE priority ) {

   KERNEL_TCB_T * new_tcb_p;
   BYTE * sp;
   MISCWORD xlate;    // to split a word into two bytes
   WORD lock;

   if(!presto_initialized) {
      presto_fatal_error(ERROR_KERNEL_CREATE_BEFORE_INIT);
   }

   if(free_tcb_p==NULL) {
      // There are no more TCB's left.
      presto_fatal_error(ERROR_KERNEL_CREATE_NO_MORE_TCBS);
      return -1;
   }

   // we're about to mess with tasks, TCB's... interrupts off
   presto_lock_save(lock);

   // allocate TCB for new task
   new_tcb_p=free_tcb_p;
   free_tcb_p=free_tcb_p->next;

   // initialize TCB elements
   // new_tcb_p->task_id is already assigned
   new_tcb_p->stack_top=stack+stack_size-1;
   new_tcb_p->stack_bottom=stack;
   new_tcb_p->priority=priority;
   new_tcb_p->wait_mask=0;
   new_tcb_p->flags=0;
   new_tcb_p->in_use=TRUE;

   /*
   new_tcb_p->mailbox_head=NULL;
   new_tcb_p->mailbox_tail=NULL;
   */

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
   *sp--=0xBB;         // _.tmp 0000(L)
   *sp--=0xAA;         // _.tmp 0000(H)
   *sp--=0xDD;         // _.z   0002(L)
   *sp--=0xCC;         // _.z   0002(H)
   *sp--=0xFF;         // _.xy  0004(L)
   *sp--=0xEE;         // _.xy  0004(H)

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
      KERNEL_TCB_T * ptr=tcb_head_p;
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
   presto_unlock_restore(lock);

   return new_tcb_p->task_id;
}

////////////////////////////////////////////////////////////////////////////////

void presto_start_scheduler(void) {

   // we're about to switch to our first task... interrupts off
   presto_lock();

   set_interrupt(INTR_TOC2, systimer_isr);
   set_interrupt(INTR_SWI, context_switch_isr);

   // start timer interrupts for pre-emption
   systimer_Start();

   // pick next task to run
   // first task in list is highest priority and is ready
   current_tcb_p=tcb_head_p;
   if(current_tcb_p==NULL) {
      presto_fatal_error(ERROR_KERNEL_START_NOTASKS);
   }

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
   presto_fatal_error(ERROR_KERNEL_START_AFTER_RTI);
}

////////////////////////////////////////////////////////////////////////////////

KERNEL_FLAG_T presto_wait(KERNEL_FLAG_T wait_for) {

   // Save wait_for for later.
   current_tcb_p->wait_mask=wait_for;

   // If we do not need to wait, then keep going.
   if((wait_for & current_tcb_p->flags)==0) {
      // Current task must wait.
      // We should re-evaluate priorities and swap tasks.
      asm("swi");
   }

   // When we wake up, we'll be ready to run again.
   // Interrupts will be enabled.

   // Save wait_for for later.
   current_tcb_p->wait_mask=0;

   // We must re-evaluate here... may have changed since our last check
   // (because we SWI'd and swapped to another task).
   return (wait_for & current_tcb_p->flags);
}

////////////////////////////////////////////////////////////////////////////////

void presto_flag_set(KERNEL_TCB_T * tcb_p, KERNEL_FLAG_T flag) {
   kernel_flag_set(tcb_p,flag);
}

////////////////////////////////////////////////////////////////////////////////

void presto_flag_clear(KERNEL_FLAG_T flag) {
   current_tcb_p->flags &= ~flag;
}

////////////////////////////////////////////////////////////////////////////////

void presto_get_clock(PRESTO_TIME_T * clk) {
   *clk=kernel_clock;
}

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

STATIC KERNEL_TCB_T * scheduler_FindNextReadyTask(void) {
   // pick highest priority ready task to run
   KERNEL_TCB_T * ptr=tcb_head_p;
   while(ptr!=NULL) {
      if(ptr->wait_mask==0) return ptr;
      if(ptr->wait_mask & ptr->flags) return ptr;
      ptr=ptr->next;
   }
   // should never get here
   presto_fatal_error(ERROR_KERNEL_SCHEDULER_ERROR);
   return NULL;
}

////////////////////////////////////////////////////////////////////////////////

STATIC void systimer_Start(void) {
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

STATIC void systimer_Restart(void) {
   // store (last plus CLOCKS_PER_TICK)
   TOC2 = (WORD)(TOC2 + CLOCKS_PER_TICK);
   // clear the OUTPUT COMPARE flag
   // writing O's makes no change, writing 1's clears the bit
   TFLG1 = TFLG1_OC2F;
}

////////////////////////////////////////////////////////////////////////////////

STATIC void systimer_isr(void) {
   WORD lock;
   // take care of clock things
   clock_add_ms(&kernel_clock,MS_PER_TICK);
   systimer_Restart();
   // check timers
   if(kernel_timer_poll()>0) {
      asm("swi");
   }
}

////////////////////////////////////////////////////////////////////////////////

STATIC void context_switch_isr(void) {
   // registers are pushed when SWI is executed
   // pseudo-registers are also pushed (tmp,z,xy)

   #ifdef CHECK_STACK_CLOBBERING
      // check to see if the old task has clobbered its stack
      if(((current_tcb_p->stack_ptr)>(current_tcb_p->stack_top))
      ||((current_tcb_p->stack_ptr)<(current_tcb_p->stack_bottom)))
         presto_fatal_error(ERROR_KERNEL_CONTEXTSWITCH_STACKCLOBBERED);
   #endif // CHECK_STACK_CLOBBERING

   // the inline asm will save old SP in old TCB
   old_task_stack_pointer_p=&(current_tcb_p->stack_ptr);

   // pick next task to run
   old_tcb_p=current_tcb_p;
   current_tcb_p=scheduler_FindNextReadyTask();

   // check to see if the same task won...
   // only manipulate stacks if there is a context SWITCH
   if(current_tcb_p!=old_tcb_p) {

      // there's a new "highest priority ready task"

      BITNOT(PORTA,3);   // toggle speaker

      #ifdef CHECK_STACK_CLOBBERING
         // check to see if the new task has clobbered its stack
         if(((current_tcb_p->stack_ptr)>(current_tcb_p->stack_top))
         ||((current_tcb_p->stack_ptr)<(current_tcb_p->stack_bottom)))
            presto_fatal_error(ERROR_KERNEL_CONTEXTSWITCH_STACKCLOBBERED);
      #endif // CHECK_STACK_CLOBBERING

      // call asm routine to set up new stack
      // when we return, we'll be another process
      // the asm routine will re-enable interrupts
      global_new_sp=current_tcb_p->stack_ptr;

      // store the old stack pointer in a global place
      asm("sts global_old_sp");
      // put the old stack pointer into the old task's TCB
      *old_task_stack_pointer_p=global_old_sp;
      // load the new stack pointer
      asm("lds global_new_sp");

   }

   // pseudo-registers are pulled (xy,z,tmp)
   // then normal registers are pulled
}

////////////////////////////////////////////////////////////////////////////////

STATIC void idle_task(void) {
   while(1) {
      // do nothing
   }
}

////////////////////////////////////////////////////////////////////////////////

STATIC KERNEL_TCB_T * tid_to_tcbptr(BYTE tid) {
   if(tid>=MAX_TASKS) {
      presto_fatal_error(ERROR_KERNEL_TIDTOTCB_RANGE);
   }
   if(tcb_list[tid].in_use==FALSE) return NULL;
   return &tcb_list[tid];
}

////////////////////////////////////////////////////////////////////////////////


