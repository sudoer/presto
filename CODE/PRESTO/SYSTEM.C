
#include <hc11.h>
#include "hc11regs.h"
#include "system.h"
#include "presto\kernel.h"
#include "services\motors.h"
#include "services\sound.h"

extern void _start();   // entry point in crt11.s

////////////////////////////////////////////////////////////////////////////////

// This function is called from the startup (crt11.s) before interrupts have
// been turned on but after the stack has been set up.

void _HC11Setup() {

   INTR_OFF();

   // request output compare interrupt for TOC1,TOC2,TOC3
   TMSK1 = 0x00;   // not yet ... TMSK1_OC1I | TMSK1_OC2I | TMSK1_OC3I;

   // set prescaler for timer to 1
   // disable TOF, RTIF, PAOVF, PAIF interrupts
   TMSK2 = 0x00;

   // disable SECURITY and COP, disable ROM and EEPROM
   CONFIG=0x0C;

   INTR_ON();
}

////////////////////////////////////////////////////////////////////////////////

#pragma interrupt_handler inert_isr
void inert_isr(void) {
}

////////////////////////////////////////////////////////////////////////////////

#pragma interrupt_handler presto_system_isr

// address FFD6 for simulator
// address BFD6 for handyboard
#pragma abs_address:0xBFD6
static void (*special_interrupt_vectors[])() = {
   inert_isr,          // SCI
   inert_isr,          // SPI
   inert_isr,          // PAIE
   inert_isr,          // PAO
   inert_isr,          // TOF
   inert_isr,          // TOC5
   inert_isr,          // TOC4
   inert_isr,          // TOC3
   presto_system_isr,  // TOC2
   inert_isr,          // TOC1
   inert_isr,          // TIC3
   inert_isr,          // TIC2
   inert_isr,          // TIC1
   inert_isr,          // RTI
   inert_isr,          // IRQ
   inert_isr,          // XIRQ
   presto_fatal_error, // SWI
   inert_isr,          // ILLOP
   inert_isr,          // COP
   inert_isr,          // CLM
   _start              // RESET
};
#pragma end_abs_address

////////////////////////////////////////////////////////////////////////////////

//#pragma abs_address:0xBFD6
//static void (*normal_interrupt_vectors[21])();
//#pragma end_abs_address

////////////////////////////////////////////////////////////////////////////////

void set_interrupt(BYTE intr, void (*vector)(void)) {
   if(intr<=INTR_RESET) {
      //special_interrupt_vectors[intr]=vector;
      //normal_interrupt_vectors[intr]=vector;
   }
}

////////////////////////////////////////////////////////////////////////////////

void normal_mode(void) {
   INTR_OFF();

   set_interrupt(INTR_SCI  ,inert_isr);
   set_interrupt(INTR_SPI  ,inert_isr);
   set_interrupt(INTR_PAIE ,inert_isr);
   set_interrupt(INTR_PAO  ,inert_isr);
   set_interrupt(INTR_TOF  ,inert_isr);
   set_interrupt(INTR_TOC5 ,inert_isr);
   set_interrupt(INTR_TOC4 ,inert_isr);
   set_interrupt(INTR_TOC3 ,inert_isr);
   set_interrupt(INTR_TOC2 ,inert_isr);
   set_interrupt(INTR_TOC1 ,inert_isr);
   set_interrupt(INTR_TIC3 ,inert_isr);
   set_interrupt(INTR_TIC2 ,inert_isr);
   set_interrupt(INTR_TIC1 ,inert_isr);
   set_interrupt(INTR_RTI  ,inert_isr);
   set_interrupt(INTR_IRQ  ,inert_isr);
   set_interrupt(INTR_XIRQ ,inert_isr);
   set_interrupt(INTR_SWI  ,inert_isr);
   set_interrupt(INTR_ILLOP,inert_isr);
   set_interrupt(INTR_COP  ,inert_isr);
   set_interrupt(INTR_CLM  ,inert_isr);
   set_interrupt(INTR_RESET,_start);

   HPRIO=0x2C;   // Expanded Multiplexed mode, promote TOC2

   INTR_ON();
};

////////////////////////////////////////////////////////////////////////////////

