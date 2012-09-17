
#ifndef _KERNEL_H_
#define _KERNEL_H_

////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "configure.h"

////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////

#define KERNEL_TASKID_NONE    (PRESTO_KERNEL_MAXUSERTASKS+1)

////////////////////////////////////////////////////////////////////////////////
//   S I M P L E   D A T A   T Y P E S
////////////////////////////////////////////////////////////////////////////////

typedef unsigned char  KERNEL_TASKID_T;
typedef unsigned char  KERNEL_PRIORITY_T;

#if ( PRESTO_KERNEL_TRIGGERBITS == 8 )
   typedef unsigned char KERNEL_TRIGGER_T;
#elif ( PRESTO_KERNEL_TRIGGERBITS == 16 )
   typedef unsigned short KERNEL_TRIGGER_T;
#else
   #error PRESTO_KERNEL_TRIGGERBITS must be either 8 or 16
   // The following line suppresses a screenful of errors that would
   // otherwise be printed AFTER the #error above, helping the programmer
   // to see the #error.
   typedef unsigned char KERNEL_TRIGGER_T;
#endif // PRESTO_KERNEL_TRIGGERBITS

// the most significant bit is reserved for kernel calls that block
#define KERNEL_INTERNAL_TRIGGER  (1<<(PRESTO_KERNEL_TRIGGERBITS-1))

////////////////////////////////////////////////////////////////////////////////
//   K E R N E L - O N L Y   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

extern KERNEL_TASKID_T kernel_current_task(void);
extern void kernel_context_switch(void);
extern void kernel_trigger_set_noswitch(KERNEL_TASKID_T tid, KERNEL_TRIGGER_T trigger);

////////////////////////////////////////////////////////////////////////////////

#endif // _KERNEL_H_

