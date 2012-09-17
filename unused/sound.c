
#include "types.h"
#include "hc11regs.h"
#include "intvect.h"
#include "services/sound.h"

////////////////////////////////////////////////////////////////////////////////

// FUNCTION PROTOTYPES

void sound_isr(void) __attribute__((interrupt));

////////////////////////////////////////////////////////////////////////////////

// LOCAL VARIABLES

static unsigned int beep_tone;

////////////////////////////////////////////////////////////////////////////////

void sound_init(void) {
   set_interrupt(INTR_TOC5, sound_isr);
}

////////////////////////////////////////////////////////////////////////////////

void sound_tick(void) {
   BITNOT(PORTA,3);
}

////////////////////////////////////////////////////////////////////////////////

void sound_on(unsigned short tone) {
   if(tone!=beep_tone) {
      beep_tone=tone;

      // store (current plus beep_tone)
      TOC5 = TCNT + beep_tone;

      // clear the OUTPUT COMPARE flag
      // writing O's makes no change, writing 1's clears the bit
      TFLG1 = TFLG1_OC5F;

      // successful comparison toggles OC5 output line
      MASKSET(TCTL1,TCTL1_OL5);
      MASKCLR(TCTL1,TCTL1_OM5);

      // enable beeper interrupt
      MASKSET(TMSK1,TMSK1_OC5I);
   }
}

////////////////////////////////////////////////////////////////////////////////

void sound_off(void) {
   // disable beeper interrupt
   MASKCLR(TMSK1,TMSK1_OC1I);
   // Turn off line toggling
   MASKCLR(TCTL1,(TCTL1_OM5|TCTL1_OL5));
   beep_tone = 0;
}

////////////////////////////////////////////////////////////////////////////////

void sound_isr(void) {
   // reset timer to go off again in a little bit
   TOC5 = TOC5 + beep_tone;
   if(TCNT>TOC5) TOC5 = TCNT + beep_tone;
   // clear the OUTPUT COMPARE flag
   // writing O's makes no change, writing 1's clears the bit
   TFLG1 = TFLG1_OC5F;
}

////////////////////////////////////////////////////////////////////////////////

