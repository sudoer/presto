
extern void _start(void);      // entry point in crt11.s (ICC only)
extern void inert_isr(void);

extern void inert_sci_isr(void);
extern void inert_spi_isr(void);
extern void inert_paie_isr(void);
extern void inert_pao_isr(void);
extern void inert_tof_isr(void);
extern void inert_toc5_isr(void);
extern void inert_toc4_isr(void);
extern void inert_toc3_isr(void);
extern void inert_toc2_isr(void);
extern void inert_toc1_isr(void);
extern void inert_tic3_isr(void);
extern void inert_tic2_isr(void);
extern void inert_tic1_isr(void);
extern void inert_rti_isr(void);
extern void inert_irq_isr(void);
extern void inert_xirq_isr(void);
extern void inert_swi_isr(void);
extern void inert_illop_isr(void);
extern void inert_cop_isr(void);
extern void inert_clm_isr(void);
extern void inert_reset_isr(void);

////////////////////////////////////////////////////////////////////////////////

// The handyboard boots up in a mode called "SPECIAL TEST" operating mode.
// This is because certain pins are asserted high or low (when the STOP button
// button is pressed, it boots up into SPECIAL BOOTSTRAP mode instead).

// One of the side effects of booting in one of these two so-called "special"
// modes is that the interrupt vector table is stored at 0xBFD6 instead of the
// normal 0xFFD6.  Therefore, if we want to boot properly, we need to initialize
// an interrupt vector table at this address.

// However, we won't use this table for long.  We tend to test our software
// using simulators as well as actual hardware, and most of these simulators
// make the assumption that we are running in normal mode, so they expect the
// interrupt vector table to be at 0xFFD6.  So we'll go ahead and make another
// vector table there, and as soon as we've booted up, we'll start using the
// normal one.

// So this table is only used to get us up and running.  In _HC11Setup(), we'll
// switch to normal mode, and start using the "normal" table.

////////////////////////////////////////////////////////////////////////////////


asm("   .sect .specvect");
asm("   .globl special_interrupt_vectors");

asm("special_interrupt_vectors:");
asm("   .word   inert_isr      ");   //  0xFFC0 - dummy
asm("   .word   inert_isr      ");   //  0xFFC2 - dummy
asm("   .word   inert_isr      ");   //  0xFFC4 - dummy
asm("   .word   inert_isr      ");   //  0xFFC6 - dummy
asm("   .word   inert_isr      ");   //  0xFFC8 - dummy
asm("   .word   inert_isr      ");   //  0xFFCA - dummy
asm("   .word   inert_isr      ");   //  0xFFCC - dummy
asm("   .word   inert_isr      ");   //  0xFFCE - dummy
asm("   .word   inert_isr      ");   //  0xFFD0 - dummy
asm("   .word   inert_isr      ");   //  0xFFD2 - dummy
asm("   .word   inert_isr      ");   //  0xFFD4 - dummy

asm("   .word   inert_sci_isr  ");   //  0xFFD6 - SCI
asm("   .word   inert_spi_isr  ");   //  0xFFD8 - SPI
asm("   .word   inert_paie_isr ");   //  0xFFDA - PAII
asm("   .word   inert_pao_isr  ");   //  0xFFDC - PAOVI
asm("   .word   inert_tof_isr  ");   //  0xFFDE - TOI
asm("   .word   inert_toc5_isr ");   //  0xFFE0 - TOC5
asm("   .word   inert_toc4_isr ");   //  0xFFE2 - TOC4
asm("   .word   inert_toc3_isr ");   //  0xFFE4 - TOC3
asm("   .word   inert_toc2_isr ");   //  0xFFE6 - TOC2
asm("   .word   inert_toc1_isr ");   //  0xFFE8 - TOC1
asm("   .word   inert_tic3_isr ");   //  0xFFEA - TIC3
asm("   .word   inert_tic2_isr ");   //  0xFFEC - TIC2
asm("   .word   inert_tic1_isr ");   //  0xFFEE - TIC1
asm("   .word   inert_rti_isr  ");   //  0xFFF0 - RTII
asm("   .word   inert_irq_isr  ");   //  0xFFF2 - IRQ
asm("   .word   inert_xirq_isr ");   //  0xFFF4 - XIRQ
asm("   .word   inert_swi_isr  ");   //  0xFFF6 - SWI
asm("   .word   inert_illop_isr");   //  0xFFF8 - ILL
asm("   .word   inert_cop_isr  ");   //  0xFFFA - COP Failure
asm("   .word   inert_clm_isr  ");   //  0xFFFC - COP Clock monitor
asm("   .word   _start         ");   //  0xFFFE - reset


/*

#ifdef ICC
   #pragma abs_address:0xBFD6 // for SPECIAL BOOTSTRAP and SPECIAL TEST modes
#endif
#ifdef GCC
   asm("   .sect .specvect");
#endif
void (*special_interrupt_vectors[])() = {
   //__asm__(".org 0xBFD6");
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
#ifdef ICC
   #pragma end_abs_address
#endif

*/

////////////////////////////////////////////////////////////////////////////////
