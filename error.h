
#ifndef DEBUG_H
#define DEBUG_H

////////////////////////////////////////////////////////////////////////////////

#include "types.h"

////////////////////////////////////////////////////////////////////////////////

typedef enum {
   ERROR_MAIN_END,                            // 00
   ERROR_CONTEXTSWITCH_STACKCLOBBERED,        // 01
   ERROR_CREATE_BEFORE_INIT,                  // 02
   ERROR_DELIVER_NOFREE,                      // 03
   ERROR_DELIVER_TCBPNULL,                    // 04
   ERROR_INTR_ILLOP,                          // 05
   ERROR_INTR_OTHER,                          // 06
   ERROR_NEXTTCB_NOTFOUND,                    // 07
   ERROR_NO_MORE_TCB,                         // 08
   ERROR_START_AFTERRTI,                      // 09
   ERROR_START_BEFORE_INIT,                   // 0A
   ERROR_SYSTEMISR_STACKCLOBBERED,            // 0B
   ERROR_TCB_HEAD_IS_NULL,                    // 0C
   ERROR_TIDTOTCB_RANGE,                      // 0D
   ERROR_TIMER_NOFREE,                        // 0E
   ERROR_TIMER_TONOBODY,                      // 0F
   ERROR_MAILGET_NOMESSAGES,                  // 10
   ERROR_MAILGET_NOTFORME,                    // 11
   ERROR_MAIL_DESTBOXNULL,                    // 12
   ERROR_MAIL_NOFREE,                         // 13
   ERROR_MAIL_TONULLBOX,                      // 14
   ERROR_MAIL_TONOBODY,                       // 15
   ERROR_MAILWAIT_NOMAIL,                     // 16
   ERROR_TIMER_ERROR1,                        // 17
} error_number_e;

////////////////////////////////////////////////////////////////////////////////

extern void presto_fatal_error(error_number_e err);

////////////////////////////////////////////////////////////////////////////////

#endif

