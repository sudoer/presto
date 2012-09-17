////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////

// If you've gotten here, you're in trouble.  These functions report an error
// condition.  The functions are application-specific, since different boards
// will have different ways of reporting stuff to the user.

// The Handyboard has four motor drivers with status LED's.  I use them to
// show 8-bit values, but that means I have to multiplex the data in two
// sets of four bits (a motor can not be going forwards and backwards at
// the same time).  While I am at it, I make a little bit of noise.

////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "error_codes.h"
#include "hc11_regs.h"
#include "locks.h"
#include "handyboard.h"
#include "error.h"

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void show_one_byte(BYTE leds) {
   BYTE delay=0;
   // assert LED's 256 times (high nibble 128 times, low nibble 128 times)
   while (--delay>0) {
      if (delay&0x01) MOTOR_LED_PORT=0xF0&((leds&0x0F)<<4);
      else MOTOR_LED_PORT=0x0F|(leds&0xF0);
   }
}

////////////////////////////////////////////////////////////////////////////////
//   E X T E R N A L   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void error_fatal(error_number_e err) {
   cpu_lock();  // no more interrupts
   while (1) {
      TOGGLE_SPEAKER();
      show_one_byte(err);
   }
}

////////////////////////////////////////////////////////////////////////////////

void error_crash(void) {
   // This will cause an ILLEGAL OPERATION interrupt
   asm("test");
}

////////////////////////////////////////////////////////////////////////////////

void error_address(unsigned short address) {
   BYTE delay;
   cpu_lock();  // no more interrupts
   while (1) {
      for (delay=0;delay<150;delay++) {
         show_one_byte(0x00);
      }
      for (delay=0;delay<100;delay++) {
         TOGGLE_SPEAKER();
         show_one_byte((BYTE)(address>>8));
      }
      for (delay=0;delay<25;delay++) {
         show_one_byte(0x00);
      }
      for (delay=0;delay<100;delay++) {
         TOGGLE_SPEAKER();
         show_one_byte((BYTE)(address&0x00FF));
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

