
extern void _start();      // entry point in crt11.s (ICC only)
extern void inert_isr();

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

#pragma abs_address:0xBFD6 // for SPECIAL BOOTSTRAP and SPECIAL TEST modes
void (*special_interrupt_vectors[])() = {
   inert_isr,   // SCI
   inert_isr,   // SPI
   inert_isr,   // PAIE
   inert_isr,   // PAO
   inert_isr,   // TOF
   inert_isr,   // TOC5
   inert_isr,   // TOC4
   inert_isr,   // TOC3
   inert_isr,   // TOC2
   inert_isr,   // TOC1
   inert_isr,   // TIC3
   inert_isr,   // TIC2
   inert_isr,   // TIC1
   inert_isr,   // RTI
   inert_isr,   // IRQ
   inert_isr,   // XIRQ
   inert_isr,   // SWI
   inert_isr,   // ILLOP
   inert_isr,   // COP
   inert_isr,   // CLM
   _start       // RESET
};
#pragma end_abs_address

////////////////////////////////////////////////////////////////////////////////

