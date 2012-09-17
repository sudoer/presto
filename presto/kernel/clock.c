
#include "types.h"
#include "kernel/clock.h"

////////////////////////////////////////////////////////////////////////////////

static void clock_add(KERNEL_TIME_T * clk, unsigned short sec, unsigned short msec) {
   clk->msec+=msec;
   while (clk->msec>=1000) {
      clk->msec-=1000;
      clk->sec++;
   }
   clk->sec+=sec;
   while (clk->sec>=3600) {
      clk->sec-=3600;
      clk->hour++;
   }
}

////////////////////////////////////////////////////////////////////////////////

void clock_reset(KERNEL_TIME_T * clk) {
   clk->msec=0;
   clk->sec=0;
   clk->hour=0;
}

////////////////////////////////////////////////////////////////////////////////

void clock_add_ms(KERNEL_TIME_T * clk, unsigned short ms) {
   clock_add(clk,0,ms);
}

////////////////////////////////////////////////////////////////////////////////

void clock_add_sec(KERNEL_TIME_T * clk, unsigned short s) {
   clock_add(clk,s,0);
}

////////////////////////////////////////////////////////////////////////////////

signed char clock_compare(KERNEL_TIME_T * A,KERNEL_TIME_T * B) {
   if (A->hour < B->hour) return -1;
   if (A->hour > B->hour) return 1;
   // we now know that A->hour == B->hour
   if (A->sec < B->sec) return -1;
   if (A->sec > B->sec) return 1;
   // we now know that A->sec == B->sec
   if (A->msec < B->msec) return -1;
   if (A->msec > B->msec) return 1;
   // we now know that A->msec == B->msec
   return 0;
}

////////////////////////////////////////////////////////////////////////////////


