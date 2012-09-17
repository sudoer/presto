
#ifndef _ERROR_H_
#define _ERROR_H_

////////////////////////////////////////////////////////////////////////////////

typedef enum {

     ERROR_INTVECT_ILLOP,
     ERROR_INTVECT_OTHER,

     ERROR_KERNEL_CREATE_BEFORE_INIT,
     ERROR_KERNEL_CREATE_NO_MORE_TCBS,
     ERROR_KERNEL_START_NOTASKS,
     ERROR_KERNEL_START_AFTER_RTI,
     ERROR_KERNEL_SCHEDULER_ERROR,
     ERROR_KERNEL_MAILSEND_NOFREE,
     ERROR_KERNEL_MAILSEND_TONULLBOX,
     ERROR_KERNEL_MAILSEND_TONOBODY,
     ERROR_KERNEL_MAILWAIT_NOMAIL,
     ERROR_KERNEL_MAILGET_NOMESSAGES,
     ERROR_KERNEL_MAILGET_NOTFORME,
     ERROR_KERNEL_CONTEXTSWITCH_STACKCLOBBERED,
     ERROR_KERNEL_TIDTOTCB_RANGE,

     ERROR_MAIN_AFTERSTART,

} error_number_e;

////////////////////////////////////////////////////////////////////////////////

extern void presto_fatal_error(error_number_e err);

////////////////////////////////////////////////////////////////////////////////

#endif

