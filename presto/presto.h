#ifndef _PRESTO_H_
#define _PRESTO_H_

////////////////////////////////////////////////////////////////////////////////

// This is the main header file for the operating system.  Below you
// will find all of the constants, type declarations and function
// prototypes necessary for basic multitasking.

////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "kernel/clock.h"
#include "kernel/kernel.h"
#include "kernel/mail.h"
#include "kernel/timer.h"

////////////////////////////////////////////////////////////////////////////////

typedef KERNEL_TIME_T       PRESTO_TIME_T;
typedef KERNEL_MAILBOX_T    PRESTO_MAILBOX_T;
typedef KERNEL_TIMER_T      PRESTO_TIMER_T;
typedef KERNEL_TID_T        PRESTO_TID_T;
typedef KERNEL_MSGID_T      PRESTO_MSGID_T;
typedef KERNEL_FLAG_T       PRESTO_FLAG_T;
typedef KERNEL_INTERVAL_T   PRESTO_INTERVAL_T;

////////////////////////////////////////////////////////////////////////////////

typedef union KERNEL_MAIL_U {
   struct {DWORD dw1;} dw;
   struct {WORD w1,w2;} w;
   struct {void *p1,*p2;} p;
   struct {BYTE b1,b2,b3,b4;} b;
} PRESTO_MAIL_T;

////////////////////////////////////////////////////////////////////////////////
//   K E R N E L
////////////////////////////////////////////////////////////////////////////////

// initialization
extern void presto_init(void);
extern PRESTO_TID_T presto_create_task( void (*func)(void), BYTE * stack, short stack_size, BYTE priority );
extern void presto_start_scheduler(void);

// clock
extern void presto_get_clock(PRESTO_TIME_T * clk);

// waiting / status flags
extern KERNEL_FLAG_T presto_wait(KERNEL_FLAG_T flags);
extern void presto_flag_clear(KERNEL_FLAG_T flag);
extern void presto_flag_set(KERNEL_TCB_T * tcb_p, KERNEL_FLAG_T flag);


////////////////////////////////////////////////////////////////////////////////
//   M A I L
////////////////////////////////////////////////////////////////////////////////

void presto_mail_init(PRESTO_MAILBOX_T * box_p, KERNEL_FLAG_T flag);
KERNEL_MSGID_T presto_mail_send(PRESTO_MAILBOX_T * box_p, PRESTO_MAIL_T payload);
BOOLEAN presto_mail_waiting(PRESTO_MAILBOX_T * box_p);
void presto_mail_wait(PRESTO_MAILBOX_T * box_p, PRESTO_MAIL_T * payload_p);
BOOLEAN presto_mail_get(PRESTO_MAILBOX_T * box_p, PRESTO_MAIL_T * payload_p);

////////////////////////////////////////////////////////////////////////////////
//   T I M E R S
////////////////////////////////////////////////////////////////////////////////

void presto_timer(PRESTO_TIMER_T * timer_p, KERNEL_INTERVAL_T delay, KERNEL_INTERVAL_T period, KERNEL_FLAG_T flag);

////////////////////////////////////////////////////////////////////////////////

#endif

