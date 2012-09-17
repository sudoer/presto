////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////

// This module implements a simple memory pool for fast dynamic memory
// allocation.  It is flexible -- it can be configured to use several
// pools of fixed sized "items" by changing a handful of constants in
// configure.h.

// There are two main data structures involved with keeping track of
// memory allocation.  There are "pool" structures, which keep high level
// information about a set of similar memory items (how big are the items,
// how many are currently used, etc).  And then there are the memory items
// themselves, which each keep track of one block of allocated memory.

// The memory item structres are stored interleaved with the actual memory
// that is allocated.  This makes it possible to find a specific memory
// item, given only a pointer to the allocated memory.

// Here is a simple example of the data strctures.
// mempools: { 5 items, 3 bytes each, A-E }, { 4 items, 7 bytes each, F-I }
// membytes: A111B222C333D444E555F6666666G7777777H8888888I9999999
//
// In this example, there is a memory item A which precedes three bytes of
// allocatable memory.  The structure in A contains information about those
// three bytes: is it being used, which pool does it belong to, how many
// bytes were actually requested (less than three?).  When the user asks
// for up to three bytes, he is given a pointer to the "111" area.  When
// he returns the memory to the pool, he gives us back the same pointer.
// Using this pointer, we can go backwards a few bytes and see the structure
// A, and we can return the memory to the pool where it belongs.

////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "presto.h"
#include "configure.h"
#include "cpu/error.h"
#include "cpu/locks.h"
#include "kernel/kernel.h"
#include "kernel/memory.h"

////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////

#define SPECIAL_MARKER  0x55

////////////////////////////////////////////////////////////////////////////////
//   D A T A   T Y P E S
////////////////////////////////////////////////////////////////////////////////

typedef struct MEMORY_ITEM_S {
   /* short debug_item_number; */
   short requested_bytes;
   // We never us 'next' and 'pool' at the same time, so we use a union.
   union {
      struct MEMORY_ITEM_S * next;
      struct MEMORY_POOL_S * pool;
   } point;
} MEMORY_ITEM_T;

typedef struct MEMORY_POOL_S {
   short mempool_num_items;
   short mempool_item_size;
   short current_used_items;
   MEMORY_ITEM_T * free_list;
   #ifdef FEATURE_MEMORY_STATISTICS
      short current_total_bytes;
      short max_used_items;
      short max_requested_size;
   #endif
} MEMORY_POOL_T;

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N   P R O T O T Y P E S
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   G L O B A L   D A T A
////////////////////////////////////////////////////////////////////////////////

static MEMORY_POOL_T mempools[PRESTO_MEM_NUMPOOLS]
   __attribute((section(".heap")));

static BYTE membytes[PRESTO_MEM_TOTALBYTES+(PRESTO_MEM_NUM_ITEMS*sizeof(MEMORY_ITEM_T))]
   __attribute((section(".heap")));


////////////////////////////////////////////////////////////////////////////////
//   E X T E R N A L   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


BYTE * presto_memory_allocate(unsigned short requested_bytes) {
   short pool;

   if (requested_bytes==0) return NULL;

   for (pool=0;pool<PRESTO_MEM_NUMPOOLS;pool++) {
      MEMORY_POOL_T * pool_ptr;
      pool_ptr=&mempools[pool];
      if (pool_ptr->mempool_item_size>=requested_bytes) {
         if (pool_ptr->free_list!=NULL) {
            CPU_LOCK_T lock;
            MEMORY_ITEM_T * item_ptr;
            BYTE * mem_ptr;
            cpu_lock_save(lock);
            // remove memory item from free list
            item_ptr=pool_ptr->free_list;
            pool_ptr->free_list=pool_ptr->free_list->point.next;
            // update memory pool
            pool_ptr->current_used_items++;
            cpu_unlock_restore(lock);
            // keep a pointer back to the pool... we'll need it later
            item_ptr->point.pool=&mempools[pool];
            // statistics for item usage
            item_ptr->requested_bytes=requested_bytes;
            // this is the pointer to the actual memory area
            mem_ptr=(BYTE *)item_ptr+sizeof(MEMORY_ITEM_T);
            #ifdef FEATURE_MEMORY_STATISTICS
               if (pool_ptr->current_used_items > pool_ptr->max_used_items) {
                  pool_ptr->max_used_items=pool_ptr->current_used_items;
               }
               if (requested_bytes > pool_ptr->max_requested_size) {
                  pool_ptr->max_requested_size=requested_bytes;
               }
               pool_ptr->current_total_bytes+=requested_bytes;
            #endif
            #ifdef SANITYCHECK_MEMORY_WROTETOOFAR
               // write a pattern to memory... will be checked on free
               if (requested_bytes < pool_ptr->mempool_item_size) {
                  *(mem_ptr+requested_bytes)=SPECIAL_MARKER;
               }
            #endif
            return (BYTE *)item_ptr+sizeof(MEMORY_ITEM_T);
         }
      }
   }

   return NULL;
}


