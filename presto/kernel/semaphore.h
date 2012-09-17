
#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "configure.h"
#include "kernel/kernel.h"

////////////////////////////////////////////////////////////////////////////////

typedef struct KERNEL_SEMUSER_S {
   KERNEL_TASKID_T tid;
   KERNEL_TRIGGER_T trigger;
   KERNEL_PRIORITY_T natural_priority;
   struct KERNEL_SEMUSER_S * next;
} KERNEL_SEMUSER_T;

////////////////////////////////////////////////////////////////////////////////

typedef struct KERNEL_SEMAPHORE_S {
   signed int max_resources;
   signed int available_resources;
   #ifdef FEATURE_SEMAPHORE_PRIORITYINHERITANCE
      BOOLEAN use_inheritance;
   #endif
   struct KERNEL_SEMUSER_S * user_list;
   struct KERNEL_SEMUSER_S * wait_list;
   struct KERNEL_SEMUSER_S * free_list;
   struct KERNEL_SEMUSER_S semuser_data[PRESTO_SEM_WAITLIST];
} KERNEL_SEMAPHORE_T;

////////////////////////////////////////////////////////////////////////////////

extern void kernel_semaphore_init(void);

////////////////////////////////////////////////////////////////////////////////

#endif

