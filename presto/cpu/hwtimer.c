////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "configure.h"
#include "cpu/hc11regs.h"
#include "cpu/hwtimer.h"


////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////

// These settings depend on your hardware platform

#define CYCLES_PER_MS       8000        // crystal frequency
#define CYCLES_PER_ECLOCK   4           // M68HC11 runs e-clock at f=xtal/4
#define TIMER_PRESCALE      16          // set in TMSK2 register

// timing, computed from values in configure.h
#define ECLOCKS_PER_MS       CYCLES_PER_MS/CYCLES_PER_ECLOCK/TIMER_PRESCALE
#define ECLOCKS_PER_TICK     ECLOCKS_PER_MS*PRESTO_KERNEL_MSPERTICK


////////////////////////////////////////////////////////////////////////////////
//   E X P O R T E D   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void hwtimer_Start(void) {
   // store (current plus ECLOCKS_PER_TICK)
   TOC2 = (WORD)(TCNT + ECLOCKS_PER_TICK);
   // request output compare interrupt
   TMSK1 |= TMSK1_OC2I;
   // clear the OUTPUT COMPARE trigger
   // writing O's makes no change, writing 1's clears the bit
   TFLG1 = TFLG1_OC2F;
   // counter disconnected from output pin logic
   TCTL1 &= ~(TCTL1_OM2|TCTL1_OL2);
}

////////////////////////////////////////////////////////////////////////////////

void hwtimer_Restart(void) {
   // store (last plus ECLOCKS_PER_TICK)
   TOC2 = (WORD)(TOC2 + ECLOCKS_PER_TICK);
   // clear the OUTPUT COMPARE trigger
   // writing O's makes no change, writing 1's clears the bit
   TFLG1 = TFLG1_OC2F;
}

////////////////////////////////////////////////////////////////////////////////

