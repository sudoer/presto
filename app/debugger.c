////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "presto.h"
#include "types.h"
#include "cpu/hc11regs.h"
#include "cpu/intvect.h"
#include "services/serial.h"
#include "services/string.h"
#include "app/debugger.h"


////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////

#define DEBUGGER_STACK_SIZE 200
#define TIMER_FLAG  0x02
#define SERIAL_FLAG 0x01

////////////////////////////////////////////////////////////////////////////////
//   D A T A   T Y P E S
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   G L O B A L   D A T A
////////////////////////////////////////////////////////////////////////////////

static BYTE debugger_stack[DEBUGGER_STACK_SIZE];
static PRESTO_TID_T debugger_tid;

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N   P R O T O T Y P E S
////////////////////////////////////////////////////////////////////////////////

static void debugger(void);


////////////////////////////////////////////////////////////////////////////////
//   E X P O R T E D   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void debugger_init(void) {
   debugger_tid=presto_task_create(debugger, debugger_stack, DEBUGGER_STACK_SIZE, 1);
   serial_init(debugger_tid,SERIAL_FLAG);
}

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

#define LOOP  100
#define PTRS  100
extern void presto_memory_debug(void);

BYTE * m[PTRS];
static void debugger(void) {

   int i;
   int j;
   int k;

   unsigned short amount=1;

   serial_send_string("hello world\r\n");
   while (1) {

      for(k=0;k<LOOP;k++) {

         for(i=0,j=0;i<PTRS;i++,j=(j+5)%PTRS) {
            m[j]=presto_memory_allocate(amount);
         }
         if (k==0) presto_memory_debug();


         for(i=0,j=0;i<PTRS;i++,j=(j+7)%PTRS) {
            presto_memory_free(m[j]);
         }
         if (k==0) presto_memory_debug();

      }

      presto_timer_wait(500,0x01);

   }
}

////////////////////////////////////////////////////////////////////////////////


