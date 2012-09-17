
#ifndef DEBUG_H
#define DEBUG_H

////////////////////////////////////////////////////////////////////////////////

#include "types.h"

////////////////////////////////////////////////////////////////////////////////

typedef enum {
   ERROR_CONTEXTSWITCH_STACKCLOBBERED,
   ERROR_CREATE_BEFORE_INIT,
   ERROR_DELIVER_NOFREE,
   ERROR_DELIVER_TCBPNULL,
   ERROR_INTR_ILLOP,
   ERROR_MAIN_END,
   ERROR_NEXTTCB_NOTFOUND,
   ERROR_NO_MORE_TCB,
   ERROR_START_AFTERRTI,
   ERROR_SYSTEMISR_STACKCLOBBERED,
   ERROR_TCB_HEAD_IS_NULL,
   ERROR_TIDTOTCB_RANGE,
   ERROR_TIMER_NOFREE,
   ERROR_TIMER_TONOBODY,
   ERROR_WAITFORMSG_NOMESSAGES,
   ERROR_WAITFORMSG_NOTFORME,
} error_number_e;

////////////////////////////////////////////////////////////////////////////////

extern void presto_fatal_error(error_number_e err);

////////////////////////////////////////////////////////////////////////////////

#endif

