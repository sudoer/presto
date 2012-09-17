////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////


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


#include "services/serial.h"
#include "services/string.h"


////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//   D A T A   T Y P E S
////////////////////////////////////////////////////////////////////////////////

typedef struct MEMORY_ITEM_S {
   short debug_item_number;
   short amount_requested;
   union {
      struct MEMORY_ITEM_S * next;
      struct MEMORY_POOL_S * pool;
   } point;
} MEMORY_ITEM_T;

typedef struct MEMORY_POOL_S {
   short items_in_pool;
   short item_size;
   short items_used;
   MEMORY_ITEM_T * free_list;
   #ifdef FEATURE_MEMORY_STATISTICS
      short total_bytes_used;
      short max_items_used;
      short max_item_size;
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
      if (pool_ptr->item_size>=requested_bytes) {
         if (pool_ptr->free_list!=NULL) {
            CPU_LOCK_T lock;
            MEMORY_ITEM_T * item_ptr;
            cpu_lock_save(lock);
            // remove memory item from free list
            item_ptr=pool_ptr->free_list;
            pool_ptr->free_list=pool_ptr->free_list->point.next;
            // keep a pointer back to the pool... we'll need it later
            item_ptr->point.pool=&mempools[pool];
            // statistics for item usage
            item_ptr->amount_requested=requested_bytes;
            // update memory pool
            pool_ptr->items_used++;
            #ifdef FEATURE_MEMORY_STATISTICS
               if (pool_ptr->items_used > pool_ptr->max_items_used) {
                  pool_ptr->max_items_used=pool_ptr->items_used;
               }
               if (requested_bytes > pool_ptr->max_item_size) {
                  pool_ptr->max_item_size=requested_bytes;
               }
               pool_ptr->total_bytes_used+=requested_bytes;
            #endif
            // TODO - write a pattern to memory?
            cpu_unlock_restore(lock);
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

   if (free_me==NULL) return;

   cpu_lock_save(lock);
   item_ptr=(MEMORY_ITEM_T *)(free_me-sizeof(MEMORY_ITEM_T));
   // determine which memory pool this item came from
   pool_ptr=item_ptr->point.pool;
   // add memory item to free list
   item_ptr->point.next=pool_ptr->free_list;
   pool_ptr->free_list=item_ptr;
   // update memory pool
   pool_ptr->items_used--;
   #ifdef FEATURE_MEMORY_STATISTICS
      pool_ptr->total_bytes_used-=item_ptr->amount_requested;
   #endif
   // show that this block is unused
   item_ptr->amount_requested=0;
   // TODO - check the pattern written in memory?
   cpu_unlock_restore(lock);
}


////////////////////////////////////////////////////////////////////////////////

#define PRT  80
void presto_memory_debug(void) {

   char prt[PRT];

   short pool;
   short count;
   MEMORY_ITEM_T * ptr;

   for (pool=0;pool<PRESTO_MEM_NUMPOOLS;pool++) {

      serial_send_string("pool=");
      string_IntegerToString(pool,prt,PRT);
      serial_send_string(prt);
      serial_send_string("\r\n");


      serial_send_string("SIZE ");
      #ifdef FEATURE_MEMORY_STATISTICS
         serial_send_string("max/");
      #endif
      serial_send_string("limit=");
      #ifdef FEATURE_MEMORY_STATISTICS
         string_IntegerToString(mempools[pool].max_item_size,prt,PRT);
         serial_send_string(prt);
         serial_send_string("/");
      #endif
      string_IntegerToString(mempools[pool].item_size,prt,PRT);
      serial_send_string(prt);
      serial_send_string("\r\n");


      serial_send_string("QTY current/");
      #ifdef FEATURE_MEMORY_STATISTICS
         serial_send_string("max/");
      #endif
      serial_send_string("limit=");
      string_IntegerToString(mempools[pool].items_used,prt,PRT);
      serial_send_string(prt);
      serial_send_string("/");
      #ifdef FEATURE_MEMORY_STATISTICS
         string_IntegerToString(mempools[pool].max_items_used,prt,PRT);
         serial_send_string(prt);
         serial_send_string("/");
      #endif
      string_IntegerToString(mempools[pool].items_in_pool,prt,PRT);
      serial_send_string(prt);
      serial_send_string("\r\n");


      #ifdef FEATURE_MEMORY_STATISTICS
         serial_send_string("total_bytes_used=");
         string_IntegerToString(mempools[pool].total_bytes_used,prt,PRT);
         serial_send_string(prt);
         serial_send_string("\r\n");
      #endif


      serial_send_string("free_list");
      serial_send_string("->");
      count=0;
      ptr=mempools[pool].free_list;
      while (ptr!=NULL) {
         string_IntegerToString(ptr->debug_item_number,prt,PRT);
         serial_send_string(prt);
         serial_send_string("->");
         ptr=ptr->point.next;
         if (++count>4) {
            serial_send_string("...->");
            ptr=NULL;
         }
      }
      serial_send_string("x\r\n");
      serial_send_string("\r\n");
   }
   serial_send_string("\r\n");

}


////////////////////////////////////////////////////////////////////////////////
//   K E R N E L - O N L Y   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


void kernel_memory_init(void) {
   short mempools_sizes[PRESTO_MEM_NUMPOOLS]= PRESTO_MEM_POOL_SIZES;
   short mempools_qtys[PRESTO_MEM_NUMPOOLS]=  PRESTO_MEM_POOL_QTYS;

   short pool;
   short debug=0;
   BYTE * mem_ptr;

   mem_ptr=membytes;
   for (pool=0;pool<PRESTO_MEM_NUMPOOLS;pool++) {
      MEMORY_POOL_T * pool_ptr;
      short i;

      // record location, for speed
      pool_ptr=&mempools[pool];

      // fill in administrative areas of pool
      pool_ptr->items_in_pool=mempools_qtys[pool];
      pool_ptr->item_size=mempools_sizes[pool];
      pool_ptr->items_used=0;
      #ifdef FEATURE_MEMORY_STATISTICS
         pool_ptr->max_items_used=0;
         pool_ptr->max_item_size=0;
         pool_ptr->total_bytes_used=0;
      #endif

      // build the free list
      mempools[pool].free_list=(MEMORY_ITEM_T *)mem_ptr;
      for (i=pool_ptr->items_in_pool;i>0;i--) {
         MEMORY_ITEM_T * item_ptr;

         // record the address of the memory item
         item_ptr=(MEMORY_ITEM_T *)mem_ptr;

         // fill in the details in the memory item header
         item_ptr->debug_item_number=debug++;
         item_ptr->amount_requested=0;

         // calculate and record the start of user memory
         mem_ptr+=sizeof(MEMORY_ITEM_T);
         // item_ptr->memory=mem_ptr; is implied

         // calculate the position of the next item header
         mem_ptr+=pool_ptr->item_size;

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























