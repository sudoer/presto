#ifndef _CLOCK_H_
#define _CLOCK_H_

////////////////////////////////////////////////////////////////////////////////

#include "types.h"

////////////////////////////////////////////////////////////////////////////////

#define SECS_PER_DAY  86400

////////////////////////////////////////////////////////////////////////////////

// note - clock rolls over after seven years
typedef struct KERNEL_TIME_S {
   unsigned short usec;
   unsigned short msec;
   unsigned short sec;
   unsigned short hour;
} KERNEL_TIME_T;

////////////////////////////////////////////////////////////////////////////////

extern void clock_reset(struct KERNEL_TIME_S * clk);
extern void clock_add_us(struct KERNEL_TIME_S * clk, unsigned short us);
extern void clock_add_ms(struct KERNEL_TIME_S * clk, unsigned short ms);
extern void clock_add_sec(struct KERNEL_TIME_S * clk, unsigned short s);
extern signed char clock_compare(struct KERNEL_TIME_S * A,struct KERNEL_TIME_S * B);

////////////////////////////////////////////////////////////////////////////////

#endif
