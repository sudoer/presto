#ifndef _TIMER_H_
#define _TIMER_H_

////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "kernel/clock.h"
#include "kernel/kernel.h"

////////////////////////////////////////////////////////////////////////////////

typedef unsigned short KERNEL_INTERVAL_T;

typedef struct KERNEL_TIMER_S {
   KERNEL_TIME_T delivery_time;
   KERNEL_INTERVAL_T timer_period;
   KERNEL_TASKID_T owner_tid;
   KERNEL_TRIGGER_T trigger;
   struct KERNEL_TIMER_S * next;
} KERNEL_TIMER_T;

////////////////////////////////////////////////////////////////////////////////

extern void kernel_timer_init(void);
extern void kernel_master_clock_start(void);

////////////////////////////////////////////////////////////////////////////////

#endif // _TIMER_H_



