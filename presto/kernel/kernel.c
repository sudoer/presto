////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////

// Presto is a pre-emptive priority-driven real time operating system.

// This module is the heart of Presto.  Here is where we keep our list of
// tasks, and where we evaluate priorities to pick who runs.

// In order to do pre-emptive multitasking, we have to perform context
// switches.  This is a pretty simple operation, but it can be tricky to
// get right.  Basically, all of the registers are saved to the stack and
// then the stack pointer is saved in the task's TCB.  The new task's
// stack pointer is read from its TCB, and then the registers are pulled
// from that task's stack (remember, each task has its own stack).

// The process of determining who should run next and then doing a context
// switch has been cleanly encapsulated in an interrupt service routine.
// That way, whenever some event happens that makes a new task ready, we
// simply issue a SWI (software interrupt).

// In order to tell if a task is ready to run or not, Presto uses "ready
// bits" which I call "triggers".  Each task has eight triggers (this can
// be changed in configure.h if needed).  If a task is waiting, then its
// wait_flag will be set to something other than zero.  When one or more
// of the triggers that it is waiting for becomes set, then that task will
// become ready.

// The tasks are kept in a linked list (in priority order), so it is easy
// to traverse the list to find the highest priority ready task.  Presto
// supports temporary over-riding of priorities (in which case the task
// will be moved to a different place in the linked list).  This feature
// is used by the "priority inheritance" feature of semaphores.

// This file only deals with tasks and triggers.  The other O/S primitives
// are implemented their own modules, and their only connection to this
// kernel is through setting triggers.

////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "presto.h"
#include "configure.h"
#include "cpu/error.h"
#include "cpu/boot.h"
#include "cpu/intvect.h"
#include "cpu/locks.h"
#include "cpu/misc_hw.h"
#include "kernel/kernel.h"
#include "kernel/mail.h"
#include "kernel/timer.h"
#include "kernel/semaphore.h"
#include "kernel/memory.h"

////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////

// CONFIGURATION
#define MAX_TASKS           (PRESTO_KERNEL_MAXUSERTASKS+1)
#define IDLE_PRIORITY       0
#define IDLE_STACK_SIZE     0x100

#define WAIT_MASK_READY     ((KERNEL_TRIGGER_T)0)

////////////////////////////////////////////////////////////////////////////////
//   D A T A   T Y P E S
////////////////////////////////////////////////////////////////////////////////

typedef struct KERNEL_TCB_S {
   KERNEL_TASKID_T task_id;
   BYTE * stack_ptr;
   BYTE * stack_top;
   BYTE * stack_bottom;
   KERNEL_PRIORITY_T natural_priority;
   KERNEL_PRIORITY_T current_priority;
   KERNEL_TRIGGER_T wait_mask;
   KERNEL_TRIGGER_T triggers;
   struct KERNEL_TCB_S * next;
} KERNEL_TCB_T;

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N   P R O T O T Y P E S
////////////////////////////////////////////////////////////////////////////////

// scheduling
static KERNEL_TCB_T * scheduler_next_ready(void);
static void idle_task(void);

// priority queue
static void priority_queue_insert_tcb(KERNEL_TCB_T * tcb_p, KERNEL_PRIORITY_T priority);
static void priority_queue_remove_tcb(KERNEL_TCB_T * tcb_p);

// context switching
static void context_switch_isr(void) __attribute__((interrupt));

// utilities
static KERNEL_TCB_T * tid_to_tcbptr(KERNEL_TASKID_T tid);

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   G L O B A L   D A T A
////////////////////////////////////////////////////////////////////////////////

