
#ifndef _ERROR_CODES_H_
#define _ERROR_CODES_H_

////////////////////////////////////////////////////////////////////////////////

typedef enum {

   // KERNEL ERRORS
   ERROR_KERNEL_CONTEXTSWITCH_STACKCLOBBERED,    // 00
   ERROR_KERNEL_CREATEBEFOREINIT,                // 01
   ERROR_KERNEL_NOMORETCBS,                      // 02
   ERROR_KERNEL_NOTASKTOSTART,                   // 03
   ERROR_KERNEL_SCHEDULERERROR,                  // 04
   ERROR_KERNEL_STARTAFTERRTI,                   // 05
   ERROR_KERNEL_TIDTOTCB_RANGE,                  // 06
   ERROR_MAIL_SENDNULLENVELOPE,                  // 07
   ERROR_MAIL_SENDTONULLBOX,                     // 08
   ERROR_MEMORY_CLOBBEREDMEMBLOCK,               // 09
   ERROR_SEMAPHORE_TOOMANYWAITERS,               // 0A

   // CPU-SPECIFIC ERRORS

#ifdef CPU_M68HC11
   ERROR_INTVECT_SCI,                            // 0B
   ERROR_INTVECT_SPI,                            // 0C
   ERROR_INTVECT_PAIE,                           // 0D
   ERROR_INTVECT_PAO,                            // 0E
   ERROR_INTVECT_TOF,                            // 0F
   ERROR_INTVECT_TOC5,                           // 10
   ERROR_INTVECT_TOC4,                           // 11
   ERROR_INTVECT_TOC3,                           // 12
   ERROR_INTVECT_TOC2,                           // 13
   ERROR_INTVECT_TOC1,                           // 14
   ERROR_INTVECT_TIC3,                           // 15
   ERROR_INTVECT_TIC2,                           // 16
   ERROR_INTVECT_TIC1,                           // 17
   ERROR_INTVECT_RTI,                            // 18
   ERROR_INTVECT_IRQ,                            // 19
   ERROR_INTVECT_XIRQ,                           // 1A
   ERROR_INTVECT_SWI,                            // 1B
   ERROR_INTVECT_COP,                            // 1C
   ERROR_INTVECT_ILLOP,                          // 1D
   ERROR_INTVECT_CLM,                            // 1E
   ERROR_INTVECT_RESET,                          // 1F
   ERROR_INTVECT_OTHER,                          // 20
#endif

#ifdef CPU_AVR8515
   ERROR_INTVECT_INT0,
   ERROR_INTVECT_INT1,
   ERROR_INTVECT_T1CAP,
   ERROR_INTVECT_T1CMPA,
   ERROR_INTVECT_T1CMPB,
   ERROR_INTVECT_T1OVF,
   ERROR_INTVECT_T0OVF,
   ERROR_INTVECT_SPISTC,
   ERROR_INTVECT_UARTRX,
   ERROR_INTVECT_UARTUDRE,
   ERROR_INTVECT_UARTTX,
   ERROR_INTVECT_ANACOMP,
   ERROR_INTVECT_OTHER,
#endif

   // APPLICATION ERRORS
   ERROR_MAIN_AFTERSTART,                        // 21

} error_number_e;

////////////////////////////////////////////////////////////////////////////////

#endif

