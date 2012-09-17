
#include "types.h"
#include "kernel/clock.h"

////////////////////////////////////////////////////////////////////////////////

#define STATIC //static

////////////////////////////////////////////////////////////////////////////////

STATIC clock_add(struct KERNEL_TIME_S * clk, unsigned short sec, unsigned short msec, unsigned short usec) {
   clk->usec+=usec;
   while(clk->usec>=1000) {
      clk->usec-=1000;
      clk->msec++;
   }
   clk->msec+=msec;
   while(clk->msec>=1000) {
      clk->msec-=1000;
      clk->sec++;
   }
   clk->sec+=sec;
   while(clk->sec>=3600) {
      clk->sec-=3600;
      clk->hour++;
   }
}

////////////////////////////////////////////////////////////////////////////////

void clock_reset(struct KERNEL_TIME_S * clk) {
   clk->usec=0;
   clk->msec=0;
   clk->sec=0;
   clk->hour=0;
}

////////////////////////////////////////////////////////////////////////////////

void clock_add_us(struct KERNEL_TIME_S * clk, unsigned short us) {
   clock_add(clk,0,0,us);
}

////////////////////////////////////////////////////////////////////////////////

void clock_add_ms(struct KERNEL_TIME_S * clk, unsigned short ms) {
   clock_add(clk,0,ms,0);
}

////////////////////////////////////////////////////////////////////////////////

void clock_add_sec(struct KERNEL_TIME_S * clk, unsigned short s) {
   clock_add(clk,s,0,0);
}

////////////////////////////////////////////////////////////////////////////////

signed char clock_compare(struct KERNEL_TIME_S * A,struct KERNEL_TIME_S * B) {
   if(A->hour < B->hour) return -1;
   if(A->hour > B->hour) return 1;
   // we now know that A->hour == B->hour
   if(A->sec < B->sec) return -1;
   if(A->sec > B->sec) return 1;
   // we now know that A->sec == B->sec
   if(A->msec < B->msec) return -1;
   if(A->msec > B->msec) return 1;
   // we now know that A->msec == B->msec
   if(A->usec < B->usec) return -1;
   if(A->usec > B->usec) return 1;
   return 0;
}

////////////////////////////////////////////////////////////////////////////////


