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

typedef union {
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
BYTE presto_timer(PRESTO_TID_T to, unsigned short delay, PRESTO_MAIL_T payload);
BYTE presto_send_message(PRESTO_TID_T to, PRESTO_MAIL_T payload);
BYTE presto_mail_waiting(void);
BYTE presto_wait_for_message(PRESTO_MAIL_T * payload_p);


////////////////////////////////////////////////////////////////////////////////

#ifndef INTR_ON
#define INTR_ON() asm("cli");
#define INTR_OFF() asm("sei");
#endif

////////////////////////////////////////////////////////////////////////////////

#endif

