////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "hc11regs.h"
#include "system.h"
#include "kernel\kernel.h"
//#include "services\serial.h"
//#include "services\motors.h"
//#include "services\sound.h"


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

#pragma interrupt presto_swi
void presto_swi(void);

#pragma interrupt inert_isr
void inert_isr(void);

void inert_sci_isr(void);
void inert_spi_isr(void);
void inert_paie_isr(void);
void inert_pao_isr(void);
void inert_tof_isr(void);
void inert_toc5_isr(void);
void inert_toc4_isr(void);
void inert_toc3_isr(void);
void inert_toc2_isr(void);
void inert_toc1_isr(void);
void inert_tic3_isr(void);
void inert_tic2_isr(void);
void inert_tic1_isr(void);
void inert_rti_isr(void);
void inert_irq_isr(void);
void inert_xirq_isr(void);
void inert_swi_isr(void);
void inert_illop_isr(void);
void inert_cop_isr(void);
void inert_clm_isr(void);
void inert_reset_isr(void);

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   G L O B A L   D A T A
////////////////////////////////////////////////////////////////////////////////

// INTERRUPT VECTORS

/*

#ifdef ICC
   #pragma abs_address:0xFFD6 // for NORMAL and EXPANDED MULTIPLEXED modes
#endif
void (*normal_interrupt_vectors[])() = {
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
#ifdef ICC
   #pragma end_abs_address
#endif

*/

extern void (*normal_interrupt_vectors[32])();

asm("   .sect .normvect");
asm("   .globl normal_interrupt_vectors");

asm("normal_interrupt_vectors:");
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
asm("   .word   inert_reset_isr");   //  0xFFFE - reset




////////////////////////////////////////////////////////////////////////////////
//   E X P O R T E D   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

// This function is called from the startup (crt11.s) before interrupts have
// been turned on but after the stack has been set up.

#ifdef ICC
void _HC11Setup() {
#endif
#ifdef GCC
void __premain() {
#endif

   INTR_OFF();

   // RAM would start at $0000 if it were enabled
   // control registers are mapped to locations $1000-$103F (default)
   INIT=0x01;

   // disable output compare interrupts for TOC1,TOC2,TOC3,TOC4,TOC5
   // disable input capture interrupts for TIC1,TIC2,TIC3
   TMSK1=0x00;

   // set prescaler for timer to 1
   // disable TOF, RTIF, PAOVF, PAIF interrupts
   TMSK2=0x00;

   // disable SPI subsystem, disable SPI interrupt
   SPCR=0x04;

   // disable all serial interrupts
   SCCR2=0x00;

   // disable parallel I/O (and strobe A interrupt)
   PIOC=0x00;

   // disable SECURITY and COP, disable ROM and EEPROM
   CONFIG=0x0C;

   // turn on the A2D subsystem (wait 100 usec before using)
   // use "E clock" to drive the A2D
   // disable COP clock monitor (interrupt)
   OPTION=0xA0;  // OPTION_ADPU=1,OPTION_CSEL=0

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
   normal_interrupt_vectors[INTR_ILLOP]=presto_fatal_error;
   normal_interrupt_vectors[INTR_COP]=  inert_cop_isr;
   normal_interrupt_vectors[INTR_CLM]=  inert_clm_isr;
   normal_interrupt_vectors[INTR_RESET]=_start;

   // get out of SPECIAL TEST operating mode
   // go into EXPANDED MULTIPLEXED operating mode
   // promote IRQ interrupt priority
   HPRIO=0x25;

   //INTR_ON();
}

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
void inert_illop_isr(void) { asm("rti"); }
void inert_cop_isr(void)   { asm("rti"); }
void inert_clm_isr(void)   { asm("rti"); }
void inert_reset_isr(void) { asm("rti"); }

////////////////////////////////////////////////////////////////////////////////

//extern void os_set_irq(int number, void (*fn)() );
void set_interrupt(BYTE intr, void (*vector)(void)) {
   if(intr<=INTR_RESET) {
      normal_interrupt_vectors[intr]=vector;
   }
}

////////////////////////////////////////////////////////////////////////////////

#ifdef GCC
void __attribute__((noreturn)) exit(int code) {
}
#endif

////////////////////////////////////////////////////////////////////////////////
//   S A F E T Y   C H E C K
////////////////////////////////////////////////////////////////////////////////

// this is the memory location for the motor controller
#define ERROR_PORT *(unsigned char *)(0x7FFF)

void presto_fatal_error(void) {
   // should never get here
   BYTE delay;
   INTR_OFF();

   // reload the original stack pointer, so we don't trash anything else
#ifdef ICC
   asm("lds #init_sp");
#endif
#ifdef GCC
   asm("lds #0xB5FF");
#endif

   // speaker is always an output
   BITSET(DDRD,4);              // LED is an output
   while(1) {
      // toggle speaker
      BITNOT(PORTA,3);
      // LED on
      BITCLR(PORTD,4);
      // delay
      while(--delay>0) {
         // This will force the motor lights to blink so fast
         // that all eight of them will appear to be on.
         ERROR_PORT=delay;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