// task control blocks
static KERNEL_TCB_T tcb_list[MAX_TASKS];
static KERNEL_TCB_T * current_tcb_p=NULL;
static KERNEL_TCB_T * tcb_head_p=NULL;
static KERNEL_TCB_T * free_tcb_p=NULL;

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

   if (presto_initialized) return;

   // initialize TCB list
   for (count=0;count<MAX_TASKS;count++) {
      tcb_list[count].task_id=count;
      tcb_list[count].next=&tcb_list[count+1];
   }
   // We want the idle task to have task_id equal to (MAX_TASKS-1)
   // (also equals PRESTO_KERNEL_MAXUSERTASKS).  We want the rest
   // of the tasks to be assigned in order from 0 to (MAXUSERTASKS-1).
   // That way, other parts of the kernel can keep arrays of information
   // about each task (example: mail system has "default_mailbox" array).
   // The size of these arrays can be MAXUSERTASKS and the subscripts
   // in the array will be 0 to (MAXUSERTASKS-1).
   free_tcb_p=&tcb_list[MAX_TASKS-1];
   tcb_list[MAX_TASKS-1].next=&tcb_list[0];
   tcb_list[MAX_TASKS-2].next=NULL;

   // initialize other kernel subsystems
   kernel_timer_init();
   kernel_mail_init();
   kernel_semaphore_init();
   kernel_memory_init();

   // must be done before creating idle task
   presto_initialized=1;

   // initialize idle task
   idle_tid=presto_task_create(idle_task,idle_stack,IDLE_STACK_SIZE,IDLE_PRIORITY);
   idle_tcb_p=tid_to_tcbptr(idle_tid);
}


////////////////////////////////////////////////////////////////////////////////


KERNEL_TASKID_T presto_task_create(void (*func)(void), BYTE * stack, short stack_size, KERNEL_PRIORITY_T priority) {

   KERNEL_TCB_T * new_tcb_p;
   BYTE * sp;
   MISCWORD xlate;    // to split a word into two bytes
   CPU_LOCK_T lock;

   if (!presto_initialized) {
      error_fatal(ERROR_KERNEL_CREATEBEFOREINIT);
   }

   if (free_tcb_p==NULL) {
      // There are no more TCB's left.
      error_fatal(ERROR_KERNEL_NOMORETCBS);
   }

   // we're about to mess with task list... interrupts off
   cpu_lock_save(lock);

   // allocate TCB for new task
   new_tcb_p=free_tcb_p;
   free_tcb_p=free_tcb_p->next;

   // we're done messing with the task list... interrupts back on
   cpu_unlock_restore(lock);

   // initialize TCB elements
   // new_tcb_p->task_id is already assigned
   new_tcb_p->stack_top=stack+stack_size-1;
   new_tcb_p->stack_bottom=stack;
   new_tcb_p->current_priority=priority;
   new_tcb_p->natural_priority=priority;
   new_tcb_p->wait_mask=WAIT_MASK_READY;
   new_tcb_p->triggers=(KERNEL_TRIGGER_T)0;

   // SET UP NEW STACK (stack grows down)

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

   return new_tcb_p->task_id;
}


///////////////////////////////////////////////////////////////////////////////


