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

// NOTE - User tasks are numbered from 0 to (PRESTO_KERNEL_MAXUSERTASKS-1),
// giving a total of PRESTO_KERNEL_MAXUSERTASKS user tasks.  This makes it
// convenient to keep an array of items, one for each user task (see the
// default_mailbox array in mail.c).  The idle task's task ID number equals
// PRESTO_KERNEL_MAXUSERTASKS.

////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "presto.h"
#include "configure.h"
#include "error.h"
#include "cpu_locks.h"
#include "cpu_magic.h"
#include "cpu_debug.h"
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
#define WAIT_MASK_READY     ((KERNEL_TRIGGER_T)0)
#define STATIC

#ifdef FEATURE_STACKSHARING
#error "DO NOT USE STACK SHARING YET!"
#endif // FEATURE_STACKSHARING

////////////////////////////////////////////////////////////////////////////////
//   D A T A   T Y P E S
////////////////////////////////////////////////////////////////////////////////

typedef struct KERNEL_TCB_S {
   KERNEL_TASKID_T task_id;
#ifdef FEATURE_STACKSHARING
   void (*main)(void);
#endif // FEATURE_STACKSHARING
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
STATIC KERNEL_TCB_T * scheduler_next_ready(void);
STATIC void send_idle_notifications(void);
STATIC void idle_task(void);

// priority queue
STATIC void priority_queue_insert_tcb(KERNEL_TCB_T * tcb_p, KERNEL_PRIORITY_T priority);
STATIC void priority_queue_remove_tcb(KERNEL_TCB_T * tcb_p);

// context switching
CPU_MAGIC_DECLARE_SWI(context_switch_isr);

// utilities
STATIC KERNEL_TCB_T * tid_to_tcbptr(KERNEL_TASKID_T tid, unsigned char c);

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   G L O B A L   D A T A
////////////////////////////////////////////////////////////////////////////////

// task control blocks
STATIC KERNEL_TCB_T tcb_list[MAX_TASKS];
STATIC KERNEL_TCB_T * current_tcb_p=NULL;
STATIC KERNEL_TCB_T * tcb_head_p=NULL;
STATIC KERNEL_TCB_T * free_tcb_p=NULL;

// idle task stuff
#ifdef FEATURE_STACKSHARING
extern BYTE initial_stack[BOOT_INITIALSTACKSIZE];
#else
STATIC BYTE idle_stack[PRESTO_KERNEL_IDLESTACKSIZE];
#endif // FEATURE_STACKSHARING
STATIC KERNEL_TCB_T * idle_tcb_p;
STATIC KERNEL_TASKID_T idle_tid;   // always equals PRESTO_KERNEL_MAXUSERTASKS
STATIC BYTE idle_notification[TASK_BITMASK_SIZE];

// miscellaneous
STATIC enum {
   PROGRESS_STARTUP,
   PROGRESS_INITIALIZED,
   PROGRESS_STARTED,
   PROGRESS_IDLE_REACHED,
} presto_runtime_progress=PROGRESS_STARTUP;

// These are used to pass arguments to inline assembly routines.
// Do not put these on the stack (BOOM).
STATIC KERNEL_TCB_T * swi_old_tcb_p;
STATIC BYTE * swi_new_sp;
STATIC BYTE ** swi_old_spp;

////////////////////////////////////////////////////////////////////////////////
//   I N I T I A L I Z A T I O N
////////////////////////////////////////////////////////////////////////////////


void presto_init(void) {
   BYTE count;

   if (presto_runtime_progress!=PROGRESS_STARTUP) {
      error_fatal(ERROR_KERNEL_BADINIT);
   }

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
   #ifdef FEATURE_KERNEL_TIMER
      kernel_timer_init();
   #endif // FEATURE_KERNEL_TIMER
   #ifdef FEATURE_KERNEL_MAIL
      kernel_mail_init();
   #endif // FEATURE_KERNEL_MAIL
   #ifdef FEATURE_KERNEL_SEMAPHORE
      kernel_semaphore_init();
   #endif // FEATURE_KERNEL_SEMAPHORE
   #ifdef FEATURE_KERNEL_MEMORY
      kernel_memory_init();
   #endif // FEATURE_KERNEL_MEMORY

   for (count=0;count<TASK_BITMASK_SIZE;count++) {
      idle_notification[count]=0;
   }

   // must be done before creating idle task
   presto_runtime_progress=PROGRESS_INITIALIZED;

   // initialize idle task
#ifdef FEATURE_STACKSHARING
   idle_tid=presto_task_create(idle_task,initial_stack,BOOT_INITIALSTACKSIZE,IDLE_PRIORITY);
#else
   idle_tid=presto_task_create(idle_task,idle_stack,PRESTO_KERNEL_IDLESTACKSIZE,IDLE_PRIORITY);
#endif // FEATURE_STACKSHARING
   idle_tcb_p=tid_to_tcbptr(idle_tid,8);
}


////////////////////////////////////////////////////////////////////////////////


KERNEL_TASKID_T presto_task_create(void (*func)(void), BYTE * stack, short stack_size, KERNEL_PRIORITY_T priority) {

   KERNEL_TCB_T * new_tcb_p;
   CPU_LOCK_T lock;

   if (presto_runtime_progress<PROGRESS_INITIALIZED) {
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


#ifdef FEATURE_STACKSHARING
   new_tcb_p->main=func;
#endif // FEATURE_STACKSHARING
   new_tcb_p->stack_top=stack+stack_size-1;
   new_tcb_p->stack_bottom=stack;
   new_tcb_p->current_priority=priority;
   new_tcb_p->natural_priority=priority;
   new_tcb_p->wait_mask=WAIT_MASK_READY;
   new_tcb_p->triggers=(KERNEL_TRIGGER_T)0;

   // SET UP NEW STACK
   new_tcb_p->stack_ptr=CPU_MAGIC_SETUP_STACK(new_tcb_p->stack_top,func);

   priority_queue_insert_tcb(new_tcb_p,priority);

   return new_tcb_p->task_id;
}


///////////////////////////////////////////////////////////////////////////////


void presto_scheduler_start(void) {

   if (presto_runtime_progress!=PROGRESS_INITIALIZED) {
      error_fatal(ERROR_KERNEL_BADSTART);
   }
   presto_runtime_progress=PROGRESS_STARTED;

   // pick next task to run
   // first task in list is highest priority and is ready
   current_tcb_p=tcb_head_p;
   if (current_tcb_p==NULL) {
      error_fatal(ERROR_KERNEL_NOTASKTOSTART);
   }

   // we're about to switch to our first task... interrupts off
   cpu_lock();

#ifdef FEATURE_STACKSHARING
   KERNEL_TCB_T * ptr=tcb_head_p;
   if (ptr!=NULL) {
      // set up task stacks, skip last one (idle task)
      do {
         ptr->stack_ptr=CPU_MAGIC_SETUP_STACK(ptr->stack_top,ptr->main);
         ptr=ptr->next;
      } while (ptr!=NULL);
   }
#endif // FEATURE_STACKSHARING

   CPU_MAGIC_INITIALIZE_SOFTWARE_INTERRUPT(context_switch_isr);

   #ifdef FEATURE_KERNEL_TIMER
      kernel_master_clock_start();
   #endif // FEATURE_KERNEL_TIMER

   // SET UP A NEW STACK AND START EXECUTION USING IT
   CPU_MAGIC_LOAD_STACK_PTR(current_tcb_p->stack_ptr);
#ifdef FEATURE_STACKSHARING
   // set up idle task stack
   idle_tcb_p->stack_ptr=CPU_MAGIC_SETUP_STACK(idle_tcb_p->stack_top,idle_tcb_p->main);
#endif // FEATURE_STACKSHARING
   CPU_MAGIC_RUN_FIRST_TASK();

   // we never get here
   error_fatal(ERROR_KERNEL_STARTAFTERRTI);
}


////////////////////////////////////////////////////////////////////////////////
//   P R I O R I T I E S
////////////////////////////////////////////////////////////////////////////////


KERNEL_PRIORITY_T presto_priority_get(KERNEL_TASKID_T tid) {
   KERNEL_TCB_T * tcb_p;
   tcb_p=tid_to_tcbptr(tid,1);
   return tcb_p->current_priority;
}


////////////////////////////////////////////////////////////////////////////////


void presto_priority_set(PRESTO_TASKID_T tid, PRESTO_PRIORITY_T new_priority) {
   KERNEL_TCB_T * tcb_p;
   tcb_p=tid_to_tcbptr(tid,2);
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
   tcb_p=tid_to_tcbptr(tid,3);
   if (tcb_p->current_priority!=new_priority) {
      priority_queue_remove_tcb(tcb_p);
      tcb_p->current_priority=new_priority;
      priority_queue_insert_tcb(tcb_p, new_priority);
   }
}


////////////////////////////////////////////////////////////////////////////////


void presto_priority_restore(KERNEL_TASKID_T tid) {
   KERNEL_TCB_T * tcb_p;
   tcb_p=tid_to_tcbptr(tid,4);
   if (tcb_p->current_priority!=tcb_p->natural_priority) {
      priority_queue_remove_tcb(tcb_p);
      tcb_p->current_priority=tcb_p->natural_priority;
      priority_queue_insert_tcb(tcb_p, tcb_p->natural_priority);
   }
}


////////////////////////////////////////////////////////////////////////////////
//   T R I G G E R S
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
      kernel_context_switch();
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
   // No need to do kernel_context_switch() here.
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
   tcb_p=tid_to_tcbptr(tid,5);
   MASKSET(tcb_p->triggers,trigger);
   // if the task is waiting on this trigger, then re-evaluate priorities
   if (tcb_p->wait_mask & trigger) {
      kernel_context_switch();
   }
}


////////////////////////////////////////////////////////////////////////////////
//   K E R N E L - O N L Y   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


void kernel_trigger_set_noswitch(KERNEL_TASKID_T tid, KERNEL_TRIGGER_T trigger) {
   KERNEL_TCB_T * tcb_p;
   tcb_p=tid_to_tcbptr(tid,6);
   MASKSET(tcb_p->triggers,trigger);
}


////////////////////////////////////////////////////////////////////////////////


KERNEL_TASKID_T kernel_current_task(void) {
   return current_tcb_p->task_id;
}


////////////////////////////////////////////////////////////////////////////////


KERNEL_PRIORITY_T kernel_priority_get(KERNEL_TASKID_T tid) {
   KERNEL_TCB_T * tcb_p;
   tcb_p=tid_to_tcbptr(tid,7);
   return tcb_p->natural_priority;
}


////////////////////////////////////////////////////////////////////////////////


void kernel_context_switch(void) {
   CPU_MAGIC_SOFTWARE_INTERRUPT();
}


////////////////////////////////////////////////////////////////////////////////
//   P R I O R I T Y   Q U E U E
////////////////////////////////////////////////////////////////////////////////


STATIC void priority_queue_insert_tcb(KERNEL_TCB_T * new_tcb_p, BYTE new_priority) {

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


STATIC void priority_queue_remove_tcb(KERNEL_TCB_T * old_tcb_p) {
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
//   S C H E D U L E R
////////////////////////////////////////////////////////////////////////////////


STATIC KERNEL_TCB_T * scheduler_next_ready(void) {
   // pick highest priority ready task to run
   KERNEL_TCB_T * ptr=tcb_head_p;
   while (ptr!=NULL) {
      // if the wait_mask equals WAIT_MASK_READY, we are "running"
      if (ptr->wait_mask==WAIT_MASK_READY) return ptr;
      // if we are waiting on triggers and they have fired, we are "ready"
      if (ptr->wait_mask & ptr->triggers) return ptr;
      // otherwise, we are "waiting"... next patient, please!
      ptr=ptr->next;
   }
   // should never get here
   error_fatal(ERROR_KERNEL_SCHEDULERERROR);
   return NULL;
}


////////////////////////////////////////////////////////////////////////////////
//   C O N T E X T   S W I T C H
////////////////////////////////////////////////////////////////////////////////


void context_switch_isr(void) {

   CPU_MAGIC_START_OF_SWI();
   CPU_DEBUG_SWI_START();

   #ifdef SANITY_KERNEL_CLOBBEREDSTACK
      // check to see if the old task has clobbered its stack
      if (((current_tcb_p->stack_ptr)>(current_tcb_p->stack_top))
      ||((current_tcb_p->stack_ptr)<(current_tcb_p->stack_bottom)))
         error_fatal(ERROR_KERNEL_CONTEXTSWITCH_STACKCLOBBERED);
   #endif // SANITY_KERNEL_CLOBBEREDSTACK

   // the inline asm will save old SP in old TCB
   swi_old_spp=&(current_tcb_p->stack_ptr);

   // remember which task was running... we may not need to switch
   swi_old_tcb_p=current_tcb_p;

   // pick next task to run
   current_tcb_p=scheduler_next_ready();

   // check to see if the same task won...
   // only manipulate stacks if there is a context SWITCH
   if (current_tcb_p!=swi_old_tcb_p) {

      CPU_DEBUG_TASK_SWITCH();

      // there's a new "highest priority ready task"

      #ifdef SANITY_KERNEL_CLOBBEREDSTACK
         // check to see if the new task has clobbered its stack
         if (((current_tcb_p->stack_ptr)>(current_tcb_p->stack_top))
         ||((current_tcb_p->stack_ptr)<(current_tcb_p->stack_bottom)))
            error_fatal(ERROR_KERNEL_CONTEXTSWITCH_STACKCLOBBERED);
      #endif // SANITY_KERNEL_CLOBBEREDSTACK

      // call asm routine to set up new stack
      // when we return, we'll be another process
      // the asm routine will re-enable interrupts
      swi_new_sp=current_tcb_p->stack_ptr;

      CPU_MAGIC_SWAP_STACK_POINTERS(swi_old_spp,swi_new_sp);

   }

   CPU_DEBUG_SWI_END();

   CPU_MAGIC_END_OF_SWI();
}


////////////////////////////////////////////////////////////////////////////////
//   I D L E   N O T I F I C A T I O N
////////////////////////////////////////////////////////////////////////////////


void presto_wait_for_idle(void) {
   KERNEL_TASKID_T tid=kernel_current_task();
   BYTE index=TASK_BITMASK_INDEX(tid);
   BYTE mask=TASK_BITMASK_MASK(tid);
   if (presto_runtime_progress>=PROGRESS_IDLE_REACHED) return;
   MASKSET(idle_notification[index],mask);
   presto_wait(KERNEL_INTERNAL_TRIGGER);
}


////////////////////////////////////////////////////////////////////////////////


STATIC void send_idle_notifications(void) {
   KERNEL_TASKID_T t;
   BYTE index=TASK_BITMASK_FIRSTINDEX();
   BYTE mask=TASK_BITMASK_FIRSTMASK();
   for(t=0;t<PRESTO_KERNEL_MAXUSERTASKS;t++) {
      if (idle_notification[index]&mask) {
         kernel_trigger_set_noswitch(t,KERNEL_INTERNAL_TRIGGER);
      }
      TASK_BITMASK_INCREMENT(index,mask);
   }
   presto_runtime_progress=PROGRESS_IDLE_REACHED;
}


////////////////////////////////////////////////////////////////////////////////
//   I D L E   T A S K
////////////////////////////////////////////////////////////////////////////////


STATIC void idle_task(void) {
   send_idle_notifications();
   kernel_context_switch();
   while (1) {
      CPU_MAGIC_IDLE_WORK();
   }
}


////////////////////////////////////////////////////////////////////////////////
//   M I S C E L L A N E O U S
////////////////////////////////////////////////////////////////////////////////


STATIC KERNEL_TCB_T * tid_to_tcbptr(KERNEL_TASKID_T tid, unsigned char c) {
   if (tid>=MAX_TASKS) error_fatal(0xA0+c); // ERROR_KERNEL_TIDTOTCB_RANGE);
   return &tcb_list[tid];
}


////////////////////////////////////////////////////////////////////////////////


