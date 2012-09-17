////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "presto.h"
#include "error.h"
#include "chip/boot.h"
#include "chip/intvect.h"
#include "chip/locks.h"
#include "chip/misc_hw.h"
#include "kernel/clock.h"
#include "config.h"
#include "kernel/kernel_types.h"
#include "kernel/kernel_funcs.h"
#include "kernel/mail_types.h"
#include "kernel/mail_funcs.h"
#include "kernel/timer_types.h"
#include "kernel/timer_funcs.h"


////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////

// CONFIGURATION
#define MAX_TASKS           MAX_USER_TASKS+1
#define IDLE_PRIORITY       0
#define IDLE_STACK_SIZE     50

#define WAIT_MASK_READY     0x00

////////////////////////////////////////////////////////////////////////////////
//   D A T A   T Y P E S
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N   P R O T O T Y P E S
////////////////////////////////////////////////////////////////////////////////

// system timer
static void systimer_isr(void) __attribute__((interrupt));

// scheduling
static KERNEL_TCB_T * scheduler_FindNextReadyTask(void);
static void idle_task(void);

// priority queue
static void priority_queue_insert_tcb(KERNEL_TCB_T * tcb_p, KERNEL_PRIORITY_T priority);
static void priority_queue_remove_tcb(KERNEL_TCB_T * tcb_p);

// context switching
static void context_switch_isr(void) __attribute__((interrupt));

// utilities
static KERNEL_TCB_T * tid_to_tcbptr(KERNEL_TID_T tid);

////////////////////////////////////////////////////////////////////////////////
//   K E R N E L - O N L Y   G L O B A L   D A T A
////////////////////////////////////////////////////////////////////////////////

// task control blocks
KERNEL_TCB_T * kernel_current_tcb_p=NULL;

// clock
KERNEL_TIME_T kernel_clock;


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   G L O B A L   D A T A
////////////////////////////////////////////////////////////////////////////////

// task control blocks
static KERNEL_TCB_T * tcb_head_p=NULL;
static KERNEL_TCB_T * free_tcb_p=NULL;
static KERNEL_TCB_T tcb_list[MAX_TASKS];

// idle task stuff
static BYTE idle_stack[IDLE_STACK_SIZE];
static KERNEL_TCB_T * idle_tcb_p;
static BYTE idle_tid;

// miscellaneous
static BYTE presto_initialized=0;

// These are used to pass arguments to inline assembly routines.
// Do not put these on the stack (BOOM).
static KERNEL_TCB_T * old_tcb_p;
static BYTE * global_new_sp=NULL;
static BYTE * global_old_sp=NULL;
static BYTE ** old_task_stack_pointer_p;

////////////////////////////////////////////////////////////////////////////////
//   E X T E R N A L   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


void presto_init(void) {
   BYTE count;
   BYTE triggers;

   if(presto_initialized) return;

   // initialize master clock
   clock_reset(&kernel_clock);

   // initialize TCB list
   for(count=0;count<MAX_TASKS;count++) {
      tcb_list[count].task_id=count;
      tcb_list[count].next=&tcb_list[count+1];
      tcb_list[count].task_id=count;
      //tcb_list[count].running_state=STATE_INACTIVE;
   }
   tcb_list[MAX_TASKS-1].next=NULL;
   free_tcb_p=&tcb_list[0];

   kernel_mail_init();
   kernel_timer_init();
   kernel_semaphore_init();

   // must be done before creating idle task
   presto_initialized=1;

   // initialize idle task
   idle_tid=presto_create_task(idle_task,idle_stack,IDLE_STACK_SIZE,IDLE_PRIORITY);
   idle_tcb_p=tid_to_tcbptr(idle_tid);
}


////////////////////////////////////////////////////////////////////////////////


KERNEL_TID_T presto_create_task( void (*func)(void), BYTE * stack, short stack_size, KERNEL_PRIORITY_T priority ) {

   KERNEL_TCB_T * new_tcb_p;
   BYTE * sp;
   MISCWORD xlate;    // to split a word into two bytes
   KERNEL_LOCK_T lock;

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
   new_tcb_p->current_priority=priority;
   new_tcb_p->natural_priority=priority;
   new_tcb_p->wait_mask=WAIT_MASK_READY;
   new_tcb_p->triggers=0x00;
   //new_tcb_p->running_state=STATE_READY;

   // SET UP NEW STACK (stack grown down)

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

   priority_queue_insert_tcb(new_tcb_p,priority);

   // we're done messing with the task list... interrupts back on
   presto_unlock_restore(lock);

   return new_tcb_p->task_id;
}


///////////////////////////////////////////////////////////////////////////////


