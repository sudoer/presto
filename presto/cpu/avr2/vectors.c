////////////////////////////////////////////////////////////////////////////////
//   I N T R O D U C T I O N
////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "error.h"

////////////////////////////////////////////////////////////////////////////////
//   E X T E R N A L   F U N C T I O N   P R O T O T Y P E S
////////////////////////////////////////////////////////////////////////////////

extern void reset_vector(void);
extern void context_switch_isr(void);
extern void timer_isr(void);

////////////////////////////////////////////////////////////////////////////////

static void bad_isr_int1(void)     { error_fatal(ERROR_INTVECT_INT1);     }
static void bad_isr_t1cap(void)    { error_fatal(ERROR_INTVECT_T1CAP);    }
static void bad_isr_t1cmpb(void)   { error_fatal(ERROR_INTVECT_T1CMPB);   }
static void bad_isr_t1ovf(void)    { error_fatal(ERROR_INTVECT_T1OVF);    }
static void bad_isr_t0ovf(void)    { error_fatal(ERROR_INTVECT_T0OVF);    }
static void bad_isr_spistc(void)   { error_fatal(ERROR_INTVECT_SPISTC);   }
static void bad_isr_uartrx(void)   { error_fatal(ERROR_INTVECT_UARTRX);   }
static void bad_isr_uartudre(void) { error_fatal(ERROR_INTVECT_UARTUDRE); }
static void bad_isr_uarttx(void)   { error_fatal(ERROR_INTVECT_UARTTX);   }
static void bad_isr_anacomp(void)  { error_fatal(ERROR_INTVECT_ANACOMP);  }

////////////////////////////////////////////////////////////////////////////////
//   I N T E R F A C E   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void vector_table(void) __attribute((section(".vectors")));
void vector_table(void) __attribute((naked));
void vector_table(void) {

   NOT_USED(bad_isr_int1)
   NOT_USED(bad_isr_t1cap)
   NOT_USED(bad_isr_t1cmpb)
   NOT_USED(bad_isr_t1ovf)
   NOT_USED(bad_isr_t0ovf)
   NOT_USED(bad_isr_spistc)
   NOT_USED(bad_isr_uartrx)
   NOT_USED(bad_isr_uartudre)
   NOT_USED(bad_isr_uarttx)
   NOT_USED(bad_isr_anacomp)

   asm volatile (
      "rjmp reset_vector"          "\n\t"  // RESET
      "rjmp context_switch_isr"    "\n\t"  // INT0
      "rjmp bad_isr_int1"          "\n\t"  // INT1
      "rjmp bad_isr_t1cap"         "\n\t"  // TIMER1_CAPT
      "rjmp timer_isr"             "\n\t"  // TIMER1_COMPA
      "rjmp bad_isr_t1cmpb"        "\n\t"  // TIMER1_COMPB
      "rjmp bad_isr_t1ovf"         "\n\t"  // TIMER1_OVF
      "rjmp bad_isr_t0ovf"         "\n\t"  // TIMER0_OVF
      "rjmp bad_isr_spistc"        "\n\t"  // SPI,STC
      "rjmp bad_isr_uartrx"        "\n\t"  // UART_RX
      "rjmp bad_isr_uartudre"      "\n\t"  // UART_UDRE
      "rjmp bad_isr_uarttx"        "\n\t"  // UART_TX
      "rjmp bad_isr_anacomp"       "\n\t"  // ANA_COMP
   );
}

////////////////////////////////////////////////////////////////////////////////

// This is needed because the GCClib wants to control your interrupts
// for you.  It tries to include an interrupt vector table in a section
// called ".vectors".  I throw this section away (except for my own
// vectors above).  But GCClib still looks for a label called
// "__vector_default".  I provide a dummy one here so we can link.

void __vector_default(void) __attribute((naked));
void __vector_default(void) {
   error_fatal(ERROR_INTVECT_DEFAULT);
}

////////////////////////////////////////////////////////////////////////////////

