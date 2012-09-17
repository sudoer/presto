////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "hc11regs.h"
#include "system.h"
#include "kernel\kernel.h"
#include "intvect.h"


extern void _start();   // entry point in crt11.s or crt0.o

////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   D A T A   T Y P E S
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N   P R O T O T Y P E S
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   G L O B A L   D A T A
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//   E X P O R T E D   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

// This function is called from the startup (crt11.s) before interrupts have
// been turned on but after the stack has been set up.

void __premain() {

   INTR_OFF();

   // RAM would start at $0000 if it were enabled
   // control registers are mapped to locations $1000-$103F (default)
   // (must be within first 64 clocks or in special mode)
   INIT=0x01;

   // turn on the A2D subsystem (wait 100 usec before using)
   // use "E clock" to drive the A2D
   // disable COP clock monitor (interrupt)
   // (must be within first 64 clocks or in special mode)
   OPTION=0xA0;  // OPTION_ADPU=1,OPTION_CSEL=0

   // disable output compare interrupts for TOC1,TOC2,TOC3,TOC4,TOC5
   // disable input capture interrupts for TIC1,TIC2,TIC3
   // (OK at any time)
   TMSK1=0x00;

   // set prescaler for timer to 1
   // disable TOF, RTIF, PAOVF, PAIF interrupts
   // (OK at any time)
   TMSK2=0x00;

   // disable SPI subsystem, disable SPI interrupt
   // (OK at any time)
   SPCR=0x04;

   // disable all serial interrupts
   // (OK at any time)
   SCCR2=0x00;

   // disable parallel I/O (and strobe A interrupt)
   // (OK at any time)
   PIOC=0x00;

   // disable SECURITY and COP, disable ROM and EEPROM
   // (OK at any time)
   CONFIG=0x0C;

   //default_interrupts();

   // get out of SPECIAL TEST operating mode
   // go into EXPANDED MULTIPLEXED operating mode
   // promote IRQ interrupt priority
   // (must be in special mode, more or less)
   HPRIO=0x25;
}

////////////////////////////////////////////////////////////////////////////////

void __attribute__((noreturn)) exit(int code) {
   while(1) { }
}

////////////////////////////////////////////////////////////////////////////////

