#ifndef _PRESTO_H_
#define _PRESTO_H_

////////////////////////////////////////////////////////////////////////////////

// This is the main header file for the operating system.  Below you
// will find all of the constants, type declarations and function
// prototypes necessary for basic multitasking.

////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "clock.h"

////////////////////////////////////////////////////////////////////////////////

typedef signed char PRESTO_TID_T;
typedef signed char PRESTO_MSGID_T;
typedef unsigned char PRESTO_FLAG_T;
typedef unsigned short PRESTO_INTERVAL_T;

////////////////////////////////////////////////////////////////////////////////

typedef struct PRESTO_MAILBOX_S {
   unsigned short message_count;
   struct PRESTO_MESSAGE_S * mailbox_head;
   struct PRESTO_MESSAGE_S * mailbox_tail;
   struct PRESTO_TCB_S * owner_tcb_p;
   PRESTO_FLAG_T trigger_flag;
} PRESTO_MAILBOX_T;

////////////////////////////////////////////////////////////////////////////////

typedef struct PRESTO_TIMER_S {
   PRESTO_TIME_T delivery_time;
   PRESTO_INTERVAL_T timer_period;
   struct PRESTO_TCB_S * owner_tcb_p;
   PRESTO_FLAG_T trigger_flag;
   struct PRESTO_TIMER_S * next;
} PRESTO_TIMER_T;

////////////////////////////////////////////////////////////////////////////////

typedef union PRESTO_MAIL_U {
   struct {DWORD dw1;} dw;
   struct {WORD w1,w2;} w;
   struct {void *p1,*p2;} p;
   struct {BYTE b1,b2,b3,b4;} b;
} PRESTO_MAIL_T;

////////////////////////////////////////////////////////////////////////////////

// KERNEL

void presto_init(void);
PRESTO_TID_T presto_create_task( void (*func)(void), BYTE * stack, short stack_size, BYTE priority );
void presto_start_scheduler(void);
void presto_sleep(void);
void presto_get_clock(PRESTO_TIME_T * clk);
PRESTO_FLAG_T presto_wait(PRESTO_FLAG_T flags);

// mail
void presto_mailbox_init(PRESTO_MAILBOX_T * box_p, PRESTO_FLAG_T flag);
BOOLEAN presto_mail_get(PRESTO_MAILBOX_T * box_p, PRESTO_MAIL_T * payload_p);
PRESTO_MSGID_T presto_mail_send(PRESTO_MAILBOX_T * box_p, PRESTO_MAIL_T payload);

BOOLEAN presto_mail_waiting(PRESTO_MAILBOX_T * box_p);
PRESTO_MSGID_T presto_wait_for_message(PRESTO_MAIL_T * payload_p);

// timers
void presto_timer(PRESTO_TIMER_T * timer_p, PRESTO_INTERVAL_T delay, PRESTO_INTERVAL_T period, PRESTO_FLAG_T flag);


////////////////////////////////////////////////////////////////////////////////

#endif

