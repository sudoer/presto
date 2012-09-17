////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////

// This is the first code that runs.


////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "configure.h"
#include "cpu_locks.h"
#include "boot.h"
#include "types.h"
#include "registers.h"
#include <avr/io.h>


////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//   G L O B A L   D A T A
////////////////////////////////////////////////////////////////////////////////

BYTE initial_stack[BOOT_INITIALSTACKSIZE] __attribute((section(".stack")));


////////////////////////////////////////////////////////////////////////////////
//   E X P O R T E D   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void reset_vector() __attribute__((naked));
void reset_vector() {
   cpu_lock();

   asm volatile ("clr __zero_reg__");
   asm volatile ("out __SREG__,__zero_reg__");
   outw(SPL,(unsigned short)(initial_stack+BOOT_INITIALSTACKSIZE-1));

   // set Clock Prescaler Change Enable
   CLKPR = (1<<CLKPCE);
   // main clock = internal RC 8Mhz / 8 = 1Mhz
   // set prescaler = 8, resulting in 125kHz
   CLKPR = 0;  // was (3<<CLKPS0);

   // Disable Analog Comparator (power save)
   ACSR = (1<<ACD);

   // Disable Digital input on PF0-2 (power save)
   DIDR1 = (7<<ADC0D);

   // Enable pullup on certain ports
   PORTB = (15<<PB0);
   PORTE = (15<<PE4);

   // set OC1A (speaker) as output
   sbi(DDRB, 5);

   asm volatile("jmp __init");
}

////////////////////////////////////////////////////////////////////////////////

