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


////////////////////////////////////////////////////////////////////////////////
//   E X P O R T E D   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void clock_reset(KERNEL_TIME_T * clk) {
   BYTE i;
   for (i=0;i<CLOCK_PARTS;i++) {
      clk->t[i]=0;
   }
}

////////////////////////////////////////////////////////////////////////////////

void clock_add_ms(KERNEL_TIME_T * clk, unsigned short ms) {
   unsigned short was;
   was=clk->t[0];
   clk->t[0]+=ms;
   if ((clk->t[0]<was)||(clk->t[0]<ms)) {
      BYTE i=1;
      clk->t[i]++;
      while(clk->t[i]==0) {
         i++;
         if(i==CLOCK_PARTS) return;
         clk->t[i]++;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

signed char clock_compare(KERNEL_TIME_T * A,KERNEL_TIME_T * B) {
   BYTE i;
   i=CLOCK_PARTS;
   CLOCK_PART_T * ap=&(A->t[i]);
   CLOCK_PART_T * bp=&(B->t[i]);
   while(i>0) {
      i--;
      ap--;
      bp--;
      if (*ap < *bp) return -1;
      if (*ap > *bp) return 1;
   }
   return 0;
}

////////////////////////////////////////////////////////////////////////////////


