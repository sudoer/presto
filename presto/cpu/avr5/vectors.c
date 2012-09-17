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

static void bad_isr_int0(void)         { error_fatal(ERROR_INTVECT_INT0);          }
static void bad_isr_pcint0(void)       { error_fatal(ERROR_INTVECT_PCINT0);        }
static void bad_isr_pcint1(void)       { error_fatal(ERROR_INTVECT_PCINT1);        }
static void bad_isr_timer2_comp(void)  { error_fatal(ERROR_INTVECT_TIMER2_COMP);   }
static void bad_isr_timer2_ovf(void)   { error_fatal(ERROR_INTVECT_TIMER2_OVF);    }
static void bad_isr_timer1_cap(void)   { error_fatal(ERROR_INTVECT_TIMER1_CAP);    }
static void bad_isr_timer1_compa(void) { error_fatal(ERROR_INTVECT_TIMER1_COMPA);  }
static void bad_isr_timer1_compb(void) { error_fatal(ERROR_INTVECT_TIMER1_COMPB);  }
static void bad_isr_timer1_ovf(void)   { error_fatal(ERROR_INTVECT_TIMER1_OVF);    }
static void bad_isr_timer0_comp(void)  { error_fatal(ERROR_INTVECT_TIMER0_COMP);   }
static void bad_isr_timer0_ovf(void)   { error_fatal(ERROR_INTVECT_TIMER0_OVF);    }
static void bad_isr_spi_stc(void)      { error_fatal(ERROR_INTVECT_SPI_STC);       }
static void bad_isr_uart_rx(void)      { error_fatal(ERROR_INTVECT_UART_RX);       }
static void bad_isr_uart_udre(void)    { error_fatal(ERROR_INTVECT_UART_UDRE);     }
static void bad_isr_uart_tx(void)      { error_fatal(ERROR_INTVECT_UART_TX);       }
static void bad_isr_usi_start(void)    { error_fatal(ERROR_INTVECT_USI_START);     }
static void bad_isr_usi_overflow(void) { error_fatal(ERROR_INTVECT_USI_OVERFLOW);  }
static void bad_isr_ana_comp(void)     { error_fatal(ERROR_INTVECT_ANA_COMP);      }
static void bad_isr_adc_ready(void)    { error_fatal(ERROR_INTVECT_ADC_READY);     }
static void bad_isr_ee_ready(void)     { error_fatal(ERROR_INTVECT_EE_READY);      }
static void bad_isr_spm_ready(void)    { error_fatal(ERROR_INTVECT_SPM_READY);     }
static void bad_isr_lcd_stfrm(void)    { error_fatal(ERROR_INTVECT_LCD_STFRM);     }


////////////////////////////////////////////////////////////////////////////////
//   I N T E R F A C E   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void vector_table(void) __attribute((section(".vectors")));
void vector_table(void) __attribute((naked));
void vector_table(void) {

   NOT_USED(bad_isr_int0);
   NOT_USED(bad_isr_pcint0);
   NOT_USED(bad_isr_pcint1);
   NOT_USED(bad_isr_timer2_comp);
   NOT_USED(bad_isr_timer2_ovf);
   NOT_USED(bad_isr_timer1_cap);
   NOT_USED(bad_isr_timer1_compa);
   NOT_USED(bad_isr_timer1_compb);
   NOT_USED(bad_isr_timer1_ovf);
   NOT_USED(bad_isr_timer0_comp);
   NOT_USED(bad_isr_timer0_ovf);
   NOT_USED(bad_isr_spi_stc);
   NOT_USED(bad_isr_uart_rx);
   NOT_USED(bad_isr_uart_udre);
   NOT_USED(bad_isr_uart_tx);
   NOT_USED(bad_isr_usi_start);
   NOT_USED(bad_isr_usi_overflow);
   NOT_USED(bad_isr_ana_comp);
   NOT_USED(bad_isr_adc_ready);
   NOT_USED(bad_isr_ee_ready);
   NOT_USED(bad_isr_spm_ready);
   NOT_USED(bad_isr_lcd_stfrm);

   asm volatile (
      "jmp reset_vector"          "\n\t"  // RESET
      "jmp bad_isr_int0"          "\n\t"  // INT0
      "jmp bad_isr_pcint0"        "\n\t"  // PCINT0
      "jmp bad_isr_pcint1"        "\n\t"  // PCINT1
      "jmp bad_isr_timer2_comp"   "\n\t"  // TIMER2_COMP
      "jmp bad_isr_timer2_ovf"    "\n\t"  // TIMER2_OVF
      "jmp bad_isr_timer1_cap"    "\n\t"  // TIMER1_CAP
      "jmp bad_isr_timer1_compa"  "\n\t"  // TIMER1_COMPA
      "jmp bad_isr_timer1_compb"  "\n\t"  // TIMER1_COMPB
      "jmp bad_isr_timer1_ovf"    "\n\t"  // TIMER1_OVF

      //"jmp bad_isr_timer0_comp"   "\n\t"  // TIMER0_COMP
      "jmp timer_isr"             "\n\t"  // TIMER0_COMP

      "jmp bad_isr_timer0_ovf"    "\n\t"  // TIMER0_OVF
      "jmp bad_isr_spi_stc"       "\n\t"  // SPI_STC
      "jmp bad_isr_uart_rx"       "\n\t"  // UART_RX
      "jmp bad_isr_uart_udre"     "\n\t"  // UART_UDRE
      "jmp bad_isr_uart_tx"       "\n\t"  // UART_TX
      "jmp bad_isr_usi_start"     "\n\t"  // USI_START
      "jmp context_switch_isr"    "\n\t"  // USI_OVERFLOW
      "jmp bad_isr_ana_comp"      "\n\t"  // ANA_COMP
      "jmp bad_isr_adc_ready"     "\n\t"  // ADC_READY
      "jmp bad_isr_ee_ready"      "\n\t"  // EE_READY
      "jmp bad_isr_spm_ready"     "\n\t"  // SPM_READY
      "jmp bad_isr_lcd_stfrm"     "\n\t"  // LCD_STFRM
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

