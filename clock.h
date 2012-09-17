#ifndef _CLOCK_H_
#define _CLOCK_H_

////////////////////////////////////////////////////////////////////////////////

#include "types.h"

////////////////////////////////////////////////////////////////////////////////

#define SECS_PER_DAY  86400

////////////////////////////////////////////////////////////////////////////////

// note - clock rolls over after seven years
typedef struct {
   unsigned short usec;
   unsigned short msec;
   unsigned short sec;
   unsigned short hour;
} PRESTO_TIME_T;

////////////////////////////////////////////////////////////////////////////////

extern void clock_reset(PRESTO_TIME_T * clk);
extern void clock_add_us(PRESTO_TIME_T * clk, unsigned short us);
extern void clock_add_ms(PRESTO_TIME_T * clk, unsigned short ms);
extern void clock_add_sec(PRESTO_TIME_T * clk, unsigned short s);
extern signed char clock_compare(PRESTO_TIME_T * A,PRESTO_TIME_T * B);

////////////////////////////////////////////////////////////////////////////////

#endif
