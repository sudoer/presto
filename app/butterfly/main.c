

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <inttypes.h>


#include "types.h"
#include "cpu_locks.h"
#include "presto.h"
#include "lcd_driver.h"
#include "error.h"

////////////////////////////////////////////////////////////////////////////////

#define STACK_SIZE 0x80
#define STATIC

STATIC BYTE one_stack[STACK_SIZE];
STATIC BYTE two_stack[STACK_SIZE];

PRESTO_TASKID_T one_tid;
PRESTO_TASKID_T two_tid;

////////////////////////////////////////////////////////////////////////////////

extern void one(void);
extern void two(void);

////////////////////////////////////////////////////////////////////////////////

int main(void) {

   lcd_init();

   presto_init();
   two_tid=presto_task_create(two,  two_stack,  STACK_SIZE, 14);
   one_tid=presto_task_create(one,  one_stack,  STACK_SIZE, 99);
   presto_scheduler_start();

   // we never get here
   error_fatal(ERROR_MAIN_AFTERSTART);
   return 0;
}

////////////////////////////////////////////////////////////////////////////////