////////////////////////////////////////////////////////////////////////////////


void presto_memory_free(BYTE * free_me) {
   CPU_LOCK_T lock;
   MEMORY_POOL_T * pool_ptr;
   MEMORY_ITEM_T * item_ptr;

   // special case -- freeing memory that they never got
   if (free_me==NULL) return;

   // find the memory item structure associated with this memory
   item_ptr=(MEMORY_ITEM_T *)(free_me-sizeof(MEMORY_ITEM_T));
   // determine which memory pool this item came from
   pool_ptr=item_ptr->point.pool;

   #ifdef SANITYCHECK_MEMORY_WROTETOOFAR
       if (item_ptr->requested_bytes < pool_ptr->mempool_item_size) {
          if (*(free_me+item_ptr->requested_bytes) != SPECIAL_MARKER ) {
             error_fatal(ERROR_MEMORY_CLOBBEREDMEMBLOCK);
          }
       }
   #endif

   cpu_lock_save(lock);
   // add memory item to free list
   item_ptr->point.next=pool_ptr->free_list;
   pool_ptr->free_list=item_ptr;
   // update memory pool
   pool_ptr->current_used_items--;
   cpu_unlock_restore(lock);

   #ifdef FEATURE_MEMORY_STATISTICS
      pool_ptr->current_total_bytes-=item_ptr->requested_bytes;
   #endif
   // show that this block is unused
   item_ptr->requested_bytes=0;
}


////////////////////////////////////////////////////////////////////////////////


void memory_debug(int pool, KERNEL_MEMORYPOOLSTATS_T * stats) {
   if((pool<0)||(pool>=PRESTO_MEM_NUMPOOLS)) return;
   stats->mempool_num_items=mempools[pool].mempool_num_items;
   stats->mempool_item_size=mempools[pool].mempool_item_size;
   stats->current_used_items=mempools[pool].current_used_items;
   #ifdef FEATURE_MEMORY_STATISTICS
      stats->current_total_bytes=mempools[pool].current_total_bytes;
      stats->max_used_items=mempools[pool].max_used_items;
      stats->max_requested_size=mempools[pool].max_requested_size;
   #endif
}


////////////////////////////////////////////////////////////////////////////////
//   K E R N E L - O N L Y   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


void kernel_memory_init(void) {
   short mempools_sizes[PRESTO_MEM_NUMPOOLS]= PRESTO_MEM_POOL_SIZES;
   short mempools_qtys[PRESTO_MEM_NUMPOOLS]=  PRESTO_MEM_POOL_QTYS;

   short pool;
   /* short debug=0; */
   BYTE * mem_ptr;

   mem_ptr=membytes;
   for (pool=0;pool<PRESTO_MEM_NUMPOOLS;pool++) {
      MEMORY_POOL_T * pool_ptr;
      short i;

      // we're going to use this a lot...
      pool_ptr=&mempools[pool];

      // fill in administrative areas of pool
      pool_ptr->mempool_num_items=mempools_qtys[pool];
      pool_ptr->mempool_item_size=mempools_sizes[pool];
      pool_ptr->current_used_items=0;
      #ifdef FEATURE_MEMORY_STATISTICS
         pool_ptr->max_used_items=0;
         pool_ptr->max_requested_size=0;
         pool_ptr->current_total_bytes=0;
      #endif

      // build the free list
      mempools[pool].free_list=(MEMORY_ITEM_T *)mem_ptr;
      for (i=pool_ptr->mempool_num_items;i>0;i--) {
         MEMORY_ITEM_T * item_ptr;

         // record the address of the memory item
         item_ptr=(MEMORY_ITEM_T *)mem_ptr;

         // fill in the details in the memory item header
         /* item_ptr->debug_item_number=debug++; */
         item_ptr->requested_bytes=0;

         // calculate and record the start of user memory
         mem_ptr+=sizeof(MEMORY_ITEM_T);
         // item_ptr->memory=mem_ptr; is implied

         // calculate the position of the next item header
         mem_ptr+=pool_ptr->mempool_item_size;

         // finish off the item by assigning the 'next' pointer
         if (i==1) item_ptr->point.next=NULL;
         else item_ptr->point.next=(MEMORY_ITEM_T *)mem_ptr;
      }
   }
}


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////























