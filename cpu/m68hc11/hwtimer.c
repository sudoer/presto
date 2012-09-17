////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////

// The HC11 has one constantly-running 16-bit timer, and 5 output comparitors.
// I elevate the priority of TOC2 (why did I choose TOC2 again? I forget) so it
// will register before other interrupts, and I set it up to tick every so many
// milliseconds (configurable in configure.h).  This file handles the conversion
// from ms to E clock ticks, and it sets up the registers to make the timer go.

////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "hc11_regs.h"
#include "vectors.h"
#include "hwtimer.h"

////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////

// These settings depend on your hardware platform

#define CYCLES_PER_MS       8000        // crystal frequency
#define CYCLES_PER_ECLOCK   4           // M68HC11 runs e-clock at f=xtal/4
#define TIMER_PRESCALE      16          // set in TMSK2 register (see boot.c)

// timing, computed from values in configure.h
#define ECLOCKS_PER_MS      CYCLES_PER_MS/CYCLES_PER_ECLOCK/TIMER_PRESCALE

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   D A T A
////////////////////////////////////////////////////////////////////////////////

static unsigned short eclocks_per_tick;

////////////////////////////////////////////////////////////////////////////////
//   E X P O R T E D   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void hwtimer_start(unsigned short ms, void (*func)(void)) {
   // set up interrupt vector for TOC2
   set_interrupt(INTR_TOC2, func);
   // calculate value to count up to
   eclocks_per_tick=ECLOCKS_PER_MS*ms;
   // counter disconnected from output pin logic
   MASKCLR(TCTL1,TCTL1_OM2|TCTL1_OL2);
   // store ("current time" plus eclocks_per_tick)
   TOC2 = (WORD)(TCNT + eclocks_per_tick);
   // clear the OUTPUT COMPARE trigger
   // writing O's makes no change, writing 1's clears the bit
   TFLG1 = TFLG1_OC2F;
   // request output compare interrupt
   MASKSET(TMSK1,TMSK1_OC2I);
}

////////////////////////////////////////////////////////////////////////////////

void hwtimer_restart(void) {
   // clear the OUTPUT COMPARE trigger
   // writing O's makes no change, writing 1's clears the bit
   TFLG1 = TFLG1_OC2F;
   // store ("last compare time" plus eclocks_per_tick)
   TOC2 = (WORD)(TOC2 + eclocks_per_tick);
}

////////////////////////////////////////////////////////////////////////////////

