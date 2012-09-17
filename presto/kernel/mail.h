
#ifndef _MAIL_H_
#define _MAIL_H_

////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "kernel/kernel.h"

////////////////////////////////////////////////////////////////////////////////

typedef unsigned short KERNEL_MAILMSG_T;
typedef void * KERNEL_MAILPTR_T;

////////////////////////////////////////////////////////////////////////////////

typedef struct KERNEL_ENVELOPE_S {
   KERNEL_TASKID_T from_tid;
   struct KERNEL_MAILBOX_S * to_box_p;
   struct {
      KERNEL_MAILMSG_T message;
      KERNEL_MAILPTR_T payload;
   } userdata;
   struct KERNEL_ENVELOPE_S * next;
} KERNEL_ENVELOPE_T;

////////////////////////////////////////////////////////////////////////////////

typedef struct KERNEL_MAILBOX_S {
   unsigned short message_count;
   KERNEL_ENVELOPE_T * mailbox_head;
   KERNEL_ENVELOPE_T * mailbox_tail;
   KERNEL_TASKID_T owner_tid;
   KERNEL_TRIGGER_T trigger;
   KERNEL_TASKID_T waiter_tid;
} KERNEL_MAILBOX_T;

////////////////////////////////////////////////////////////////////////////////

extern void kernel_mail_init(void);

////////////////////////////////////////////////////////////////////////////////

#endif // _MAIL_H_

