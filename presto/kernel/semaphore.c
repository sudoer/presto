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

static void insert_semuser_into_linked_list(KERNEL_SEMLOCK_T ** ptr_to_head, KERNEL_SEMLOCK_T * item);
static int remove_semuser_from_linked_list(KERNEL_SEMLOCK_T ** ptr_to_head, KERNEL_SEMLOCK_T * item);
#ifdef FEATURE_SEMAPHORE_PRIORITYINHERITANCE
static void promote_runners_to_top_waiter_priority(KERNEL_SEMRESOURCE_T * resource_p);
#endif // FEATURE_SEMAPHORE_PRIORITYINHERITANCE

////////////////////////////////////////////////////////////////////////////////
//   E X T E R N A L   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


void presto_semaphore_init(PRESTO_SEMRESOURCE_T * resource_p, short resources) {
   resource_p->num_locks=0;
   resource_p->highest_priority=0;
   resource_p->max_resources=resources;
   resource_p->available_resources=resources;
   resource_p->user_list=NULL;
   resource_p->wait_list=NULL;
   resource_p->inheritance_type=PRESTO_SEMAPHORE_PRIORITY_NORMAL;
}


////////////////////////////////////////////////////////////////////////////////


void presto_semaphore_register(PRESTO_SEMRESOURCE_T * resource_p, PRESTO_SEMLOCK_T * semlock_p) {
   // one more user
   resource_p->num_locks++;
   // fill in semuser fields
   semlock_p->resource_p=resource_p;
   semlock_p->tid=kernel_current_task();
   semlock_p->natural_priority=kernel_priority_get(semlock_p->tid);  // NATURAL priority
   // record highest priority client
   if(semlock_p->natural_priority > resource_p->highest_priority) {
      resource_p->highest_priority=semlock_p->natural_priority;
   }
}


////////////////////////////////////////////////////////////////////////////////


void presto_semaphore_protocol(PRESTO_SEMRESOURCE_T * resource_p, PRESTO_SEMPROTOCOL_T inheritance_type) {
   resource_p->inheritance_type=inheritance_type;
}


////////////////////////////////////////////////////////////////////////////////


