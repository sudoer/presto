
#include "cpu_debug.h"
#include "cpu_magic.h"
#include "cpu_timer.h"

#include "types.h"
#include "cpu_locks.h"
#include "presto.h"
#include "lcd_driver.h"
#include "error.h"

////////////////////////////////////////////////////////////////////////////////

#define STACK_SIZE 0x80
#define STATIC static

STATIC BYTE one_stack[STACK_SIZE];
STATIC BYTE two_stack[STACK_SIZE];

PRESTO_TASKID_T one_tid;
PRESTO_TASKID_T two_tid;

////////////////////////////////////////////////////////////////////////////////

extern void one(void);
extern void two(void);
void test_timer_isr(void) __attribute__((interrupt));
char a0,a1,a2,a3,a4,a5;

////////////////////////////////////////////////////////////////////////////////

void test_timer_isr(void) {
   CPU_MAGIC_START_OF_SWI();
   CPU_DEBUG_TICK_START();
   CPU_TIMER_RESTART();
   if((a0<='A')||(a0>'Z')) a0='Z';
   else a0--;
   CPU_DEBUG_TICK_END();
   CPU_MAGIC_END_OF_SWI();
}

////////////////////////////////////////////////////////////////////////////////

int main(void) {

   lcd_init();

/*
   // do hardware timer magic
   CPU_TIMER_START(PRESTO_KERNEL_MSPERTICK,(void (*)(void))test_timer_isr);

   while(1) {
      volatile unsigned short x;
      for(a0='A';a0<='Z';a0++) {
         for(a1='A';a1<='Z';a1++) {
            for(a2='A';a2<='Z';a2++) {
               for(a3='A';a3<='Z';a3++) {
                  for(a4='A';a4<='Z';a4++) {
                     for(a5='A';a5<='Z';a5++) {
                        lcd_text_digit(0,a0);
                        lcd_text_digit(1,a1);
                        lcd_text_digit(2,a2);
                        lcd_text_digit(3,a3);
                        lcd_text_digit(4,a4);
                        lcd_text_digit(5,a5);
                        for(x=0;x<10000;x++);
                     }
                  }
               }
            }
         }
      }
   }
*/

   presto_init();
   two_tid=presto_task_create(two,  two_stack,  STACK_SIZE, 14);
   one_tid=presto_task_create(one,  one_stack,  STACK_SIZE, 15);
   presto_scheduler_start();

   // we never get here
   error_fatal(ERROR_MAIN_AFTERSTART);
   return 0;
}

////////////////////////////////////////////////////////////////////////////////


