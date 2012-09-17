
#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_

////////////////////////////////////////////////////////////////////////////////
//   O P T I O N A L   F E A T U R E S
////////////////////////////////////////////////////////////////////////////////

#define FEATURE_DEBUG

#define FEATURE_KERNEL_MAIL
#define FEATURE_KERNEL_TIMER
#define FEATURE_KERNEL_MEMORY
#define FEATURE_KERNEL_SEMAPHORE
#define FEATURE_MEMORY_STATISTICS
#define FEATURE_SEMAPHORE_PRIORITYINHERITANCE
#define FEATURE_MAIL_NOSTEALING
#define SANITY_KERNEL_CLOBBEREDSTACK
#define SANITY_MEMORY_WROTETOOFAR

////////////////////////////////////////////////////////////////////////////////
//   K E R N E L   C O N F I G U R A T I O N
////////////////////////////////////////////////////////////////////////////////

// INITIAL STACK
#define BOOT_INITIALSTACKSIZE            64

// IDLE STACK
#define PRESTO_KERNEL_IDLESTACKSIZE      64

// TASKS
#define PRESTO_KERNEL_MAXUSERTASKS       16

// TRIGGERS
#define PRESTO_KERNEL_TRIGGERBITS           16

////////////////////////////////////////////////////////////////////////////////
//   M A I L   M E S S A G E S
////////////////////////////////////////////////////////////////////////////////

// no configuration options for mail

////////////////////////////////////////////////////////////////////////////////
//   T I M E R S
////////////////////////////////////////////////////////////////////////////////

#define PRESTO_KERNEL_MSPERTICK              2

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

#define PRESTO_MEM_ITEMSIZ1              16
#define PRESTO_MEM_ITEMQTY1               8

#define PRESTO_MEM_ITEMSIZ2              24
#define PRESTO_MEM_ITEMQTY2              12


// These should be updated to reflect the number of memory pools.

#define PRESTO_MEM_POOL_SIZES  { PRESTO_MEM_ITEMSIZ1, PRESTO_MEM_ITEMSIZ2 }
#define PRESTO_MEM_POOL_QTYS   { PRESTO_MEM_ITEMQTY1, PRESTO_MEM_ITEMQTY2 }
#define PRESTO_MEM_NUM_ITEMS   ( PRESTO_MEM_ITEMQTY1+ PRESTO_MEM_ITEMQTY2 )

#define PRESTO_MEM_TOTALBYTES (( PRESTO_MEM_ITEMQTY1 * PRESTO_MEM_ITEMSIZ1 )+ \
                               ( PRESTO_MEM_ITEMQTY2 * PRESTO_MEM_ITEMSIZ2 ))

////////////////////////////////////////////////////////////////////////////////

#endif // _CONFIGURE_H_