void presto_scheduler_start(void) {

   // we're about to switch to our first task... interrupts off
   cpu_lock();

   set_interrupt(INTR_SWI, context_switch_isr);

   kernel_master_clock_start();

   // pick next task to run
   // first task in list is highest priority and is ready
   current_tcb_p=tcb_head_p;
   if (current_tcb_p==NULL) {
      error_fatal(ERROR_KERNEL_NOTASKTOSTART);
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
   error_fatal(ERROR_KERNEL_STARTAFTERRTI);
}


////////////////////////////////////////////////////////////////////////////////


KERNEL_PRIORITY_T presto_priority_get(KERNEL_TASKID_T tid) {
   KERNEL_TCB_T * tcb_p;
   tcb_p=tid_to_tcbptr(tid);
   return tcb_p->current_priority;
}


////////////////////////////////////////////////////////////////////////////////


void presto_priority_set(PRESTO_TASKID_T tid, PRESTO_PRIORITY_T new_priority) {
   KERNEL_TCB_T * tcb_p;
   tcb_p=tid_to_tcbptr(tid);
   if (tcb_p->current_priority!=new_priority) {
      priority_queue_remove_tcb(tcb_p);
      tcb_p->natural_priority=new_priority;
      tcb_p->current_priority=new_priority;
      priority_queue_insert_tcb(tcb_p, new_priority);
   }
}


////////////////////////////////////////////////////////////////////////////////


void presto_priority_override(KERNEL_TASKID_T tid, KERNEL_PRIORITY_T new_priority) {
   KERNEL_TCB_T * tcb_p;
   tcb_p=tid_to_tcbptr(tid);
   if (tcb_p->current_priority!=new_priority) {
      priority_queue_remove_tcb(tcb_p);
      tcb_p->current_priority=new_priority;
      priority_queue_insert_tcb(tcb_p, new_priority);
   }
}


////////////////////////////////////////////////////////////////////////////////


void presto_priority_restore(KERNEL_TASKID_T tid) {
   KERNEL_TCB_T * tcb_p;
   tcb_p=tid_to_tcbptr(tid);
   if (tcb_p->current_priority!=tcb_p->natural_priority) {
      priority_queue_remove_tcb(tcb_p);
      tcb_p->current_priority=tcb_p->natural_priority;
      priority_queue_insert_tcb(tcb_p, tcb_p->natural_priority);
   }
}


////////////////////////////////////////////////////////////////////////////////


KERNEL_TRIGGER_T presto_wait(KERNEL_TRIGGER_T wait_for) {
   CPU_LOCK_T lock;
   KERNEL_TRIGGER_T fired_triggers;

   // Save wait_for for later.
   current_tcb_p->wait_mask=wait_for;

   // If we do not need to wait, then keep going.
   if ((wait_for & current_tcb_p->triggers)==0) {
      // Current task must wait.
      // We should re-evaluate priorities and swap tasks.
      asm("swi");
   }

   // When we wake up, we'll be ready to run again.
   // Interrupts will be enabled.

   // show that we are ready
   current_tcb_p->wait_mask=WAIT_MASK_READY;

   // We must re-evaluate here... may have changed since our last check
   // (because we SWI'd and swapped to another task).  We only clear the
   // triggers that have actually fired (and we must be careful to only
   // clear the ones that we explicity return -- NOT the wait_for mask).
   cpu_lock_save(lock);
   fired_triggers=(wait_for & current_tcb_p->triggers);
   MASKCLR(current_tcb_p->triggers,fired_triggers);
   cpu_unlock_restore(lock);

   return fired_triggers;
}


////////////////////////////////////////////////////////////////////////////////


void presto_trigger_set(KERNEL_TRIGGER_T trigger) {
   MASKSET(current_tcb_p->triggers,trigger);
   // No need to asm("swi").
   // We are not waiting on this trigger (we are running now).
}


////////////////////////////////////////////////////////////////////////////////


void presto_trigger_clear(KERNEL_TRIGGER_T trigger) {
   MASKCLR(current_tcb_p->triggers,trigger);
}


////////////////////////////////////////////////////////////////////////////////


KERNEL_TRIGGER_T presto_trigger_poll(KERNEL_TRIGGER_T test) {
   return (test & current_tcb_p->triggers);
}


////////////////////////////////////////////////////////////////////////////////


void presto_trigger_send(KERNEL_TASKID_T tid, KERNEL_TRIGGER_T trigger) {
   KERNEL_TCB_T * tcb_p;
   tcb_p=tid_to_tcbptr(tid);
   MASKSET(tcb_p->triggers,trigger);
   // if the task is waiting on this trigger, then re-evaluate priorities
   if(tcb_p->wait_mask & trigger) {
      asm("swi");
   }
}


////////////////////////////////////////////////////////////////////////////////
//   K E R N E L - O N L Y   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


void kernel_trigger_set_noswitch(KERNEL_TASKID_T tid, KERNEL_TRIGGER_T trigger) {
   KERNEL_TCB_T * tcb_p;
   tcb_p=tid_to_tcbptr(tid);
   MASKSET(tcb_p->triggers,trigger);
}


////////////////////////////////////////////////////////////////////////////////


KERNEL_TASKID_T kernel_current_task(void) {
   return current_tcb_p->task_id;
}


////////////////////////////////////////////////////////////////////////////////


void kernel_context_switch(void) {
   asm("swi");
}


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


static void priority_queue_insert_tcb(KERNEL_TCB_T * new_tcb_p, BYTE new_priority) {

   // INSERT NEW TCB INTO LIST IN PRIORITY ORDER

   KERNEL_TCB_T ** p0;
   KERNEL_TCB_T * p1;
   CPU_LOCK_T lock;

   // messing with the task list... interrupts off
   cpu_lock_save(lock);

   // Start at the head, look at priorities.
   p0=&tcb_head_p;
   p1=tcb_head_p;
   while (1) {
      if ((p1==NULL)||((p1->current_priority)<new_priority)) {
         // Insert our item between p0 and p1
         new_tcb_p->next=p1;
         *p0=new_tcb_p;
         break; // out of the while (1) loop
      }
      p0=&(p1->next);
      p1=p1->next;
   }

   // done messing with the task list... interrupts back on
   cpu_unlock_restore(lock);
}


////////////////////////////////////////////////////////////////////////////////


static void priority_queue_remove_tcb(KERNEL_TCB_T * old_tcb_p) {
   KERNEL_TCB_T * ptr;
   CPU_LOCK_T lock;

   // messing with the task list... interrupts off
   cpu_lock_save(lock);

   // If the old TCB is at the head of the list, remove it.
   while (tcb_head_p==old_tcb_p) {
      tcb_head_p=tcb_head_p->next;
   }

   // First TCB in the list is not the old TCB.
   // Go through the list, looking for the old TCB.
   ptr=tcb_head_p;
   while (ptr!=NULL) {
      if (ptr->next==old_tcb_p) ptr->next=old_tcb_p->next;
      else ptr=ptr->next;
   }

   // done messing with the task list... interrupts back on
   cpu_unlock_restore(lock);
}


////////////////////////////////////////////////////////////////////////////////


static KERNEL_TCB_T * scheduler_next_ready(void) {
   // pick highest priority ready task to run
   KERNEL_TCB_T * ptr=tcb_head_p;
   while (ptr!=NULL) {
      // if the wait_mask equals WAIT_MASK_READY, we are "running"
      if (ptr->wait_mask==WAIT_MASK_READY) return ptr;
      // if we are waiting on triggers, see if they have fired
      if (ptr->wait_mask & ptr->triggers) return ptr;
      // next patient, please
      ptr=ptr->next;
   }
   // should never get here
   error_fatal(ERROR_KERNEL_SCHEDULERERROR);
   return NULL;
}


////////////////////////////////////////////////////////////////////////////////


static void context_switch_isr(void) {
   // registers are pushed when SWI is executed
   // pseudo-registers are also pushed (tmp,z,xy)

   #ifdef SANITYCHECK_KERNEL_CLOBBEREDSTACK
      // check to see if the old task has clobbered its stack
      if (((current_tcb_p->stack_ptr)>(current_tcb_p->stack_top))
      ||((current_tcb_p->stack_ptr)<(current_tcb_p->stack_bottom)))
         error_fatal(ERROR_KERNEL_CONTEXTSWITCH_STACKCLOBBERED);
   #endif // SANITYCHECK_KERNEL_CLOBBEREDSTACK

   // the inline asm will save old SP in old TCB
   old_task_stack_pointer_p=&(current_tcb_p->stack_ptr);

   // remember which task was running... we may not need to switch
   old_tcb_p=current_tcb_p;

   // pick next task to run
   current_tcb_p=scheduler_next_ready();

   // check to see if the same task won...
   // only manipulate stacks if there is a context SWITCH
   if (current_tcb_p!=old_tcb_p) {

      // there's a new "highest priority ready task"

      TOGGLE_SPEAKER();

      #ifdef SANITYCHECK_KERNEL_CLOBBEREDSTACK
         // check to see if the new task has clobbered its stack
         if (((current_tcb_p->stack_ptr)>(current_tcb_p->stack_top))
         ||((current_tcb_p->stack_ptr)<(current_tcb_p->stack_bottom)))
            error_fatal(ERROR_KERNEL_CONTEXTSWITCH_STACKCLOBBERED);
      #endif // SANITYCHECK_KERNEL_CLOBBEREDSTACK

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


static void idle_task(void) {
   while (1) {
      // Wait for an interrupt.  The WAI instruction places the CPU in
      // a low power "wait" mode.  Plus, it pre-stacks the registers for
      // a slightly faster response time.
      asm("wai");
   }
}


////////////////////////////////////////////////////////////////////////////////


static KERNEL_TCB_T * tid_to_tcbptr(KERNEL_TASKID_T tid) {
   if (tid>=MAX_TASKS) error_fatal(ERROR_KERNEL_TIDTOTCB_RANGE);
   return &tcb_list[tid];
}


////////////////////////////////////////////////////////////////////////////////


