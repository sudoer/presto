////////////////////////////////////////////////////////////////////////////////
//   I N T R O D U C T I O N
////////////////////////////////////////////////////////////////////////////////
//
// The handyboard boots up in a mode called "SPECIAL TEST" operating mode.
// The designers of the Handyboard made sure that certain pins were asserted
// high or low at boot time, and when the HC11 wakes up, it checks these pins
// to determine which of the four operating modes it should work in.  The
// Handyboard designers also arranged it so that if you press the STOP button
// during bootup, it will come up into SPECIAL BOOTSTRAP mode instead of
// SPECIAL TEST mode.
//
// One of the side effects of booting in one of these two so-called "special"
// modes is that the interrupt vector table is stored at 0xBFD6 instead of the
// normal 0xFFD6.  Therefore, if we want to boot properly, we need to initialize
// an interrupt vector table at this 0xBFD6 address.
//
// However, we won't use this table at 0xBFD6 for long.  Regardless of the mode
// that we boot up in, I want to run the board in NORMAL mode.  So shortly
// after bootup, I set a value in the HPRIO register that changes the mode of
// the processor.  When we change modes, we will start using the interrupt
// vector table at the normal address.

// The bottom line is that we need two interrupt vector tables.  The one at
// 0xBFD6 is used for the first hundred CPU cycles or so.  And then we switch
// into normal mode and use the other vector table for the rest of our
// operation.
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "error.h"
#include "intvect.h"


////////////////////////////////////////////////////////////////////////////////
//   E X T E R N A L   F U N C T I O N   P R O T O T Y P E S
////////////////////////////////////////////////////////////////////////////////

extern void _start(void);      // entry point in crt11.s (ICC only)


////////////////////////////////////////////////////////////////////////////////
//   I N E R T   I N T E R R U P T   S E R V I C E   R O U T I N E ( S )
////////////////////////////////////////////////////////////////////////////////

void inert_isr(void)       { asm("rti"); }

void inert_sci_isr(void)   { asm("rti"); }
void inert_spi_isr(void)   { asm("rti"); }
void inert_paie_isr(void)  { asm("rti"); }
void inert_pao_isr(void)   { asm("rti"); }
void inert_tof_isr(void)   { asm("rti"); }
void inert_toc5_isr(void)  { asm("rti"); }
void inert_toc4_isr(void)  { asm("rti"); }
void inert_toc3_isr(void)  { asm("rti"); }
void inert_toc2_isr(void)  { asm("rti"); }
void inert_toc1_isr(void)  { asm("rti"); }
void inert_tic3_isr(void)  { asm("rti"); }
void inert_tic2_isr(void)  { asm("rti"); }
void inert_tic1_isr(void)  { asm("rti"); }
void inert_rti_isr(void)   { asm("rti"); }
void inert_irq_isr(void)   { asm("rti"); }
void inert_xirq_isr(void)  { asm("rti"); }
void inert_swi_isr(void)   { asm("rti"); }
void inert_illop_isr(void) { presto_fatal_error(ERROR_INTR_ILLOP); }
void inert_cop_isr(void)   { asm("rti"); }
void inert_clm_isr(void)   { asm("rti"); }
void inert_reset_isr(void) { asm("rti"); }

////////////////////////////////////////////////////////////////////////////////
//   I N T E R R U P T   V E C T O R   T A B L E S
////////////////////////////////////////////////////////////////////////////////

static void (*special_interrupt_vectors[])()
   __attribute((section(".specvect"))) = {
   inert_sci_isr,      // SCI
   inert_spi_isr,      // SPI
   inert_paie_isr,     // PAIE
   inert_pao_isr,      // PAO
   inert_tof_isr,      // TOF
   inert_toc5_isr,     // TOC5
   inert_toc4_isr,     // TOC4
   inert_toc3_isr,     // TOC3
   inert_toc2_isr,     // TOC2
   inert_toc1_isr,     // TOC1
   inert_tic3_isr,     // TIC3
   inert_tic2_isr,     // TIC2
   inert_tic1_isr,     // TIC1
   inert_rti_isr,      // RTI
   inert_irq_isr,      // IRQ
   inert_xirq_isr,     // XIRQ
   inert_swi_isr,      // SWI
   inert_illop_isr,    // ILLOP
   inert_cop_isr,      // COP
   inert_clm_isr,      // CLM
   _start              // RESET
};

////////////////////////////////////////////////////////////////////////////////

static void (*normal_interrupt_vectors[])()
   __attribute((section(".normvect"))) = {
   inert_sci_isr,      // SCI    -   presto_serial_isr
   inert_spi_isr,      // SPI
   inert_paie_isr,     // PAIE
   inert_pao_isr,      // PAO
   inert_tof_isr,      // TOF
   inert_toc5_isr,     // TOC5
   inert_toc4_isr,     // TOC4
   inert_toc3_isr,     // TOC3   -   motor_isr
   inert_toc2_isr,     // TOC2   -   presto_system_isr
   inert_toc1_isr,     // TOC1
   inert_tic3_isr,     // TIC3
   inert_tic2_isr,     // TIC2
   inert_tic1_isr,     // TIC1
   inert_rti_isr,      // RTI
   inert_irq_isr,      // IRQ
   inert_xirq_isr,     // XIRQ
   inert_swi_isr,      // SWI
   inert_illop_isr,    // ILLOP
   inert_cop_isr,      // COP
   inert_clm_isr,      // CLM
   inert_reset_isr     // RESET
};

////////////////////////////////////////////////////////////////////////////////
//   I N T E R F A C E   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

//extern void os_set_irq(int number, void (*fn)() );
void set_interrupt(BYTE intr, void (*vector)(void)) {
   if(intr<=INTR_RESET) {
      normal_interrupt_vectors[intr]=vector;
   }
}

////////////////////////////////////////////////////////////////////////////////

