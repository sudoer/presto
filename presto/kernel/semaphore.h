
#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "configure.h"
#include "kernel/kernel.h"

////////////////////////////////////////////////////////////////////////////////

typedef struct KERNEL_SEMAPHORE_LOCK_S {
   KERNEL_TASKID_T tid;
   KERNEL_PRIORITY_T natural_priority;
   KERNEL_TRIGGER_T trigger;
   struct KERNEL_SEMAPHORE_RESOURCE_S * resource_p;
   struct KERNEL_SEMAPHORE_LOCK_S * next;
} KERNEL_SEMLOCK_T;

////////////////////////////////////////////////////////////////////////////////

typedef struct KERNEL_SEMAPHORE_RESOURCE_S {
   signed int num_locks;
   KERNEL_PRIORITY_T highest_priority;
   signed int max_resources;
   signed int available_resources;
   int inheritance_type;
   struct KERNEL_SEMAPHORE_LOCK_S * user_list;
   struct KERNEL_SEMAPHORE_LOCK_S * wait_list;
} KERNEL_SEMRESOURCE_T;

////////////////////////////////////////////////////////////////////////////////

extern void kernel_semaphore_init(void);

////////////////////////////////////////////////////////////////////////////////

#endif // _SEMAPHORE_H_

