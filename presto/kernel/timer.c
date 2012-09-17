////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "presto.h"
#include "error_codes.h"
#include "cpu/locks.h"
#include "cpu/misc_hw.h"
#include "kernel/kernel.h"
#include "kernel/timer.h"


////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//   D A T A   T Y P E S
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N   P R O T O T Y P E S
////////////////////////////////////////////////////////////////////////////////

static void timer_InsertIntoMasterList(KERNEL_TIMER_T * timer_p);
static void timer_RemoveFromMasterList(KERNEL_TIMER_T * timer_p);


////////////////////////////////////////////////////////////////////////////////
//   K E R N E L - O N L Y   D A T A
///////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   G L O B A L   D A T A
////////////////////////////////////////////////////////////////////////////////

KERNEL_TIMER_T * kernel_timer_list=NULL;


////////////////////////////////////////////////////////////////////////////////
//   E X T E R N A L   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


void presto_timer_start(KERNEL_TIMER_T * timer_p, KERNEL_INTERVAL_T delay, KERNEL_INTERVAL_T period, KERNEL_TRIGGER_T trigger) {

   // set members of the timer structure
   timer_p->delivery_time=kernel_clock;
   clock_add_ms(&timer_p->delivery_time,delay);
   timer_p->timer_period=period;
   timer_p->owner_tcb_p=kernel_current_tcb_p;
   timer_p->trigger=trigger;

   if (delay==0) {
      // This timer fires immediately.
      kernel_trigger_set(kernel_current_tcb_p, trigger);
      if (period==0) {
         // This timer is a one-shot timer.
         // We have already fired once.
         // Do not insert into the list.
         return;
      } else {
         // This timer is a repeating timer.
         // We have already fired the first shot.
         // Update the delivery time for the next shot.
         clock_add_ms(&timer_p->delivery_time,period);
      }
   }
   // make sure we don't have double-entries
   timer_RemoveFromMasterList(timer_p);
   // insert into the queue in "delivery time" order
   timer_InsertIntoMasterList(timer_p);
}


////////////////////////////////////////////////////////////////////////////////


void presto_timer_wait(KERNEL_INTERVAL_T delay, KERNEL_TRIGGER_T trigger) {
   KERNEL_TIMER_T timer;
   presto_timer_start(&timer, delay, 0, trigger);
   presto_wait(trigger);
}


////////////////////////////////////////////////////////////////////////////////


void presto_timer_stop(KERNEL_TIMER_T * timer_p) {
   timer_RemoveFromMasterList(timer_p);
}


////////////////////////////////////////////////////////////////////////////////
//   K E R N E L - O N L Y   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


void kernel_timer_init(void) {
}


////////////////////////////////////////////////////////////////////////////////


BYTE kernel_timer_tick(void) {
   BYTE count=0;
   KERNEL_TIMER_T * timer_p;
   KERNEL_LOCK_T lock;

   presto_lock_save(lock);
   while ((kernel_timer_list!=NULL)&&(clock_compare(&kernel_timer_list->delivery_time,&kernel_clock)<=0)) {

      // save a pointer to the first timer in the list
      timer_p=kernel_timer_list;

      // remove timer from master list
      kernel_timer_list=kernel_timer_list->next;   // we know that kernel_timer_list!=NULL

      kernel_trigger_set(timer_p->owner_tcb_p, timer_p->trigger);

      if (timer_p->timer_period>0) {
         // This timer is a repeating timer.
         // Keep the structure, but update the delivery time.
         clock_add_ms(&timer_p->delivery_time,timer_p->timer_period);
         timer_InsertIntoMasterList(timer_p);
      }

      // indicate that a timer expired
      count++;
   }

   presto_unlock_restore(lock);
   return count;
}


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


static void timer_InsertIntoMasterList(KERNEL_TIMER_T * timer_p) {
   KERNEL_LOCK_T lock;
   KERNEL_TIMER_T ** p0;
   KERNEL_TIMER_T * p1;

   presto_lock_save(lock);
   // Start at the head, look at delivery times.
   p0=&kernel_timer_list;
   p1=kernel_timer_list;
   while (1) {
      if ((p1==NULL)||(clock_compare(&p1->delivery_time,&timer_p->delivery_time)>0)) {
         // Insert our item between p0 and p1
         timer_p->next=p1;
         *p0=timer_p;
         break; // out of the while (1) loop
      }
      p0=&(p1->next);
      p1=p1->next;
   }
   presto_unlock_restore(lock);
}


////////////////////////////////////////////////////////////////////////////////


static void timer_RemoveFromMasterList(KERNEL_TIMER_T * timer_p) {
   KERNEL_TIMER_T * ptr;
   KERNEL_LOCK_T lock;

   presto_lock_save(lock);
   // If the timer is at the head of the list, remove it.
   while (kernel_timer_list==timer_p) {
      kernel_timer_list=kernel_timer_list->next;
   }
   // First timer in the list is not the one.
   // Go through the list, looking for the one.
   ptr=kernel_timer_list;
   while (ptr!=NULL) {
      if (ptr->next==timer_p) ptr->next=timer_p->next;
      else ptr=ptr->next;
   }
   presto_unlock_restore(lock);
}


////////////////////////////////////////////////////////////////////////////////























