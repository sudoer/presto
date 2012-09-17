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
#include "error.h"
#include "cpu_locks.h"
#include "configure.h"
#include "kernel/kernel.h"
#include "kernel/semaphore.h"

#ifdef FEATURE_KERNEL_SEMAPHORE

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N   P R O T O T Y P E S
////////////////////////////////////////////////////////////////////////////////

static void insert_semuser_into_linked_list(KERNEL_SEMUSER_T ** ptr_to_head, KERNEL_SEMUSER_T * item);
static int remove_semuser_from_linked_list(KERNEL_SEMUSER_T ** ptr_to_head, KERNEL_SEMUSER_T * item);
#ifdef FEATURE_SEMAPHORE_PRIORITYINHERITANCE
static void promote_runners_to_top_waiter_priority(KERNEL_SEMAPHORE_T * sem_p);
#endif // FEATURE_SEMAPHORE_PRIORITYINHERITANCE

////////////////////////////////////////////////////////////////////////////////
//   E X T E R N A L   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


void presto_semaphore_init(PRESTO_SEMAPHORE_T * sem_p, int resources, int num_users, PRESTO_SEMUSER_T * semuser_p) {
   int count;
   sem_p->num_users=num_users;
/*
   sem_p->highest_priority=0;
*/
   sem_p->max_resources=resources;
   sem_p->available_resources=resources;
   sem_p->inheritance_type=PRESTO_SEMAPHORE_NORMAL;
   sem_p->user_list=NULL;
   sem_p->wait_list=NULL;
   sem_p->free_list=&(semuser_p[0]);
   for (count=0;count<num_users;count++) {
      semuser_p[count].next=&(semuser_p[count+1]);
   }
   semuser_p[num_users-1].next=NULL;
}


////////////////////////////////////////////////////////////////////////////////


void presto_semaphore_protocol(PRESTO_SEMAPHORE_T * sem_p, PRESTO_SEMPROTOCOL_T inheritance_type) {
   sem_p->inheritance_type=inheritance_type;
}


////////////////////////////////////////////////////////////////////////////////


BOOLEAN presto_semaphore_request(PRESTO_SEMAPHORE_T * sem_p, KERNEL_TRIGGER_T trigger) {
   PRESTO_SEMUSER_T * semuser_p;
   BOOLEAN gotit;
   CPU_LOCK_T lock;

   // get the first available semlock structure
   cpu_lock_save(lock);
   semuser_p=sem_p->free_list;
   if (semuser_p==NULL) {
      error_fatal(ERROR_SEMAPHORE_TOOMANYWAITERS);
   }

   // sem_p->free_list is not NULL
   sem_p->free_list=sem_p->free_list->next;
   cpu_unlock_restore(lock);

   // fill in information about the waiting task
   semuser_p->task_id=kernel_current_task();
   semuser_p->natural_priority=kernel_priority_get(semuser_p->task_id);
   semuser_p->trigger=trigger;

   cpu_lock_save(lock);
   gotit=(sem_p->available_resources>0)?TRUE:FALSE;
   if (gotit) {
      KERNEL_SEMUSER_T * old_runner_p;

      // There is a resource available.  Use it.
      sem_p->available_resources--;
      kernel_trigger_set_noswitch(semuser_p->task_id, trigger);  // This is us.

      // Record who was using the resource before we asked for it.
      old_runner_p=sem_p->user_list;

      // Insert the task in the user queue.
      insert_semuser_into_linked_list(&(sem_p->user_list), semuser_p);

      #ifdef FEATURE_SEMAPHORE_PRIORITYINHERITANCE
         switch(sem_p->inheritance_type) {
            case PRESTO_SEMAPHORE_NORMAL:
               break;

            case PRESTO_SEMAPHORE_PRIORITYINHERITANCE:
               // TODO - ????
               // If there was a change because of us...
               if ((old_runner_p!=NULL)&&(old_runner_p!=sem_p->user_list)) {
                  // restore the old runner to his old priority
                  presto_priority_restore(old_runner_p->task_id);
               }
               break;

            case PRESTO_SEMAPHORE_PRIORITYCEILING:
               // A NOTE ON THE PRIORITY CEILING PROTOCOL
               // We would bump up the priority of the running task to the highest
               // level that uses this semaphore.  But that requires each task to
               // pre-register so we will know what the ceilings are.  Priority
               // ceilings prevent deadlocks and make Rate Monotonic Analysis possible.
               // If we are careful, the application can avoid deadlocks by making sure
               // that it locks resources in a certain order.

               break;

         }
      #endif // FEATURE_SEMAPHORE_PRIORITYINHERITANCE

   } else {

      // There is no resource available.
      // We have to wait.

      // Insert our task in the semaphore's wait queue.
      insert_semuser_into_linked_list(&(sem_p->wait_list), semuser_p);

   }

   #ifdef FEATURE_SEMAPHORE_PRIORITYINHERITANCE
      // TODO - let's revisit this
      switch(sem_p->inheritance_type) {
         case PRESTO_SEMAPHORE_NORMAL:
            break;
         case PRESTO_SEMAPHORE_PRIORITYINHERITANCE:
            // If we're using priority inheritance,
            // bump up the first runner to the first waiter's priority.
            promote_runners_to_top_waiter_priority(sem_p);
            break;
         case PRESTO_SEMAPHORE_PRIORITYCEILING:
            break;

      }
   #endif // FEATURE_SEMAPHORE_PRIORITYINHERITANCE

   cpu_unlock_restore(lock);
   return gotit;
}


