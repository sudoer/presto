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

#include "types.h"
#include "cpu/error.h"
#include "cpu/intvect.h"

////////////////////////////////////////////////////////////////////////////////
//   D A T A   T Y P E S
////////////////////////////////////////////////////////////////////////////////

typedef void (*interrupt_vector)(void);


////////////////////////////////////////////////////////////////////////////////
//   E X T E R N A L   F U N C T I O N   P R O T O T Y P E S
////////////////////////////////////////////////////////////////////////////////

extern void _start(void);      // entry point in crt0.s


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   G L O B A L   D A T A
////////////////////////////////////////////////////////////////////////////////

static BYTE * illop_sp;
static WORD illop_address;


////////////////////////////////////////////////////////////////////////////////
//   S I M P L E   I N T E R R U P T   S E R V I C E   R O U T I N E ( S )
////////////////////////////////////////////////////////////////////////////////

//void inert_isr(void) __attribute_((interrupt));
void inert_isr(void)       { asm("rti"); }
void error_isr(void)       { error_fatal(ERROR_INTVECT_OTHER); }

void error_SCI_isr(void)   { error_fatal(ERROR_INTVECT_SCI); }
void error_SPI_isr(void)   { error_fatal(ERROR_INTVECT_SPI); }
void error_PAIE_isr(void)  { error_fatal(ERROR_INTVECT_PAIE); }
void error_PAO_isr(void)   { error_fatal(ERROR_INTVECT_PAO); }
void error_TOF_isr(void)   { error_fatal(ERROR_INTVECT_TOF); }
void error_TOC5_isr(void)  { error_fatal(ERROR_INTVECT_TOC5); }
void error_TOC4_isr(void)  { error_fatal(ERROR_INTVECT_TOC4); }
void error_TOC3_isr(void)  { error_fatal(ERROR_INTVECT_TOC3); }
void error_TOC2_isr(void)  { error_fatal(ERROR_INTVECT_TOC2); }
void error_TOC1_isr(void)  { error_fatal(ERROR_INTVECT_TOC1); }
void error_TIC3_isr(void)  { error_fatal(ERROR_INTVECT_TIC3); }
void error_TIC2_isr(void)  { error_fatal(ERROR_INTVECT_TIC2); }
void error_TIC1_isr(void)  { error_fatal(ERROR_INTVECT_TIC1); }
void error_RTI_isr(void)   { error_fatal(ERROR_INTVECT_RTI); }
void error_IRQ_isr(void)   { error_fatal(ERROR_INTVECT_IRQ); }
void error_XIRQ_isr(void)  { error_fatal(ERROR_INTVECT_XIRQ); }
void error_SWI_isr(void)   { error_fatal(ERROR_INTVECT_SWI); }
void error_COP_isr(void)   { error_fatal(ERROR_INTVECT_COP); }
void error_ILLOP_isr(void) { error_fatal(ERROR_INTVECT_ILLOP); }
void error_CLM_isr(void)   { error_fatal(ERROR_INTVECT_CLM); }
void error_RESET_isr(void) { error_fatal(ERROR_INTVECT_RESET); }

////////////////////////////////////////////////////////////////////////////////

void illop_isr(void) {
   // When an illegal opcode is encountered, the ILLOP ISR is triggered.
   // The address of the offending instruction is on the stack (where the
   // return address would normally be).
   asm("sts illop_sp");
   illop_address=*((WORD *)(illop_sp+8));
   error_address(illop_address);
}

////////////////////////////////////////////////////////////////////////////////
//   I N T E R R U P T   V E C T O R   T A B L E S
////////////////////////////////////////////////////////////////////////////////

//static void (*special_interrupt_vectors[NUM_INTERRUPTS])()
static interrupt_vector const special_interrupt_vectors[NUM_INTERRUPTS]
   __attribute((section(".specvect"))) = {
   error_SCI_isr,   // SCI
   error_SPI_isr,   // SPI
   error_PAIE_isr,  // PAIE
   error_PAO_isr,   // PAO
   error_TOF_isr,   // TOF
   error_TOC5_isr,  // TOC5
   error_TOC4_isr,  // TOC4
   error_TOC3_isr,  // TOC3
   error_TOC2_isr,  // TOC2
   error_TOC1_isr,  // TOC1
   error_TIC3_isr,  // TIC3
   error_TIC2_isr,  // TIC2
   error_TIC1_isr,  // TIC1
   error_RTI_isr,   // RTI
   error_IRQ_isr,   // IRQ
   error_XIRQ_isr,  // XIRQ
   error_SWI_isr,   // SWI
   error_ILLOP_isr, // ILLOP
   error_COP_isr,   // COP
   error_CLM_isr,   // CLM
   _start           // RESET
};

////////////////////////////////////////////////////////////////////////////////

//static void (*normal_interrupt_vectors[NUM_INTERRUPTS])()
static interrupt_vector normal_interrupt_vectors[NUM_INTERRUPTS]
   __attribute((section(".normvect")));

////////////////////////////////////////////////////////////////////////////////
//   I N T E R F A C E   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void set_interrupt(BYTE intr, void (*function_p)(void)) {
   if (intr<=INTR_RESET) {
      normal_interrupt_vectors[intr]=function_p;
   }
}

////////////////////////////////////////////////////////////////////////////////

void init_interrupts(void) {
   int i;
   //NOT_USED(special_interrupt_vectors);
   for (i=INTR_SCI;i<INTR_RESET;i++) {
      //normal_interrupt_vectors[i]=error_isr;
      normal_interrupt_vectors[i]=special_interrupt_vectors[i];
   }
   normal_interrupt_vectors[INTR_ILLOP]=illop_isr;
   normal_interrupt_vectors[INTR_RESET]=_start;
}

////////////////////////////////////////////////////////////////////////////////
