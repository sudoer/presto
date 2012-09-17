
#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "presto.h"
#include "types.h"

////////////////////////////////////////////////////////////////////////////////

#define MAX_TASKS            (MAX_USER_TASKS+1)
#define MAX_MESSAGES         100

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

// These variables are used to pass stuff back and forth between the C functions
// and assembly functions.

extern BYTE * presto_asm_new_sp;
extern BYTE ** presto_asm_old_sp_p;
extern void (*presto_asm_new_fn)(void);
extern BYTE presto_asm_swap;

////////////////////////////////////////////////////////////////////////////////
/*
// ASM functions for kernel, called by C functions

#pragma interrupt_handler presto_system_isr
extern void presto_system_isr(void);
extern void presto_start_task_switching(void);
extern void presto_switch_tasks(void);
extern void presto_setup_new_task(void);
extern void presto_swap_within_isr(void);

////////////////////////////////////////////////////////////////////////////////

// C functions for kernel, called by ASM functions

extern void presto_service_timer_interrupt(void);
*/
////////////////////////////////////////////////////////////////////////////////

// used in kernel, as well as in system.c

extern void presto_fatal_error(void);

////////////////////////////////////////////////////////////////////////////////

#endif

