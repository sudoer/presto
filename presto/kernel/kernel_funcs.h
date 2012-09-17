
#ifndef _KERNEL_FUNCS_H_
#define _KERNEL_FUNCS_H_

////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "kernel/clock.h"
#include "kernel/kernel_types.h"
#include "kernel/mail_types.h"
#include "kernel/timer_types.h"

////////////////////////////////////////////////////////////////////////////////
//   K E R N E L - O N L Y   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

extern void kernel_trigger_set(KERNEL_TCB_T * tcb_p, KERNEL_TRIGGER_T trigger);
extern void kernel_priority_override(KERNEL_TCB_T * tcb_p, KERNEL_PRIORITY_T new_priority);
extern void kernel_priority_restore(KERNEL_TCB_T * tcb_p);

////////////////////////////////////////////////////////////////////////////////

#endif
