////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////

// This module implements a simple semaphore mechanism.

// These semaphores are used when one resource is shared among many users.
// The semaphore should be initialized at the start of execution.  Then, when
// a user wants to use the resource, he does a presto_semaphore_wait() on that
// semaphore.  His task will be blocked until the resource becomes available.

// If there are more than one resource (example: a pool of three printers),
// then the semaphore can be initialized as a "counting" semaphore (more than
// one resource) rather than a "binary" semaphore (one resource).

// One common problem with resource locking is "priority inversions."  To
// avoid this problem, this module supports priority inheritance, a practice
// where a running task is promoted to the priority of the highest waiter.

// A second problem with resource locking is "deadlock".  The only way to
// avoid deadlock is to pay special attention to the order in which multiple
// resources are reserved (do not let one task reserve A and then B, while
// a second task reserves B and then A).

// Currently, there is an arbitrary maximum to the number of tasks that
// can request a semaphore at the same time.  This maximum is specified
// by the constant PRESTO_SEM_WAITLIST in configure.h.  This constant
// tries to find some middle ground between the need to make the semaphore
// data stucture small in size, and the need to support "popular" resources.
// Ideally, we would use a more dynamic scheme.  In practice, this constant
// works just fine.  If we want to be safe at the expense of memory, we
// can set PRESTO_SEM_WAITLIST to PRESTO_KERNEL_MAXUSERTASKS.

////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "presto.h"
#include "cpu/error.h"
#include "cpu/locks.h"
#include "configure.h"
#include "kernel/kernel.h"
#include "kernel/semaphore.h"


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N   P R O T O T Y P E S
////////////////////////////////////////////////////////////////////////////////

static void insert_simuser_into_linked_list(KERNEL_SEMUSER_T ** ptr_to_head, KERNEL_SEMUSER_T * item);
static int remove_simuser_from_linked_list(KERNEL_SEMUSER_T ** ptr_to_head, KERNEL_SEMUSER_T * item);
#ifdef FEATURE_SEMAPHORE_PRIORITYINHERITANCE
static void promote_runners_to_top_waiter_priority(KERNEL_SEMAPHORE_T * sem_p);
#endif

////////////////////////////////////////////////////////////////////////////////
//   E X T E R N A L   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


#ifdef FEATURE_SEMAPHORE_PRIORITYINHERITANCE
void presto_semaphore_init(KERNEL_SEMAPHORE_T * sem_p, short resources, BOOLEAN use_inheritance) {
#else
void presto_semaphore_init(KERNEL_SEMAPHORE_T * sem_p, short resources) {
#endif
   int x;
   sem_p->max_resources=resources;
   sem_p->available_resources=resources;
   sem_p->user_list=NULL;
   sem_p->wait_list=NULL;
   sem_p->free_list=sem_p->semuser_data;
   for (x=0;x<PRESTO_SEM_WAITLIST;x++) {
      sem_p->semuser_data[x].tid=KERNEL_TASKID_NONE;
      sem_p->semuser_data[x].trigger=NULL;
      sem_p->semuser_data[x].next=&(sem_p->semuser_data[x+1]);
   }
   sem_p->semuser_data[PRESTO_SEM_WAITLIST-1].next=NULL;
   #ifdef FEATURE_SEMAPHORE_PRIORITYINHERITANCE
      sem_p->use_inheritance=use_inheritance;
   #endif
}


////////////////////////////////////////////////////////////////////////////////


BOOLEAN presto_semaphore_request(KERNEL_SEMAPHORE_T * sem_p, KERNEL_TRIGGER_T trigger) {
   BOOLEAN gotit;
   KERNEL_SEMUSER_T * our_seminfo;
   CPU_LOCK_T lock;

   cpu_lock_save(lock);

   // get the first available semuser structure
   our_seminfo=sem_p->free_list;
   if (our_seminfo==NULL) {
      error_fatal(ERROR_SEMAPHORE_TOOMANYWAITERS);
   }

   // sem_p->free_list is not NULL
   sem_p->free_list=sem_p->free_list->next;
   cpu_unlock_restore(lock);

   // fill in information about the waiting task
   our_seminfo->tid=kernel_current_task();
   our_seminfo->trigger=trigger;

   // A NOTE ON THE PRIORITY CEILING PROTOCOL
   // We would bump up the priority of the running task to the highest
   // level that uses this semaphore.  But that requires each task to
   // pre-register so we will know what the ceilings are.  Priority
   // ceilings prevent deadlocks and make Rate Monotonic Analysis possible.
   // If we are careful, the application can avoid deadlocks by making sure
   // that it locks resources in a certain order.

   cpu_lock_save(lock);
   gotit=(sem_p->available_resources>0)?TRUE:FALSE;
   if (gotit) {
      KERNEL_SEMUSER_T * old_runner_p;

      // There is a resource available.  Use it.
      sem_p->available_resources--;
      kernel_trigger_set_noswitch(kernel_current_task(), trigger);

      // Record who was using the resource before we asked for it.
      old_runner_p=sem_p->user_list;

      // Insert the task in the user queue.
      insert_simuser_into_linked_list(&(sem_p->user_list), our_seminfo);

      #ifdef FEATURE_SEMAPHORE_PRIORITYINHERITANCE
         // If we're using priority inheritance..
         if (sem_p->use_inheritance) {
            // If there was a change because of us...
            if ((old_runner_p!=NULL)&&(old_runner_p!=sem_p->user_list)) {
               // restore the old runner to his old priority
               presto_priority_restore(old_runner_p->tid);
            }
         }
      #endif

   } else {

      // There is no resource available.
      // We have to wait.

      // Insert the task in the wait queue.
      insert_simuser_into_linked_list(&(sem_p->wait_list), our_seminfo);

   }

   #ifdef FEATURE_SEMAPHORE_PRIORITYINHERITANCE
      // If we're using priority inheritance,
      // bump up the first runner to the first waiter's priority.
      if (sem_p->use_inheritance) {
         promote_runners_to_top_waiter_priority(sem_p);
      }
   #endif

   cpu_unlock_restore(lock);
   return gotit;
}


////////////////////////////////////////////////////////////////////////////////


void presto_semaphore_wait(KERNEL_SEMAPHORE_T * sem_p, KERNEL_TRIGGER_T trigger) {
   presto_semaphore_request(sem_p, trigger);
   presto_wait(trigger);
}


////////////////////////////////////////////////////////////////////////////////


void presto_semaphore_release(KERNEL_SEMAPHORE_T * sem_p) {
   CPU_LOCK_T lock;
   KERNEL_SEMUSER_T * first_in_line;

   cpu_lock_save(lock);

   // Remove ourselves from the running list.
   first_in_line=sem_p->user_list;
   if (!remove_simuser_from_linked_list(&(sem_p->user_list), first_in_line)) {
      // oops, we were not a runner!
      cpu_unlock_restore(lock);
      return;
   }

   #ifdef FEATURE_SEMAPHORE_PRIORITYINHERITANCE
      if (sem_p->use_inheritance) {
         // restore our original current_priority
         presto_priority_restore(kernel_current_task());
      }
   #endif

   // See if anyone is waiting for our resource.
   first_in_line=sem_p->wait_list;
   if (first_in_line==NULL) {
      // No one else is waiting -- return the resource.
      sem_p->available_resources++;
   } else {
      // Someone is waiting -- give the resource to them.
      kernel_trigger_set_noswitch(first_in_line->tid, first_in_line->trigger);
      // Remove them from wait list.
      remove_simuser_from_linked_list(&(sem_p->wait_list), first_in_line);
      // Add them to the user list.
      insert_simuser_into_linked_list(&(sem_p->user_list), first_in_line);
      #ifdef FEATURE_SEMAPHORE_PRIORITYINHERITANCE
         // If we're using priority inheritance,
         // bump up the first runner to the first waiter's priority.
         if (sem_p->use_inheritance) {
            // TODO - What should we do if there are more than one runner?
            promote_runners_to_top_waiter_priority(sem_p);
         }
      #endif
   }

   cpu_unlock_restore(lock);

   if (first_in_line!=NULL) {
      kernel_context_switch();
   }

}


////////////////////////////////////////////////////////////////////////////////
//   K E R N E L - O N L Y   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void kernel_semaphore_init(void) {

}

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


