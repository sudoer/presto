
#ifndef _KERNEL_H_
#define _KERNEL_H_

////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "kernel/clock.h"

////////////////////////////////////////////////////////////////////////////////
//   D E B U G G I N G
////////////////////////////////////////////////////////////////////////////////

#define CHECK_STACK_CLOBBERING
#define PARANOID

////////////////////////////////////////////////////////////////////////////////
//   C O N F I G U R A T I O N
////////////////////////////////////////////////////////////////////////////////

// TASKS
#define MAX_USER_TASKS       12

// MAIL MESSAGES
#define MAX_MESSAGES         40

// TIMING
#define CYCLES_PER_MS       2000        // based on hardware
#define CYCLES_PER_CLOCK    2           // based on hardware
#define CLOCK_PRESCALE      16          // set in TMSK2 register
#define MS_PER_TICK         5           // how often do you want?

////////////////////////////////////////////////////////////////////////////////
//   S I M P L E   D A T A   T Y P E S
////////////////////////////////////////////////////////////////////////////////

typedef signed char    KERNEL_TID_T;
typedef signed char    KERNEL_MSGID_T;
typedef unsigned char  KERNEL_FLAG_T;
typedef unsigned short KERNEL_INTERVAL_T;

////////////////////////////////////////////////////////////////////////////////
//   C O M P L E X   D A T A   T Y P E S
////////////////////////////////////////////////////////////////////////////////

typedef struct KERNEL_TCB_S {
   KERNEL_TID_T task_id;
   BYTE * stack_ptr;
   BYTE * stack_top;
   BYTE * stack_bottom;
   BYTE priority;
   BOOLEAN in_use;
   KERNEL_FLAG_T wait_mask;
   KERNEL_FLAG_T flags;
   struct KERNEL_TCB_S * next;
} KERNEL_TCB_T;

////////////////////////////////////////////////////////////////////////////////

typedef struct KERNEL_TIMER_S {
   struct KERNEL_TIME_S delivery_time;
   KERNEL_INTERVAL_T timer_period;
   struct KERNEL_TCB_S * owner_tcb_p;
   KERNEL_FLAG_T trigger_flag;
   struct KERNEL_TIMER_S * next;
} KERNEL_TIMER_T;

////////////////////////////////////////////////////////////////////////////////

typedef struct KERNEL_MAILBOX_S {
   unsigned short message_count;
   struct KERNEL_MESSAGE_S * mailbox_head;
   struct KERNEL_MESSAGE_S * mailbox_tail;
   struct KERNEL_TCB_S * owner_tcb_p;
   KERNEL_FLAG_T trigger_flag;
} KERNEL_MAILBOX_T;

////////////////////////////////////////////////////////////////////////////////
//   K E R N E L - O N L Y   D A T A
///////////////////////////////////////////////////////////////////////////////

extern KERNEL_TCB_T * current_tcb_p;
extern KERNEL_TIMER_T * kernel_timer_list;

////////////////////////////////////////////////////////////////////////////////
//   K E R N E L - O N L Y   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

extern void kernel_mail_init(void);
extern BYTE kernel_timer_poll(void);
extern void kernel_flag_set(KERNEL_TCB_T * tcb_p, KERNEL_FLAG_T flag);
extern KERNEL_TIME_T kernel_clock;

////////////////////////////////////////////////////////////////////////////////

#endif

