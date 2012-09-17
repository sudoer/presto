
#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "configure.h"
#include "kernel/kernel.h"

////////////////////////////////////////////////////////////////////////////////

typedef struct KERNEL_SEMAPHORE_LOCK_S {
   KERNEL_TASKID_T task_id;
   KERNEL_TRIGGER_T trigger;
   KERNEL_PRIORITY_T natural_priority;
   struct KERNEL_SEMAPHORE_LOCK_S * next;
} KERNEL_SEMUSER_T;

////////////////////////////////////////////////////////////////////////////////

typedef struct KERNEL_SEMAPHORE_RESOURCE_S {
   int num_users;
   int max_resources;
   int available_resources;
   int inheritance_type;   // actually an enumeration
   //KERNEL_PRIORITY_T highest_priority;
   struct KERNEL_SEMAPHORE_LOCK_S * user_list;
   struct KERNEL_SEMAPHORE_LOCK_S * wait_list;
   struct KERNEL_SEMAPHORE_LOCK_S * free_list;
} KERNEL_SEMAPHORE_T;

////////////////////////////////////////////////////////////////////////////////

extern void kernel_semaphore_init(void);

////////////////////////////////////////////////////////////////////////////////

#endif // _SEMAPHORE_H_

