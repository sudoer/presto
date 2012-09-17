
#ifndef _ERROR_H_
#define _ERROR_H_

////////////////////////////////////////////////////////////////////////////////

typedef enum {

     ERROR_MAIN_AFTERSTART,                       // 00
     ERROR_INTVECT_OTHER,                         // 01
     ERROR_KERNEL_CREATE_BEFORE_INIT,             // 02
     ERROR_KERNEL_CREATE_NO_MORE_TCBS,            // 03
     ERROR_KERNEL_START_NOTASKS,                  // 04
     ERROR_KERNEL_START_AFTER_RTI,                // 05
     ERROR_KERNEL_SCHEDULER_ERROR,                // 06
     ERROR_KERNEL_MAILSEND_NOFREE,                // 07
     ERROR_KERNEL_MAILSEND_TONULLBOX,             // 08
     ERROR_KERNEL_MAILSEND_TONOBODY,              // 09
     ERROR_KERNEL_MAILWAIT_NOMAIL,                // 10
     ERROR_KERNEL_MAILGET_NOMESSAGES,             // 11
     ERROR_KERNEL_MAILGET_NOTFORME,               // 12
     ERROR_KERNEL_CONTEXTSWITCH_STACKCLOBBERED,   // 13
     ERROR_KERNEL_TIDTOTCB_RANGE,                 // 14
     ERROR_KERNEL_SEMWAIT_TOOMANYUSERS,           // 15

} error_number_e;

////////////////////////////////////////////////////////////////////////////////

#endif