static void insert_simuser_into_linked_list(KERNEL_SEMUSER_T ** ptr_to_head, KERNEL_SEMUSER_T * item) {
   KERNEL_PRIORITY_T priority;
   KERNEL_SEMUSER_T ** p0;
   KERNEL_SEMUSER_T * p1;

   // Determine priority of simuser, for position in list
   priority=presto_priority_get(item->tid);

   // Start at the head, look at priorities.
   p0=ptr_to_head;
   p1=*ptr_to_head;
   while (1) {
      if ((p1==NULL)||(p1->natural_priority<priority)) {
         // Insert our item between p0 and p1
         item->next=p1;
         *p0=item;
         break; // out of the while (1) loop
      }
      p0=&(p1->next);
      p1=p1->next;
   }
}


////////////////////////////////////////////////////////////////////////////////


static int remove_simuser_from_linked_list(KERNEL_SEMUSER_T ** ptr_to_head, KERNEL_SEMUSER_T * item) {
   KERNEL_SEMUSER_T ** p0;
   KERNEL_SEMUSER_T * p1;
   int count=0;
   p0=ptr_to_head;
   while (p1=*p0,p1!=NULL) {
      if (p1==item) {
         *p0=p1->next;
         count++;
      } else {
         p0=&(p1->next);
      }
   }
   return count;
}


////////////////////////////////////////////////////////////////////////////////


#ifdef FEATURE_SEMAPHORE_PRIORITYINHERITANCE
static void promote_runners_to_top_waiter_priority(KERNEL_SEMAPHORE_T * sem_p) {
   // Bump up the priority of the first TCB in the user list.
   KERNEL_SEMUSER_T * first_waiter_p;
   KERNEL_SEMUSER_T * user_p;
   KERNEL_PRIORITY_T waiter_priority;

   // find the first waiter, determine his priority
   first_waiter_p=sem_p->wait_list;
   if (first_waiter_p==NULL) return;
   waiter_priority=presto_priority_get(first_waiter_p->tid);

   // go through the user list, bump up priorities of users
   for (user_p=sem_p->user_list;user_p!=NULL;user_p=user_p->next) {
      if (presto_priority_get(user_p->tid)<waiter_priority) {
         presto_priority_override(user_p->tid, waiter_priority);
      }
   }
}
#endif


////////////////////////////////////////////////////////////////////////////////



