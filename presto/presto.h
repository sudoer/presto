#ifndef _PRESTO_H_
#define _PRESTO_H_

////////////////////////////////////////////////////////////////////////////////

// This is the main header file for the operating system.  Below you
// will find all of the constants, type declarations and function
// prototypes necessary for basic multitasking.

////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "error.h"
#include "kernel/clock.h"
#include "kernel/kernel_types.h"
#include "kernel/mail_types.h"
#include "kernel/timer_types.h"
#include "kernel/sem_types.h"

////////////////////////////////////////////////////////////////////////////////

// core kernel
typedef KERNEL_TID_T        PRESTO_TID_T;
typedef KERNEL_TRIGGER_T    PRESTO_TRIGGER_T;
typedef KERNEL_PRIORITY_T   PRESTO_PRIORITY_T;

// time
typedef KERNEL_TIME_T       PRESTO_TIME_T;
typedef KERNEL_INTERVAL_T   PRESTO_INTERVAL_T;

// mail
typedef KERNEL_MAILBOX_T    PRESTO_MAILBOX_T;
typedef KERNEL_MAIL_T       PRESTO_MAIL_T;

// timer
typedef KERNEL_TIMER_T      PRESTO_TIMER_T;

// semaphore
typedef KERNEL_SEMAPHORE_T  PRESTO_SEMAPHORE_T;


////////////////////////////////////////////////////////////////////////////////
//   K E R N E L
////////////////////////////////////////////////////////////////////////////////

// initialization
extern void presto_init(void);
extern PRESTO_TID_T presto_create_task( void (*func)(void), BYTE * stack, short stack_size, KERNEL_PRIORITY_T priority );
extern void presto_start_scheduler(void);

// clock
extern void presto_get_clock(PRESTO_TIME_T * clk);

// waiting / status triggers
extern KERNEL_TRIGGER_T presto_wait(KERNEL_TRIGGER_T triggers);
extern KERNEL_TRIGGER_T presto_trigger_poll(KERNEL_TRIGGER_T test);
extern void presto_trigger_set(KERNEL_TRIGGER_T trigger);
extern void presto_trigger_clear(KERNEL_TRIGGER_T trigger);
extern void presto_trigger_send(KERNEL_TID_T tid, KERNEL_TRIGGER_T trigger);

////////////////////////////////////////////////////////////////////////////////
//   M A I L
////////////////////////////////////////////////////////////////////////////////

extern void presto_mailbox_init(PRESTO_MAILBOX_T * box_p, KERNEL_TRIGGER_T trigger);
extern void presto_mail_send(PRESTO_MAILBOX_T * box_p, PRESTO_MAIL_T payload);
extern BOOLEAN presto_mail_get(PRESTO_MAILBOX_T * box_p, PRESTO_MAIL_T * payload_p);
extern void presto_mail_wait(PRESTO_MAILBOX_T * box_p, PRESTO_MAIL_T * payload_p);
/*
extern BOOLEAN presto_mail_test(PRESTO_MAILBOX_T * box_p);
*/

////////////////////////////////////////////////////////////////////////////////
//   T I M E R S
////////////////////////////////////////////////////////////////////////////////

extern void presto_timer_start(PRESTO_TIMER_T * timer_p, KERNEL_INTERVAL_T delay, KERNEL_INTERVAL_T period, KERNEL_TRIGGER_T trigger);
extern void presto_timer_wait(KERNEL_INTERVAL_T delay, KERNEL_TRIGGER_T trigger);
extern void presto_timer_disable(PRESTO_TIMER_T * timer_p);

////////////////////////////////////////////////////////////////////////////////
//   S E M A P H O R E S
////////////////////////////////////////////////////////////////////////////////

extern void presto_semaphore_init(PRESTO_SEMAPHORE_T * sem_p, short resources, BOOLEAN use_inheritance);
extern void presto_semaphore_request(PRESTO_SEMAPHORE_T * sem_p, KERNEL_TRIGGER_T trigger);
extern void presto_semaphore_release(PRESTO_SEMAPHORE_T * sem_p);
extern void presto_semaphore_wait(PRESTO_SEMAPHORE_T * sem_p, KERNEL_TRIGGER_T trigger);
/*
extern BOOLEAN presto_semaphore_test(PRESTO_SEMAPHORE_T * sem_p);
*/

////////////////////////////////////////////////////////////////////////////////
//   E R R O R   H A N D L I N G
////////////////////////////////////////////////////////////////////////////////

extern void presto_fatal_error(error_number_e err);
extern void presto_crash_address(unsigned short address);
extern void presto_crash(void);

////////////////////////////////////////////////////////////////////////////////

#endif