void presto_start_scheduler(void) {

   // we're about to switch to our first task... interrupts off
   presto_lock();

   set_interrupt(INTR_TOC2, systimer_isr);
   set_interrupt(INTR_SWI, context_switch_isr);

   // start timer interrupts for pre-emption
   hwtimer_Start();

   // pick next task to run
   // first task in list is highest priority and is ready
   kernel_current_tcb_p=tcb_head_p;
   if(kernel_current_tcb_p==NULL) {
      presto_fatal_error(ERROR_KERNEL_START_NOTASKS);
   }

   // SET UP A NEW STACK AND START EXECUTION USING IT

   // these parameters will be used in inline assembly...
   // must be put in global space, not on stack
   global_new_sp=kernel_current_tcb_p->stack_ptr;

   asm("lds global_new_sp");
   asm("pulx");  // _.xy
   asm("pulx");  // _.z
   asm("pulx");  // _.tmp
   asm("rti");

   // we never get here
   presto_fatal_error(ERROR_KERNEL_START_AFTER_RTI);
}


////////////////////////////////////////////////////////////////////////////////


KERNEL_TRIGGER_T presto_wait(KERNEL_TRIGGER_T wait_for) {

   // Save wait_for for later.
   kernel_current_tcb_p->wait_mask=wait_for;

   // If we do not need to wait, then keep going.
   if((wait_for & kernel_current_tcb_p->triggers)==0) {
      // Current task must wait.
      // We should re-evaluate priorities and swap tasks.
      asm("swi");
   }

   // When we wake up, we'll be ready to run again.
   // Interrupts will be enabled.

   kernel_current_tcb_p->wait_mask=WAIT_MASK_READY;

   // We must re-evaluate here... may have changed since our last check
   // (because we SWI'd and swapped to another task).
   return (wait_for & kernel_current_tcb_p->triggers);
}


////////////////////////////////////////////////////////////////////////////////


void presto_trigger_set(KERNEL_TRIGGER_T trigger) {
   kernel_trigger_set(kernel_current_tcb_p,trigger);
   // No need to asm("swi").
   // We are not waiting on this trigger (we are running now).
}


////////////////////////////////////////////////////////////////////////////////


void presto_trigger_clear(KERNEL_TRIGGER_T trigger) {
   kernel_current_tcb_p->triggers &= ~trigger;
}


////////////////////////////////////////////////////////////////////////////////


KERNEL_TRIGGER_T presto_trigger_poll(KERNEL_TRIGGER_T test) {
   return (test & kernel_current_tcb_p->triggers);
}


////////////////////////////////////////////////////////////////////////////////


void presto_trigger_send(KERNEL_TID_T tid, KERNEL_TRIGGER_T trigger) {
   KERNEL_TCB_T * tcb_p;
   tcb_p=tid_to_tcbptr(tid);
   kernel_trigger_set(tcb_p,trigger);
   asm("swi");
}


////////////////////////////////////////////////////////////////////////////////


void presto_get_clock(KERNEL_TIME_T * clk) {
   *clk=kernel_clock;
}


////////////////////////////////////////////////////////////////////////////////
//   K E R N E L - O N L Y   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


void kernel_trigger_set(KERNEL_TCB_T * tcb_p, KERNEL_TRIGGER_T trigger) {
   tcb_p->triggers |= trigger;
}


////////////////////////////////////////////////////////////////////////////////


void kernel_priority_override(KERNEL_TCB_T * tcb_p, KERNEL_PRIORITY_T new_priority) {
   priority_queue_remove_tcb(tcb_p);
   tcb_p->current_priority=new_priority;
   priority_queue_insert_tcb(tcb_p, new_priority);
}


////////////////////////////////////////////////////////////////////////////////


void kernel_priority_restore(KERNEL_TCB_T * tcb_p) {
   priority_queue_remove_tcb(tcb_p);
   tcb_p->current_priority=tcb_p->natural_priority;
   priority_queue_insert_tcb(tcb_p, tcb_p->natural_priority);
}


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


static void priority_queue_insert_tcb(KERNEL_TCB_T * new_tcb_p, BYTE new_priority) {

   // INSERT NEW TCB INTO LIST IN PRIORITY ORDER

/*
   if(tcb_head_p==NULL) {
      // we are the only TCB in the list
      tcb_head_p=new_tcb_p;
      new_tcb_p->next=NULL;
   } else if(new_priority>(tcb_head_p->current_priority)) {
      // we are the first TCB in the list
      new_tcb_p->next=tcb_head_p;
      tcb_head_p=new_tcb_p;
   } else {
      KERNEL_TCB_T * ptr=tcb_head_p;
      while(ptr->next!=NULL) {
         if(new_priority>(ptr->next->current_priority)) break;
         ptr=ptr->next;
      }
      // ptr->next is either NULL or lower current_priority than us
      // either way, we want to get inserted between ptr and ptr->next
      new_tcb_p->next=ptr->next;
      ptr->next=new_tcb_p;
   }
*/

   KERNEL_TCB_T ** p0;
   KERNEL_TCB_T * p1;
   KERNEL_TCB_T * temp;
   KERNEL_LOCK_T lock;

   presto_lock_save(lock);
   // Start at the head, look at priorities.
   p0=&tcb_head_p;
   p1=tcb_head_p;
   while(1) {
      if((p1==NULL)||((p1->current_priority)<new_priority)) {
         // Insert our item between p0 and p1
         new_tcb_p->next=p1;
         *p0=new_tcb_p;
         break; // out of the while(1) loop
      }
      p0=&(p1->next);
      p1=p1->next;
   }
   presto_unlock_restore(lock);
}


