#ifndef _MEMORY_H_
#define _MEMORY_H_

////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "kernel/kernel.h"

////////////////////////////////////////////////////////////////////////////////

typedef struct KERNEL_MEMORYPOOLSTATS_S {
   short mempool_num_items;
   short mempool_item_size;
   short current_used_items;
   #ifdef FEATURE_MEMORY_STATISTICS
      short current_total_bytes;
      short max_used_items;
      short max_requested_size;
   #endif
} KERNEL_MEMORYPOOLSTATS_T;

////////////////////////////////////////////////////////////////////////////////

extern void kernel_memory_init(void);
extern void memory_debug(int pool, KERNEL_MEMORYPOOLSTATS_T * stats);

////////////////////////////////////////////////////////////////////////////////

#endif



