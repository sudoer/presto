
#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "presto.h"
#include "types.h"

////////////////////////////////////////////////////////////////////////////////

#define MAX_TASKS            6
#define MAX_MESSAGES         20

////////////////////////////////////////////////////////////////////////////////

typedef struct PRESTO_MESSAGE {
   WORD serial_number;
   PRESTO_TID_T from_tid;
   struct PRESTO_TCB * to_tcb_p;
   PRESTO_TIME_T delivery_time;
   PRESTO_MAIL_T payload;
   struct PRESTO_MESSAGE * next;
} PRESTO_MESSAGE_T;

////////////////////////////////////////////////////////////////////////////////

typedef enum {
   STATE_READY,
   STATE_BLOCKED,
   STATE_INACTIVE
// STATE_RUNNING
} PRESTO_TASK_STATE_T;

////////////////////////////////////////////////////////////////////////////////

typedef struct PRESTO_TCB {
   PRESTO_TID_T task_id;
   BYTE * stack_ptr;
   BYTE * stack_top;
   BYTE * stack_bottom;
   BYTE priority;
   PRESTO_TASK_STATE_T state;
   struct PRESTO_TCB * next;
   struct PRESTO_MESSAGE * mailbox_head;
   struct PRESTO_MESSAGE * mailbox_tail;
} PRESTO_TCB_T;

////////////////////////////////////////////////////////////////////////////////

#endif