////////////////////////////////////////////////////////////////////////////////


static void priority_queue_remove_tcb(KERNEL_TCB_T * old_tcb_p) {
   KERNEL_TCB_T * ptr;
   KERNEL_LOCK_T lock;

   presto_lock_save(lock);
   // If the old TCB is at the head of the list, remove it.
   while(tcb_head_p==old_tcb_p) {
      tcb_head_p=tcb_head_p->next;
   }
   // First TCB in the list is not the old TCB.
   // Go through the list, looking for the old TCB.
   ptr=tcb_head_p;
   while(ptr!=NULL) {
      if(ptr->next==old_tcb_p) ptr->next=old_tcb_p->next;
      else ptr=ptr->next;
   }
   presto_unlock_restore(lock);
}


////////////////////////////////////////////////////////////////////////////////


static KERNEL_TCB_T * scheduler_FindNextReadyTask(void) {
   // pick highest priority ready task to run
   KERNEL_TCB_T * ptr=tcb_head_p;
   while(ptr!=NULL) {
      if(ptr->wait_mask==WAIT_MASK_READY) return ptr;
      if(ptr->wait_mask & ptr->triggers) return ptr;
      ptr=ptr->next;
   }
   // should never get here
   presto_fatal_error(ERROR_KERNEL_SCHEDULER_ERROR);
   return NULL;
}


////////////////////////////////////////////////////////////////////////////////


static void systimer_isr(void) {
   KERNEL_LOCK_T lock;
   // take care of clock things
   clock_add_ms(&kernel_clock,MS_PER_TICK);
   hwtimer_Restart();
   // check timers
   if(kernel_timer_tick()>0) {
      asm("swi");
   }
}


////////////////////////////////////////////////////////////////////////////////


static void context_switch_isr(void) {
   // registers are pushed when SWI is executed
   // pseudo-registers are also pushed (tmp,z,xy)

   #ifdef CHECK_STACK_CLOBBERING
      // check to see if the old task has clobbered its stack
      if(((kernel_current_tcb_p->stack_ptr)>(kernel_current_tcb_p->stack_top))
      ||((kernel_current_tcb_p->stack_ptr)<(kernel_current_tcb_p->stack_bottom)))
         presto_fatal_error(ERROR_KERNEL_CONTEXTSWITCH_STACKCLOBBERED);
   #endif // CHECK_STACK_CLOBBERING

   // the inline asm will save old SP in old TCB
   old_task_stack_pointer_p=&(kernel_current_tcb_p->stack_ptr);

   // pick next task to run
   old_tcb_p=kernel_current_tcb_p;
   kernel_current_tcb_p=scheduler_FindNextReadyTask();

   // check to see if the same task won...
   // only manipulate stacks if there is a context SWITCH
   if(kernel_current_tcb_p!=old_tcb_p) {

      // there's a new "highest priority ready task"

      TOGGLE_SPEAKER();

      #ifdef CHECK_STACK_CLOBBERING
         // check to see if the new task has clobbered its stack
         if(((kernel_current_tcb_p->stack_ptr)>(kernel_current_tcb_p->stack_top))
         ||((kernel_current_tcb_p->stack_ptr)<(kernel_current_tcb_p->stack_bottom)))
            presto_fatal_error(ERROR_KERNEL_CONTEXTSWITCH_STACKCLOBBERED);
      #endif // CHECK_STACK_CLOBBERING

      // call asm routine to set up new stack
      // when we return, we'll be another process
      // the asm routine will re-enable interrupts
      global_new_sp=kernel_current_tcb_p->stack_ptr;

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


static void idle_task(void) {
   while(1) {
      // do nothing
   }
}


////////////////////////////////////////////////////////////////////////////////


static KERNEL_TCB_T * tid_to_tcbptr(KERNEL_TID_T tid) {
/*
   if(tid>=MAX_TASKS) {
      presto_fatal_error(ERROR_KERNEL_TIDTOTCB_RANGE);
   }
   // TODO ??? if(tcb_list[tid].in_use==FALSE) return NULL;
   return &tcb_list[tid];
*/
   KERNEL_TCB_T * ptr=tcb_head_p;
   while(ptr!=NULL) {
      if(ptr->task_id==tid) return ptr;
      ptr=ptr->next;
   }
   return NULL;
}


////////////////////////////////////////////////////////////////////////////////


