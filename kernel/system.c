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

   // THE FOLLOWING (*) REGISTERS MAY ONLY BE ALTERED WITHIN THE FIRST 64 CLOCK CYCLES

   // 11110000 * map internal 1k RAM to $0000-$00FF (default)
   // 00001111 * map control registers to $1000-$103F (default)
   INIT=0x01;

   // 11110000 - disable TOF, RTIF, PAOVF, PAIF interrupts
   // 00000011 * set prescaler for timer to 16
   TMSK2=0x06;

   // turn on the A2D subsystem (wait 100 usec before using)
   // use "E clock" to drive the A2D
   // disable COP clock monitor (interrupt)
   // 10000000 - A/D system powered up
   // 01000000 - A/D and EE use E-clock
   // 00100000 * IRQ is edge-sensitive
   // 00010000 * no startup delay
   // 00001000 - disable clock monitor
   // 00000011 * watchdog rate
   OPTION=0xA3;  // OPTION_ADPU=1,OPTION_CSEL=0


   // THIS REGISTER MUST BE WRITTEN LIKE AN EEPROM LOCATION

   // disable SECURITY, enable COP, disable ROM and EEPROM
   // CONFIG=0x0C;


   // ALL OTHER REGISTERS MAY BE ALTERED AT ANY TIME

   // disable parallel I/O (and strobe A interrupt)
   PIOC=0x03;

   // disable all serial interrupts
   SCCR2=0x00;

   // disable SPI subsystem, disable SPI interrupt
   SPCR=0x04;

   // timers 2-5 disconnected from output pin logic
   TCTL1=0x00;

   // timers 1-3 capture disabled
   TCTL2=0x00;

   // disable output compare interrupts for TOC1,TOC2,TOC3,TOC4,TOC5
   // disable input capture interrupts for TIC1,TIC2,TIC3
   TMSK1=0x00;

   // get out of SPECIAL TEST operating mode
   // go into NORMAL EXPANDED MULTIPLEXED operating mode
   // no bootstrap ROM, no visibility of internal reads
   // promote IRQ interrupt priority
   // (must be in special mode to change this)
   HPRIO=0x25;

}

////////////////////////////////////////////////////////////////////////////////

void __attribute__((noreturn)) exit(int code) {
   while(1) { }
}

////////////////////////////////////////////////////////////////////////////////

