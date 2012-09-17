////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////

// The HC11 has one constantly-running 16-bit timer, and 5 output comparitors.
// I elevate the priority of TOC2 (why did I choose TOC2 again? I forget) do it
// will register before other interrupts, and I set it up to tick every so many
// milliseconds (configurable in configure.h).  This file handles the conversion
// from ms to E clock ticks, and it sets up the registers to make the timer go.

////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "cpu/hc11regs.h"
#include "cpu/hwtimer.h"

////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////

// These settings depend on your hardware platform

#define CYCLES_PER_MS       8000        // crystal frequency
#define CYCLES_PER_ECLOCK   4           // M68HC11 runs e-clock at f=xtal/4
#define TIMER_PRESCALE      16          // set in TMSK2 register (see boot.c)

// timing, computed from values in configure.h
#define ECLOCKS_PER_MS       CYCLES_PER_MS/CYCLES_PER_ECLOCK/TIMER_PRESCALE

////////////////////////////////////////////////////////////////////////////////
//   E X P O R T E D   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void hwtimer_start(unsigned short ms) {
   // counter disconnected from output pin logic
   TCTL1 &= ~(TCTL1_OM2|TCTL1_OL2);
   // clear the OUTPUT COMPARE trigger
   // writing O's makes no change, writing 1's clears the bit
   TFLG1 = TFLG1_OC2F;
   // store ("current time" plus ECLOCKS_PER_TICK)
   TOC2 = (WORD)(TCNT + (ECLOCKS_PER_MS*ms));
   // request output compare interrupt
   TMSK1 |= TMSK1_OC2I;
}

////////////////////////////////////////////////////////////////////////////////

void hwtimer_restart(unsigned short ms) {
   // store ("last compare time" plus ECLOCKS_PER_TICK)
   TOC2 = (WORD)(TOC2 + (ECLOCKS_PER_MS*ms));
   // clear the OUTPUT COMPARE trigger
   // writing O's makes no change, writing 1's clears the bit
   TFLG1 = TFLG1_OC2F;
}

////////////////////////////////////////////////////////////////////////////////

