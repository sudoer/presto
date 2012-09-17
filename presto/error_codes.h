
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
     ERROR_MAIL_SENDNULLENVELOPE,
     ERROR_MAIL_SENDTONULLBOX,                     // 07
     ERROR_MEMORY_CLOBBEREDMEMBLOCK,               // 08
     ERROR_SEMAPHORE_TOOMANYWAITERS,               // 09

     // CPU-SPECIFIC ERRORS
     ERROR_INTVECT_OTHER,                          // 0A

     // APPLICATION ERRORS
     ERROR_MAIN_AFTERSTART,                        // 0B

} error_number_e;

////////////////////////////////////////////////////////////////////////////////

#endif

