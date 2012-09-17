
#ifndef _SEM_TYPES_H_
#define _SEM_TYPES_H_

////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "kernel/kernel_types.h"

////////////////////////////////////////////////////////////////////////////////

#define MAX_SEM_USERS  10

////////////////////////////////////////////////////////////////////////////////

typedef struct KERNEL_SEMUSER_S {
   KERNEL_TCB_T * tcb_p;
   KERNEL_TRIGGER_T trigger;
   struct KERNEL_SEMUSER_S * next;
} KERNEL_SEMUSER_T;

////////////////////////////////////////////////////////////////////////////////

typedef struct KERNEL_SEMAPHORE_S {
   signed int max_resources;
   signed int available_resources;
   BOOLEAN use_inheritance;
   //KERNEL_TCB_T * user;
   struct KERNEL_SEMUSER_S * user_list;
   struct KERNEL_SEMUSER_S * wait_list;
   struct KERNEL_SEMUSER_S * free_list;
   struct KERNEL_SEMUSER_S semuser_data[MAX_SEM_USERS];
} KERNEL_SEMAPHORE_T;

////////////////////////////////////////////////////////////////////////////////

#endif

