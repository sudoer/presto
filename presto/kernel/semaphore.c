////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "presto.h"
#include "error.h"
#include "chip/locks.h"
#include "config.h"
#include "kernel/kernel_types.h"
#include "kernel/kernel_funcs.h"
#include "kernel/kernel_data.h"
#include "kernel/sem_types.h"
#include "kernel/sem_funcs.h"


////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//   D A T A   T Y P E S
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N   P R O T O T Y P E S
////////////////////////////////////////////////////////////////////////////////

static void insert_simuser_into_linked_list(KERNEL_SEMUSER_T ** ptr_to_head, KERNEL_SEMUSER_T * item);


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   G L O B A L   D A T A
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//   E X T E R N A L   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


void presto_semaphore_init(PRESTO_SEMAPHORE_T * sem_p, short resources, BOOLEAN use_inheritance) {
   int x;
   sem_p->max_resources=resources;
   sem_p->available_resources=resources;
   sem_p->use_inheritance=use_inheritance;
   sem_p->user_list=NULL;
   sem_p->wait_list=NULL;
   sem_p->free_list=sem_p->semuser_data;
   for(x=0;x<MAX_SEM_USERS;x++) {
      sem_p->semuser_data[x].tcb_p=NULL;
      sem_p->semuser_data[x].trigger=NULL;
      sem_p->semuser_data[x].next=&(sem_p->semuser_data[x+1]);
   }
   sem_p->semuser_data[MAX_SEM_USERS-1].next=NULL;
}


////////////////////////////////////////////////////////////////////////////////


void presto_semaphore_request(PRESTO_SEMAPHORE_T * sem_p, KERNEL_TRIGGER_T trigger) {
   KERNEL_SEMUSER_T * our_seminfo;
   KERNEL_LOCK_T lock;

   presto_lock_save(lock);
   // get the first available semuser structure
   our_seminfo=sem_p->free_list;
   if(our_seminfo==NULL) {
      presto_fatal_error(ERROR_KERNEL_SEMWAIT_TOOMANYUSERS);
   }
   // sem_p->free_list is not NULL
   sem_p->free_list=sem_p->free_list->next;
   presto_unlock_restore(lock);

   // fill in information about the waiting task
   our_seminfo->tcb_p=kernel_current_tcb_p;
   our_seminfo->trigger=trigger;

   // PRIORITY CEILING PROTOCOL
   // We would bump up the priority of the running task to the highest
   // level that uses this semaphore.  But that requires each task to
   // pre-register so we will know what the ceilings are.  Priority
   // ceilings prevent deadlocks and make Rate Monotonic Analysis possible.
   // If we are careful, the application can avoid deadlocks by making sure
   // that it locks resources in a certain order.

   presto_lock_save(lock);
   if(sem_p->available_resources>0) {
      // There is a resource available.  Use it.
      sem_p->available_resources--;
      kernel_trigger_set(kernel_current_tcb_p, trigger);
      // Insert the task in the user queue.
      // TODO - more than one user.
      insert_simuser_into_linked_list(&(sem_p->user_list), our_seminfo);
   } else {
      // There is no resource available.
      // We have to wait.

      if(sem_p->use_inheritance) {
         // Bump up the priority of the TCB's in the user list.
         // TODO - more than one user.

         if((sem_p->user_list->tcb_p->current_priority)<(kernel_current_tcb_p->current_priority)) {
            kernel_priority_override(sem_p->user_list->tcb_p, kernel_current_tcb_p->current_priority);
         }
      }

      // Insert the task in the wait queue.
      insert_simuser_into_linked_list(&(sem_p->wait_list), our_seminfo);
   }
   presto_unlock_restore(lock);
}


////////////////////////////////////////////////////////////////////////////////


void presto_semaphore_wait(PRESTO_SEMAPHORE_T * sem_p, KERNEL_TRIGGER_T trigger) {
   presto_semaphore_request(sem_p, trigger);
   presto_wait(trigger);
}


////////////////////////////////////////////////////////////////////////////////


void presto_semaphore_release(PRESTO_SEMAPHORE_T * sem_p) {
   KERNEL_LOCK_T lock;
   KERNEL_SEMUSER_T * first_in_line;

   presto_lock_save(lock);

   if(sem_p->use_inheritance) {
      // restore our original current_priority
      // TODO - more than one WAITER.
      kernel_priority_restore(kernel_current_tcb_p);
   }

   first_in_line=sem_p->wait_list;
   if(first_in_line==NULL) {
      // no one else is waiting
      // return the resource
      sem_p->available_resources++;
   } else {
      // someone is waiting
      // give the resource to them
      sem_p->wait_list=first_in_line->next;
      kernel_trigger_set(first_in_line->tcb_p, first_in_line->trigger);
   }

   presto_unlock_restore(lock);

   if(first_in_line!=NULL) {
      asm("swi");
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
   KERNEL_SEMUSER_T * temp;

   // Determine priority of simuser, for position in list
   priority=item->tcb_p->natural_priority;

   // Start at the head, look at priorities.
   p0=ptr_to_head;
   p1=*ptr_to_head;
   while(1) {
      if((p1==NULL)||((p1->tcb_p->natural_priority)<priority)) {
         // Insert our item between p0 and p1
         item->next=p1;
         *p0=item;
         break; // out of the while(1) loop
      }
      p0=&(p1->next);
      p1=p1->next;
   }
}


////////////////////////////////////////////////////////////////////////////////



