////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////

// This module implements a simple timer system.

// Timers can be either one-shot or repeating.  They are specified by an
// initial delay and then a period.  Timers can be started or stopped.

// When a user wants to use a timer, he simply calls the start function,
// which requires a delay, a period and a trigger.  When the timer expires,
// the trigger is asserted on process which started it.  The user may then
// wait for that trigger.

// Since it is very common to simply delay for a little while, there is a
// presto_timer_wait function which combines the timer declaration (on the
// stack), the start with no repeat, and the wait.

// TODO - Be smart about when to issue a timer interrupt.  Calculate how
// long until the next timer expires, and then don't bother interrupting
// until then.  This problem is harder than it seems at first!

////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "presto.h"
#include "locks.h"
#include "hwtimer.h"
#include "kernel_magic.h"
#include "kernel/kernel.h"
#include "kernel/timer.h"
#include "kernel/clock.h"

#ifdef FEATURE_KERNEL_TIMER

////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//   D A T A   T Y P E S
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N   P R O T O T Y P E S
////////////////////////////////////////////////////////////////////////////////

static void timer_insert_into_master_list(KERNEL_TIMER_T * timer_p);
static void timer_remove_from_master_list(KERNEL_TIMER_T * timer_p);
void timer_isr(void) __attribute__((interrupt));


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   G L O B A L   D A T A
////////////////////////////////////////////////////////////////////////////////

static KERNEL_TIMER_T * timer_list=NULL;
static KERNEL_TIME_T system_clock;


////////////////////////////////////////////////////////////////////////////////
//   E X T E R N A L   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


void presto_timer_start(KERNEL_TIMER_T * timer_p, KERNEL_INTERVAL_T delay, KERNEL_INTERVAL_T period, KERNEL_TRIGGER_T trigger) {

   // set members of the timer structure
   timer_p->delivery_time=system_clock;
   clock_add_ms(&timer_p->delivery_time,delay);
   timer_p->timer_period=period;
   timer_p->owner_tid=kernel_current_task();
   timer_p->trigger=trigger;

   if (delay==0) {
      // This timer fires immediately.  Since we are already the
      // running task, there is no need to switch contexts.
      kernel_trigger_set_noswitch(kernel_current_task(), trigger);
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
   timer_remove_from_master_list(timer_p);
   // insert into the queue in "delivery time" order
   timer_insert_into_master_list(timer_p);
}


////////////////////////////////////////////////////////////////////////////////


void presto_timer_wait(KERNEL_INTERVAL_T delay, KERNEL_TRIGGER_T trigger) {
   KERNEL_TIMER_T timer;
   presto_timer_start(&timer, delay, 0, trigger);
   presto_wait(trigger);
}


////////////////////////////////////////////////////////////////////////////////


void presto_timer_stop(KERNEL_TIMER_T * timer_p) {
   timer_remove_from_master_list(timer_p);
}


////////////////////////////////////////////////////////////////////////////////
//   K E R N E L - O N L Y   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


void kernel_timer_init(void) {
}


////////////////////////////////////////////////////////////////////////////////


void kernel_master_clock_start(void) {
   // initialize master clock
   clock_reset(&system_clock);
   // do hardware timer magic
   hwtimer_start(PRESTO_KERNEL_MSPERTICK,(void (*)(void))timer_isr);
}


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


void timer_isr(void) {

   KERNEL_MAGIC_INDICATE_TICK_START();

   BYTE count=0;
   KERNEL_TIMER_T * timer_p;
   PRESTO_PRIORITY_T current_pri;

   // update the system clock
   clock_add_ms(&system_clock,PRESTO_KERNEL_MSPERTICK);

   // schedule a hardware timer interrupt one tick into the future
   hwtimer_restart();

   // find out the priority of the current running task
   current_pri=presto_priority_get(kernel_current_task());

   while ((timer_list!=NULL)&&(clock_compare(&timer_list->delivery_time,&system_clock)<=0)) {

      // save a pointer to the first timer in the list
      timer_p=timer_list;

      // remove timer from master list
      timer_list=timer_list->next;   // we know that timer_list!=NULL

      kernel_trigger_set_noswitch(timer_p->owner_tid, timer_p->trigger);

      if (timer_p->timer_period>0) {
         // This timer is a repeating timer.
         // Keep the structure, but update the delivery time.
         clock_add_ms(&timer_p->delivery_time,timer_p->timer_period);
         timer_insert_into_master_list(timer_p);
      }

      // indicate that a timer expired, and that it made a high priority task ready
      if(presto_priority_get(timer_p->owner_tid)>current_pri) count++;
   }

   KERNEL_MAGIC_INDICATE_TICK_END();

   if (count>0) {
      kernel_context_switch();
   }
}


////////////////////////////////////////////////////////////////////////////////


static void timer_insert_into_master_list(KERNEL_TIMER_T * timer_p) {
   CPU_LOCK_T lock;
   KERNEL_TIMER_T ** p0;
   KERNEL_TIMER_T * p1;

   cpu_lock_save(lock);
   // Start at the head, look at delivery times.
   p0=&timer_list;
   p1=timer_list;
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
   cpu_unlock_restore(lock);
}


////////////////////////////////////////////////////////////////////////////////


static void timer_remove_from_master_list(KERNEL_TIMER_T * timer_p) {
   KERNEL_TIMER_T * ptr;
   CPU_LOCK_T lock;

   cpu_lock_save(lock);
   // If the timer is at the head of the list, remove it.
   while (timer_list==timer_p) {
      timer_list=timer_list->next;
   }
   // First timer in the list is not the one.
   // Go through the list, looking for the one.
   ptr=timer_list;
   while (ptr!=NULL) {
      if (ptr->next==timer_p) ptr->next=timer_p->next;
      else ptr=ptr->next;
   }
   cpu_unlock_restore(lock);
}


////////////////////////////////////////////////////////////////////////////////

#endif  // FEATURE_KERNEL_TIMER


