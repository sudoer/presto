////////////////////////////////////////////////////////////////////////////////
//   I N T R O D U C T I O N
////////////////////////////////////////////////////////////////////////////////
//
// The handyboard boots up in a mode called "SPECIAL TEST" operating mode.
// This is because certain pins are asserted high or low (when the STOP button
// button is pressed, it boots up into SPECIAL BOOTSTRAP mode instead).
//
// One of the side effects of booting in one of these two so-called "special"
// modes is that the interrupt vector table is stored at 0xBFD6 instead of the
// normal 0xFFD6.  Therefore, if we want to boot properly, we need to initialize
// an interrupt vector table at this address.
//
// However, we won't use this table for long.  We tend to test our software
// using simulators as well as actual hardware, and most of these simulators
// make the assumption that we are running in normal mode, so they expect the
// interrupt vector table to be at 0xFFD6.  So we'll go ahead and make another
// vector table there, and as soon as we've booted up, we'll start using the
// normal one.
//
// So this table is only used to get us up and running.  In _HC11Setup(), we'll
// switch to normal mode, and start using the "normal" table.
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "debug.h"
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
void inert_illop_isr(void) { presto_fatal_error(0x55); }
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

void default_interrupts(void) {
   normal_interrupt_vectors[INTR_SCI]=  inert_sci_isr;
   normal_interrupt_vectors[INTR_SPI]=  inert_spi_isr;
   normal_interrupt_vectors[INTR_PAIE]= inert_paie_isr;
   normal_interrupt_vectors[INTR_PAO]=  inert_pao_isr;
   normal_interrupt_vectors[INTR_TOF]=  inert_tof_isr;
   normal_interrupt_vectors[INTR_TOC5]= inert_toc5_isr;
   normal_interrupt_vectors[INTR_TOC4]= inert_toc4_isr;
   normal_interrupt_vectors[INTR_TOC3]= inert_toc3_isr;
   normal_interrupt_vectors[INTR_TOC2]= inert_toc2_isr;
   normal_interrupt_vectors[INTR_TOC1]= inert_toc1_isr;
   normal_interrupt_vectors[INTR_TIC3]= inert_tic3_isr;
   normal_interrupt_vectors[INTR_TIC2]= inert_tic2_isr;
   normal_interrupt_vectors[INTR_TIC1]= inert_tic1_isr;
   normal_interrupt_vectors[INTR_RTI]=  inert_rti_isr;
   normal_interrupt_vectors[INTR_IRQ]=  inert_irq_isr;
   normal_interrupt_vectors[INTR_XIRQ]= inert_xirq_isr;
   normal_interrupt_vectors[INTR_SWI]=  inert_swi_isr;
   normal_interrupt_vectors[INTR_ILLOP]=inert_illop_isr;    // presto_fatal_error;
   normal_interrupt_vectors[INTR_COP]=  inert_cop_isr;
   normal_interrupt_vectors[INTR_CLM]=  inert_clm_isr;
   normal_interrupt_vectors[INTR_RESET]=_start;
}

////////////////////////////////////////////////////////////////////////////////

