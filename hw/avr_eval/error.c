////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////

// If you've gotten here, you're in trouble.  These functions report an error
// condition.  The functions are application-specific, since different boards
// will have different ways of reporting stuff to the user.

// The AVR proto board has eight LED's that we use to show error codes.

////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "error.h"
#include "cpu_locks.h"
#include "registers.h"
#include <avr/io.h>


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void show_one_byte(BYTE leds) {
   volatile unsigned short delay;
   // "flicker" the byte on and off
   outb(PORTB,0xFF^leds);
   for(delay=0;delay<8000;delay++) ;
   outb(PORTB,0xFF);
   for(delay=0;delay<8000;delay++) ;
}

////////////////////////////////////////////////////////////////////////////////
//   E X T E R N A L   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void error_fatal(error_number_e err) {
   cpu_lock();  // no more interrupts
   while (1) {
      show_one_byte(err);
   }
}

////////////////////////////////////////////////////////////////////////////////

void error_crash(void) {
   // This will cause an ILLEGAL OPERATION interrupt
   /* asm("test"); */
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
         show_one_byte((BYTE)(address>>8));
      }
      for (delay=0;delay<25;delay++) {
         show_one_byte(0x00);
      }
      for (delay=0;delay<100;delay++) {
         show_one_byte((BYTE)(address&0x00FF));
      }
   }
}

////////////////////////////////////////////////////////////////////////////////





