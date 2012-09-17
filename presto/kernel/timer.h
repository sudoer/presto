#ifndef _TIMER_H_
#define _TIMER_H_

////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "kernel/clock.h"
#include "kernel/kernel.h"

////////////////////////////////////////////////////////////////////////////////

typedef struct KERNEL_TIMER_S {
   KERNEL_TIME_T delivery_time;
   KERNEL_INTERVAL_T timer_period;
   KERNEL_TCB_T * owner_tcb_p;
   KERNEL_TRIGGER_T trigger;
   struct KERNEL_TIMER_S * next;
} KERNEL_TIMER_T;

////////////////////////////////////////////////////////////////////////////////

extern void kernel_timer_init(void);

////////////////////////////////////////////////////////////////////////////////

#endif



