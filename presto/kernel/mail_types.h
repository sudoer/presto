
#ifndef _MAIL_TYPES_H_
#define _MAIL_TYPES_H_

////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "kernel/kernel_types.h"

////////////////////////////////////////////////////////////////////////////////

typedef union KERNEL_MAIL_U {
   struct {DWORD dw1;} dw;
   struct {WORD w1,w2;} w;
   struct {void *p1,*p2;} p;
   struct {BYTE b1,b2,b3,b4;} b;
} KERNEL_MAIL_T;

////////////////////////////////////////////////////////////////////////////////

typedef struct KERNEL_MESSAGE_S {
   KERNEL_TID_T from_tid;
   union KERNEL_MAIL_U payload;
   struct KERNEL_MAILBOX_S * to_box_p;
   struct KERNEL_MESSAGE_S * next;
} KERNEL_MESSAGE_T;

////////////////////////////////////////////////////////////////////////////////

typedef struct KERNEL_MAILBOX_S {
   unsigned short message_count;
   KERNEL_MESSAGE_T * mailbox_head;
   KERNEL_MESSAGE_T * mailbox_tail;
   KERNEL_TCB_T * owner_tcb_p;
   KERNEL_TRIGGER_T trigger;
} KERNEL_MAILBOX_T;

////////////////////////////////////////////////////////////////////////////////

#endif

