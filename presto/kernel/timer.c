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
#include "kernel/kernel.h"
#include "kernel/timer.h"


////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////

// debug
#define STATIC //static


////////////////////////////////////////////////////////////////////////////////
//   D A T A   T Y P E S
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N   P R O T O T Y P E S
////////////////////////////////////////////////////////////////////////////////

STATIC void timer_InsertIntoMasterList(PRESTO_TIMER_T * timer_p);


////////////////////////////////////////////////////////////////////////////////
//   K E R N E L - O N L Y   D A T A
///////////////////////////////////////////////////////////////////////////////

PRESTO_TIMER_T * kernel_timer_list=NULL;


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   G L O B A L   D A T A
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//   K E R N E L - O N L Y   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

BYTE kernel_timer_poll(void) {
   BYTE count=0;
   PRESTO_TIMER_T * timer_p;
   WORD lock;

   while((kernel_timer_list!=NULL)&&(clock_compare(&kernel_timer_list->delivery_time,&kernel_clock)<=0)) {

      // remove timer from master list
      presto_lock_save(lock);
      timer_p=kernel_timer_list;                      // we know that kernel_timer_list!=NULL
      kernel_timer_list=kernel_timer_list->next;
      presto_unlock_restore(lock);

      kernel_flag_set(timer_p->owner_tcb_p, timer_p->trigger_flag);

      if(timer_p->timer_period>0) {
         // This timer is a repeating timer.
         // Keep the structure, but update the delivery time.
         clock_add_ms(&timer_p->delivery_time,timer_p->timer_period);
         timer_InsertIntoMasterList(timer_p);
      }

      // indicate that a timer expired
      count++;
   }
   return count;
}

////////////////////////////////////////////////////////////////////////////////
//   E X T E R N A L   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void presto_timer(PRESTO_TIMER_T * timer_p, KERNEL_INTERVAL_T delay, KERNEL_INTERVAL_T period, KERNEL_FLAG_T flag) {

   // set members of the timer structure
   timer_p->delivery_time=kernel_clock;
   clock_add_ms(&timer_p->delivery_time,delay);
   timer_p->timer_period=period;
   timer_p->owner_tcb_p=current_tcb_p;
   timer_p->trigger_flag=flag;

   if(delay==0) {
      // This timer fires immediately.
      kernel_flag_set(current_tcb_p, flag);
      if(period==0) {
         // This timer is a one-shot timer.
         return;
      } else {
         // This timer is a repeating timer.
         // Update the delivery time.  We have already fired the first shot.
         clock_add_ms(&timer_p->delivery_time,period);
      }
   }
   timer_InsertIntoMasterList(timer_p);

}

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

STATIC void timer_InsertIntoMasterList(PRESTO_TIMER_T * timer_p) {
   WORD lock;

   presto_lock_save(lock);
   if(kernel_timer_list==NULL) {
      // we are the first timer in the list
      kernel_timer_list=timer_p;
      timer_p->next=NULL;
   } else if(clock_compare(&kernel_timer_list->delivery_time,&timer_p->delivery_time)>0) {
      // advance to the head of the class!
      timer_p->next=kernel_timer_list;
      kernel_timer_list=timer_p;
   } else {
      // we are one of many timers in the list
      PRESTO_TIMER_T * temp_p=kernel_timer_list;
      PRESTO_TIMER_T * next_p;
      while(next_p=temp_p->next,next_p!=NULL) {
         if(clock_compare(&next_p->delivery_time,&timer_p->delivery_time)>0) break;
         temp_p=next_p;
      }
      // temp_p->next is either NULL or later delivery time than us
      // either way, we want to get inserted between temp_p and temp_p->next
      timer_p->next=temp_p->next;
      temp_p->next=timer_p;
   }
   presto_unlock_restore(lock);
}

////////////////////////////////////////////////////////////////////////////////