////////////////////////////////////////////////////////////////////////////////


void presto_semaphore_wait(KERNEL_SEMAPHORE_T * sem_p) {
   presto_semaphore_request(sem_p, KERNEL_INTERNAL_TRIGGER);
   presto_wait(KERNEL_INTERNAL_TRIGGER);
}


////////////////////////////////////////////////////////////////////////////////


void presto_semaphore_release(KERNEL_SEMAPHORE_T * sem_p) {
   PRESTO_SEMUSER_T * semuser_p;
   CPU_LOCK_T lock;

   cpu_lock_save(lock);

   // Remove our semuser structure from the running list.
   semuser_p=sem_p->user_list;
   if (!remove_semuser_from_linked_list(&(sem_p->user_list), semuser_p)) {
      // oops, we were not a runner!
      // TODO - decide whether this is an error or not
      error_fatal(ERROR_SEMAPHORE_BADUNLOCK);
      //cpu_unlock_restore(lock);
      //return;
   }
   // Add our semuser structure to the free list.
   semuser_p->next=sem_p->free_list;
   sem_p->free_list=semuser_p;

   #ifdef FEATURE_SEMAPHORE_PRIORITYINHERITANCE
      switch(sem_p->inheritance_type) {
         case PRESTO_SEMAPHORE_NORMAL:
            break;
         case PRESTO_SEMAPHORE_PRIORITYINHERITANCE:
            // restore our original current_priority
            presto_priority_restore(semuser_p->task_id);
            break;
         case PRESTO_SEMAPHORE_PRIORITYCEILING:
            break;
      }
   #endif // FEATURE_SEMAPHORE_PRIORITYINHERITANCE

   // See if anyone is waiting for our resource.
   semuser_p=sem_p->wait_list;
   if (semuser_p==NULL) {
      // No one else is waiting -- return the resource.
      sem_p->available_resources++;
   } else {
      // Someone is waiting -- give the resource to them.
      kernel_trigger_set_noswitch(semuser_p->task_id, semuser_p->trigger);
      // Remove them from wait list.
      remove_semuser_from_linked_list(&(sem_p->wait_list), semuser_p);
      // Add them to the user list.
      insert_semuser_into_linked_list(&(sem_p->user_list), semuser_p);
      #ifdef FEATURE_SEMAPHORE_PRIORITYINHERITANCE
         // If we're using priority inheritance,
         // bump up the first runner to the first waiter's priority.
         switch(sem_p->inheritance_type) {
            case PRESTO_SEMAPHORE_NORMAL:
               break;
            case PRESTO_SEMAPHORE_PRIORITYINHERITANCE:
               // TODO - What should we do if there are more than one runner?
               promote_runners_to_top_waiter_priority(sem_p);
               break;
            case PRESTO_SEMAPHORE_PRIORITYCEILING:
               break;
         }
      #endif // FEATURE_SEMAPHORE_PRIORITYINHERITANCE
   }

   cpu_unlock_restore(lock);

   if (semuser_p!=NULL) {
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


static void insert_semuser_into_linked_list(KERNEL_SEMUSER_T ** ptr_to_head, KERNEL_SEMUSER_T * item) {
   KERNEL_PRIORITY_T current_priority;
   KERNEL_SEMUSER_T ** p0;
   KERNEL_SEMUSER_T * p1;

   // Determine priority of semuser, for position in list
   current_priority=presto_priority_get(item->task_id);

   // Start at the head, look at priorities.
   p0=ptr_to_head;
   p1=*ptr_to_head;
   while (1) {
      if ((p1==NULL)||(p1->natural_priority < current_priority)) {
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


static int remove_semuser_from_linked_list(KERNEL_SEMUSER_T ** ptr_to_head, KERNEL_SEMUSER_T * item) {
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
   KERNEL_SEMUSER_T * semuser_p;
   KERNEL_PRIORITY_T waiter_priority;

   // find the first waiter, determine his priority
   first_waiter_p=sem_p->wait_list;
   if (first_waiter_p==NULL) return;
   waiter_priority=presto_priority_get(first_waiter_p->task_id);

   // go through the user list, bump up priorities of users
   for (semuser_p=sem_p->user_list;semuser_p!=NULL;semuser_p=semuser_p->next) {
      if (presto_priority_get(semuser_p->task_id) < waiter_priority) {
         presto_priority_override(semuser_p->task_id, waiter_priority);
      }
   }
}
#endif // FEATURE_SEMAPHORE_PRIORITYINHERITANCE


////////////////////////////////////////////////////////////////////////////////

#endif // FEATURE_KERNEL_SEMAPHORE

