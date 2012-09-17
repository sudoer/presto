#ifndef _MEMORY_H_
#define _MEMORY_H_

////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "kernel/kernel.h"

////////////////////////////////////////////////////////////////////////////////

typedef struct KERNEL_MEMORYPOOLINFO_S {
   unsigned short mempool_num_items;
   unsigned short mempool_item_size;
   unsigned short current_used_items;
   #ifdef FEATURE_MEMORY_STATISTICS
      unsigned short current_total_bytes;
      unsigned short max_used_items;
      unsigned short max_requested_size;
   #endif // FEATURE_MEMORY_STATISTICS
} KERNEL_MEMORYPOOLINFO_T;

////////////////////////////////////////////////////////////////////////////////

extern void kernel_memory_init(void);
extern void memory_debug(unsigned short pool, KERNEL_MEMORYPOOLINFO_T * stats);

////////////////////////////////////////////////////////////////////////////////

#endif // _MEMORY_H_



