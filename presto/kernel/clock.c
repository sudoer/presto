////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////

// These functions define a simple clock interface.  I need a way to keep
// track of some very long running numbers.  Rather than use a "long long",
// I just use a structure with some easily-understood components.  There's
// just a few functions, mainly add and compare.

// Eventually, I would like to add some sort of subtract function, or maybe
// try to use a compiler-supported data type (like long long).  But when I
// use even 32-bit longs, the O/S crashes.  I'm not sure if the long math
// functions are re-entrant?

// These will do for now.

////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "kernel/clock.h"

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N S
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
//   E X P O R T E D   F U N C T I O N S
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