BOOLEAN presto_semaphore_request(PRESTO_SEMLOCK_T * semlock_p, KERNEL_TRIGGER_T trigger) {
   KERNEL_SEMRESOURCE_T * resource_p;
   BOOLEAN gotit;
   CPU_LOCK_T lock;

   // fill in information about the waiting task
   semlock_p->trigger=trigger;
   // semlock_p->resource_p is already filled in
   // semlock_p->tid is already filled in
   // semlock_p->natural_priority is already filled in
   // semlock_p->next will be filled in by insert_semuser_into_linked_list()

   resource_p=semlock_p->resource_p;

   cpu_lock_save(lock);
   gotit=(resource_p->available_resources>0)?TRUE:FALSE;
   if (gotit) {
      KERNEL_SEMLOCK_T * old_runner_p;

      // There is a resource available.  Use it.
      resource_p->available_resources--;
      kernel_trigger_set_noswitch(semlock_p->tid, trigger);  // This is us.

      // Record who was using the resource before we asked for it.
      old_runner_p=resource_p->user_list;

      // Insert the task in the user queue.
      insert_semuser_into_linked_list(&(resource_p->user_list), semlock_p);

      #ifdef FEATURE_SEMAPHORE_PRIORITYINHERITANCE
         switch(resource_p->inheritance_type) {
            case PRESTO_SEMAPHORE_PRIORITY_NORMAL:
               break;

            case PRESTO_SEMAPHORE_PRIORITY_INHERITANCE:
               // TODO - ????
               // If there was a change because of us...
               if ((old_runner_p!=NULL)&&(old_runner_p!=resource_p->user_list)) {
                  // restore the old runner to his old priority
                  presto_priority_restore(old_runner_p->tid);
               }
               break;

            case PRESTO_SEMAPHORE_PRIORITY_CEILING:
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
      insert_semuser_into_linked_list(&(resource_p->wait_list), semlock_p);

   }

   #ifdef FEATURE_SEMAPHORE_PRIORITYINHERITANCE
      // TODO - let's revisit this
      switch(resource_p->inheritance_type) {
         case PRESTO_SEMAPHORE_PRIORITY_NORMAL:
            break;
         case PRESTO_SEMAPHORE_PRIORITY_INHERITANCE:
            // If we're using priority inheritance,
            // bump up the first runner to the first waiter's priority.
            promote_runners_to_top_waiter_priority(resource_p);
            break;
         case PRESTO_SEMAPHORE_PRIORITY_CEILING:
            break;

      }
   #endif // FEATURE_SEMAPHORE_PRIORITYINHERITANCE

   cpu_unlock_restore(lock);
   return gotit;
}


////////////////////////////////////////////////////////////////////////////////


void presto_semaphore_wait(PRESTO_SEMLOCK_T * semlock_p) {
   presto_semaphore_request(semlock_p, KERNEL_INTERNAL_TRIGGER);
   presto_wait(KERNEL_INTERNAL_TRIGGER);
}


////////////////////////////////////////////////////////////////////////////////


void presto_semaphore_release(PRESTO_SEMLOCK_T * semlock_p) {
   KERNEL_SEMRESOURCE_T * resource_p;
   PRESTO_SEMLOCK_T * first_in_line;
   CPU_LOCK_T lock;

   resource_p=semlock_p->resource_p;

   cpu_lock_save(lock);

   // Remove ourselves from the running list.
   if (!remove_semuser_from_linked_list(&(resource_p->user_list), semlock_p)) {
      // oops, we were not a runner!
      // TODO - decide whether this is an error or not
      error_fatal(ERROR_SEMAPHORE_BADUNLOCK);
      //cpu_unlock_restore(lock);
      //return;
   }

   #ifdef FEATURE_SEMAPHORE_PRIORITYINHERITANCE
      switch(resource_p->inheritance_type) {
         case PRESTO_SEMAPHORE_PRIORITY_NORMAL:
            break;
         case PRESTO_SEMAPHORE_PRIORITY_INHERITANCE:
            // restore our original current_priority
            presto_priority_restore(semlock_p->tid);
            break;
         case PRESTO_SEMAPHORE_PRIORITY_CEILING:
            break;
      }
   #endif // FEATURE_SEMAPHORE_PRIORITYINHERITANCE

   // See if anyone is waiting for our resource.
   first_in_line=resource_p->wait_list;
   if (first_in_line==NULL) {
      // No one else is waiting -- return the resource.
      resource_p->available_resources++;
   } else {
      // Someone is waiting -- give the resource to them.
      kernel_trigger_set_noswitch(first_in_line->tid, first_in_line->trigger);
      // Remove them from wait list.
      remove_semuser_from_linked_list(&(resource_p->wait_list), first_in_line);
      // Add them to the user list.
      insert_semuser_into_linked_list(&(resource_p->user_list), first_in_line);
      #ifdef FEATURE_SEMAPHORE_PRIORITYINHERITANCE
         // If we're using priority inheritance,
         // bump up the first runner to the first waiter's priority.
         switch(resource_p->inheritance_type) {
            case PRESTO_SEMAPHORE_PRIORITY_NORMAL:
               break;
            case PRESTO_SEMAPHORE_PRIORITY_INHERITANCE:
               // TODO - What should we do if there are more than one runner?
               promote_runners_to_top_waiter_priority(resource_p);
               break;
            case PRESTO_SEMAPHORE_PRIORITY_CEILING:
               break;
         }
      #endif // FEATURE_SEMAPHORE_PRIORITYINHERITANCE
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


static void insert_semuser_into_linked_list(KERNEL_SEMLOCK_T ** ptr_to_head, KERNEL_SEMLOCK_T * item) {
   KERNEL_PRIORITY_T current_priority;
   KERNEL_SEMLOCK_T ** p0;
   KERNEL_SEMLOCK_T * p1;

   // Determine priority of semuser, for position in list
   current_priority=presto_priority_get(item->tid);

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


static int remove_semuser_from_linked_list(KERNEL_SEMLOCK_T ** ptr_to_head, KERNEL_SEMLOCK_T * item) {
   KERNEL_SEMLOCK_T ** p0;
   KERNEL_SEMLOCK_T * p1;
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
static void promote_runners_to_top_waiter_priority(KERNEL_SEMRESOURCE_T * resource_p) {
   // Bump up the priority of the first TCB in the user list.
   KERNEL_SEMLOCK_T * first_waiter_p;
   KERNEL_SEMLOCK_T * semlock_p;
   KERNEL_PRIORITY_T waiter_priority;

   // find the first waiter, determine his priority
   first_waiter_p=resource_p->wait_list;
   if (first_waiter_p==NULL) return;
   waiter_priority=presto_priority_get(first_waiter_p->tid);

   // go through the user list, bump up priorities of users
   for (semlock_p=resource_p->user_list;semlock_p!=NULL;semlock_p=semlock_p->next) {
      if (presto_priority_get(semlock_p->tid)<waiter_priority) {
         presto_priority_override(semlock_p->tid, waiter_priority);
      }
   }
}
#endif // FEATURE_SEMAPHORE_PRIORITYINHERITANCE


////////////////////////////////////////////////////////////////////////////////

#endif // FEATURE_KERNEL_SEMAPHORE

