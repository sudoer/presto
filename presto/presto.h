#ifndef _PRESTO_H_
#define _PRESTO_H_

////////////////////////////////////////////////////////////////////////////////

// This is the main header file for the presto operating system.
// Below you will find the complete API for the O/S.  You should
// not have to include any of the header files from the "kernel"
// directory in your application.

////////////////////////////////////////////////////////////////////////////////

#include "configure.h"
#include "types.h"
#include "error.h"
#include "kernel/clock.h"
#include "kernel/kernel.h"
#include "kernel/mail.h"
#include "kernel/timer.h"
#include "kernel/semaphore.h"

////////////////////////////////////////////////////////////////////////////////

// These are the data types that you will use in your application.
// Use the ones that begin with "PRESTO".

// core kernel
typedef KERNEL_TASKID_T       PRESTO_TASKID_T;
typedef KERNEL_TRIGGER_T      PRESTO_TRIGGER_T;
typedef KERNEL_PRIORITY_T     PRESTO_PRIORITY_T;

// time
typedef KERNEL_INTERVAL_T     PRESTO_INTERVAL_T;


////////////////////////////////////////////////////////////////////////////////
//   S T A R T - U P
////////////////////////////////////////////////////////////////////////////////

extern void presto_init(void);
extern PRESTO_TASKID_T presto_task_create(void (*func)(void), BYTE * stack, short stack_size, PRESTO_PRIORITY_T priority);
extern void presto_scheduler_start(void);

////////////////////////////////////////////////////////////////////////////////
//   T A S K   P R I O R I T I E S
////////////////////////////////////////////////////////////////////////////////

extern PRESTO_PRIORITY_T presto_priority_get(PRESTO_TASKID_T tid);
extern void presto_priority_set(PRESTO_TASKID_T tid, PRESTO_PRIORITY_T new_priority);
extern void presto_priority_override(PRESTO_TASKID_T tid, PRESTO_PRIORITY_T new_priority);
extern void presto_priority_restore(PRESTO_TASKID_T tid);

////////////////////////////////////////////////////////////////////////////////
//   T R I G G E R S
////////////////////////////////////////////////////////////////////////////////

// This function could also be called "presto_trigger_wait", but
// since it is used everywhere, we simply call it "presto_wait".
extern PRESTO_TRIGGER_T presto_wait(PRESTO_TRIGGER_T triggers);

extern void presto_trigger_set(PRESTO_TRIGGER_T trigger);
extern void presto_trigger_clear(PRESTO_TRIGGER_T trigger);
extern void presto_trigger_send(PRESTO_TASKID_T tid, PRESTO_TRIGGER_T trigger);
extern PRESTO_TRIGGER_T presto_trigger_poll(PRESTO_TRIGGER_T test);

////////////////////////////////////////////////////////////////////////////////
//   M A I L
////////////////////////////////////////////////////////////////////////////////

#ifdef FEATURE_KERNEL_MAIL

   typedef KERNEL_MAILBOX_T      PRESTO_MAILBOX_T;
   typedef KERNEL_ENVELOPE_T     PRESTO_ENVELOPE_T;
   typedef KERNEL_MAILMSG_T      PRESTO_MAILMSG_T;
   typedef KERNEL_MAILPTR_T      PRESTO_MAILPTR_T;

   // mailboxes, etc
   extern void presto_mailbox_init(PRESTO_MAILBOX_T * box_p, PRESTO_TRIGGER_T trigger);
   extern void presto_mailbox_default(PRESTO_MAILBOX_T * box_p);

   // envelopes
   extern PRESTO_MAILMSG_T presto_envelope_message(PRESTO_ENVELOPE_T * env_p);
   extern PRESTO_MAILPTR_T presto_envelope_payload(PRESTO_ENVELOPE_T * env_p);
   extern PRESTO_TASKID_T presto_envelope_sender(PRESTO_ENVELOPE_T * env_p);

   // sending and receiving
   extern void presto_mail_send_to_box(PRESTO_MAILBOX_T * box_p, PRESTO_ENVELOPE_T * env_p, PRESTO_MAILMSG_T message, PRESTO_MAILPTR_T payload);
   extern BOOLEAN presto_mail_send_to_task(PRESTO_TASKID_T tid, PRESTO_ENVELOPE_T * env_p, PRESTO_MAILMSG_T message, PRESTO_MAILPTR_T payload);
   extern PRESTO_ENVELOPE_T * presto_mail_get(PRESTO_MAILBOX_T * box_p);
   extern PRESTO_ENVELOPE_T * presto_mail_wait(PRESTO_MAILBOX_T * box_p);

#endif

////////////////////////////////////////////////////////////////////////////////
//   T I M E R S
////////////////////////////////////////////////////////////////////////////////

#ifdef FEATURE_KERNEL_TIMER

   typedef KERNEL_TIMER_T        PRESTO_TIMER_T;

   extern void presto_timer_start(PRESTO_TIMER_T * timer_p, PRESTO_INTERVAL_T delay, PRESTO_INTERVAL_T period, PRESTO_TRIGGER_T trigger);
   extern void presto_timer_wait(PRESTO_INTERVAL_T delay, PRESTO_TRIGGER_T trigger);
   extern void presto_timer_stop(PRESTO_TIMER_T * timer_p);

#endif

////////////////////////////////////////////////////////////////////////////////
//   S E M A P H O R E S
////////////////////////////////////////////////////////////////////////////////

#ifdef FEATURE_KERNEL_SEMAPHORE

   typedef KERNEL_SEMAPHORE_T    PRESTO_SEMAPHORE_T;

   #ifdef FEATURE_SEMAPHORE_PRIORITYINHERITANCE
      extern void presto_semaphore_init(PRESTO_SEMAPHORE_T * sem_p, short resources, BOOLEAN use_inheritance);
   #else
      extern void presto_semaphore_init(PRESTO_SEMAPHORE_T * sem_p, short resources);
   #endif
   extern BOOLEAN presto_semaphore_request(PRESTO_SEMAPHORE_T * sem_p, PRESTO_TRIGGER_T trigger);
   extern void presto_semaphore_release(PRESTO_SEMAPHORE_T * sem_p);
   extern void presto_semaphore_wait(PRESTO_SEMAPHORE_T * sem_p, PRESTO_TRIGGER_T trigger);

#endif

////////////////////////////////////////////////////////////////////////////////
//   M E M O R Y
////////////////////////////////////////////////////////////////////////////////

#ifdef FEATURE_KERNEL_MEMORY

   extern BYTE * presto_memory_allocate(unsigned short requested_bytes);
   extern void presto_memory_free(BYTE * free_me);

#endif

////////////////////////////////////////////////////////////////////////////////

#endif

