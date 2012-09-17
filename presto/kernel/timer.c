////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////


// TODO - Be smart about when to issue a timer interrupt.  Calculate how
// long until the next timer expires, and then don't bother interrupting
// until then.  This problem is harder than it seems at first!



////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "presto.h"
#include "cpu/error.h"
#include "cpu/locks.h"
#include "cpu/misc_hw.h"
#include "cpu/intvect.h"
#include "cpu/hwtimer.h"
#include "kernel/kernel.h"
#include "kernel/timer.h"
#include "kernel/clock.h"


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
static void timer_isr(void) __attribute__((interrupt));


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   G L O B A L   D A T A
////////////////////////////////////////////////////////////////////////////////

static KERNEL_TIMER_T * kernel_timer_list=NULL;
static KERNEL_TIME_T kernel_clock;


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


void presto_timer_now(KERNEL_TIME_T * clk) {
   *clk=kernel_clock;
}


////////////////////////////////////////////////////////////////////////////////
//   K E R N E L - O N L Y   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


void kernel_timer_init(void) {
   // initialize master clock
   clock_reset(&kernel_clock);
   // set up interrupt vector for TOC2
   set_interrupt(INTR_TOC2, timer_isr);
   // set up timer/counter 2 to tick every so often
   hwtimer_start(PRESTO_KERNEL_MSPERTICK);
}


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


static void timer_isr(void) {
   BYTE count=0;
   KERNEL_TIMER_T * timer_p;
   CPU_LOCK_T lock;

   // update the system clock
   clock_add_ms(&kernel_clock,PRESTO_KERNEL_MSPERTICK);

   // schedule a hardware timer interrupt one tick into the future
   hwtimer_restart(PRESTO_KERNEL_MSPERTICK);

   cpu_lock_save(lock);
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

   cpu_unlock_restore(lock);

   if (count>0) {
      asm("swi");
   }
}


////////////////////////////////////////////////////////////////////////////////


static void timer_InsertIntoMasterList(KERNEL_TIMER_T * timer_p) {
   CPU_LOCK_T lock;
   KERNEL_TIMER_T ** p0;
   KERNEL_TIMER_T * p1;

   cpu_lock_save(lock);
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
   cpu_unlock_restore(lock);
}


////////////////////////////////////////////////////////////////////////////////


static void timer_RemoveFromMasterList(KERNEL_TIMER_T * timer_p) {
   KERNEL_TIMER_T * ptr;
   CPU_LOCK_T lock;

   cpu_lock_save(lock);
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
   cpu_unlock_restore(lock);
}


////////////////////////////////////////////////////////////////////////////////




