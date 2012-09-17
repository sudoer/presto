
#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_

////////////////////////////////////////////////////////////////////////////////
//   O P T I O N A L   F E A T U R E S
////////////////////////////////////////////////////////////////////////////////

#define FEATURE_DEBUG

#ifdef CPU_M68HC11
   #define FEATURE_KERNEL_MAIL
   #define FEATURE_KERNEL_TIMER
   #define FEATURE_KERNEL_MEMORY
   #define FEATURE_KERNEL_SEMAPHORE
   #define FEATURE_MEMORY_STATISTICS
   #define FEATURE_SEMAPHORE_PRIORITYINHERITANCE
   #define FEATURE_MAIL_NOSTEALING
   #define SANITYCHECK_KERNEL_CLOBBEREDSTACK
   #define SANITYCHECK_MEMORY_WROTETOOFAR
#endif
#ifdef CPU_AVR8515
   //#define FEATURE_KERNEL_MAIL
   #define FEATURE_KERNEL_TIMER
   //#define FEATURE_KERNEL_MEMORY
   //#define FEATURE_KERNEL_SEMAPHORE
   //#define FEATURE_MEMORY_STATISTICS
   //#define FEATURE_SEMAPHORE_PRIORITYINHERITANCE
   //#define FEATURE_MAIL_NOSTEALING
   //#define SANITYCHECK_KERNEL_CLOBBEREDSTACK
   //#define SANITYCHECK_MEMORY_WROTETOOFAR
#endif

////////////////////////////////////////////////////////////////////////////////
//   K E R N E L   C O N F I G U R A T I O N
////////////////////////////////////////////////////////////////////////////////

// INITIAL STACK
#ifdef CPU_M68HC11
   #define BOOT_INITIALSTACKSIZE            64
#endif
#ifdef CPU_AVR8515
   #define BOOT_INITIALSTACKSIZE            64
#endif

// IDLE STACK
#ifdef CPU_M68HC11
   #define PRESTO_KERNEL_IDLESTACKSIZE      64
#endif
#ifdef CPU_AVR8515
   #define PRESTO_KERNEL_IDLESTACKSIZE      64
#endif

// TASKS
#define PRESTO_KERNEL_MAXUSERTASKS           8

// TRIGGERS
#define PRESTO_KERNEL_TRIGGERBITS            8

////////////////////////////////////////////////////////////////////////////////
//   M A I L   M E S S A G E S
////////////////////////////////////////////////////////////////////////////////

// no configuration options for mail

////////////////////////////////////////////////////////////////////////////////
//   T I M E R S
////////////////////////////////////////////////////////////////////////////////

#define PRESTO_KERNEL_MSPERTICK              1

////////////////////////////////////////////////////////////////////////////////
//   S E M A P H O R E S
////////////////////////////////////////////////////////////////////////////////

#define PRESTO_SEM_WAITLIST                  3

////////////////////////////////////////////////////////////////////////////////
//   M E M O R Y   P O O L S
////////////////////////////////////////////////////////////////////////////////

#define PRESTO_MEM_NUMPOOLS                  2

// The memory pools should be defined in increasing size order.
// For each pool, there is a SIZE and a QTY.  That is, there are
// QTY number of SIZE-sized items in a single pool.

#ifdef CPU_M68HC11
   #define PRESTO_MEM_ITEMSIZ1              16
   #define PRESTO_MEM_ITEMQTY1               8

   #define PRESTO_MEM_ITEMSIZ2              24
   #define PRESTO_MEM_ITEMQTY2              12
#endif
#ifdef CPU_AVR8515
   #define PRESTO_MEM_ITEMSIZ1              16
   #define PRESTO_MEM_ITEMQTY1               2

   #define PRESTO_MEM_ITEMSIZ2              20
   #define PRESTO_MEM_ITEMQTY2               0
#endif


// These should be updated to reflect the number of memory pools.

#define PRESTO_MEM_POOL_SIZES  { PRESTO_MEM_ITEMSIZ1, PRESTO_MEM_ITEMSIZ2 }
#define PRESTO_MEM_POOL_QTYS   { PRESTO_MEM_ITEMQTY1, PRESTO_MEM_ITEMQTY2 }
#define PRESTO_MEM_NUM_ITEMS   ( PRESTO_MEM_ITEMQTY1+ PRESTO_MEM_ITEMQTY2 )

#define PRESTO_MEM_TOTALBYTES (( PRESTO_MEM_ITEMQTY1 * PRESTO_MEM_ITEMSIZ1 )+ \
                               ( PRESTO_MEM_ITEMQTY2 * PRESTO_MEM_ITEMSIZ2 ))

////////////////////////////////////////////////////////////////////////////////

#endif

