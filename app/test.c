
#include "presto.h"
#include "types.h"
#include "services.h"
#include "priority.h"

////////////////////////////////////////////////////////////////////////////////

// system crashes after 21 seconds (42*500=21000,35*600=21000,30*700=21000)
#define TIMER0    1200
#define TIMER1    5000
#define TIMER2    600
#define TIMER3    1000

////////////////////////////////////////////////////////////////////////////////

#define STACK_SIZE 100

static BYTE task_one_stack[STACK_SIZE];
static BYTE task_two_stack[STACK_SIZE];
static BYTE task_three_stack[STACK_SIZE];
static BYTE task_zero_stack[STACK_SIZE];

PRESTO_TID_T one_tid=0;
PRESTO_TID_T two_tid=0;
PRESTO_TID_T three_tid=0;
PRESTO_TID_T zero_tid=0;

////////////////////////////////////////////////////////////////////////////////

void Zero(void) {
   sint8 speed0=MOTORS_MAX_SPEED;
   PRESTO_MAIL_T msg;
   msg.dw.dw1=0;
   while(1) {
      motor_set_speed(0,speed0);
      presto_timer(zero_tid,TIMER0,msg);
      presto_wait_for_message(&msg);
      speed0=0-speed0;
   }
}

////////////////////////////////////////////////////////////////////////////////

void One(void) {
   sint8 speed1=MOTORS_MAX_SPEED;
   PRESTO_MAIL_T msg;
   msg.dw.dw1=0;
   while(1) {
      motor_set_speed(1,speed1);
      presto_timer(one_tid,TIMER1,msg);
      presto_wait_for_message(&msg);
      speed1=0-speed1;
   }
}

////////////////////////////////////////////////////////////////////////////////

void Two(void) {
   sint8 speed2=MOTORS_MAX_SPEED;
   PRESTO_MAIL_T msg;
   msg.dw.dw1=0;
   while(1) {
      motor_set_speed(2,speed2);
      presto_timer(two_tid,TIMER2,msg);
      presto_wait_for_message(&msg);
      speed2=0-speed2;
   }
}

////////////////////////////////////////////////////////////////////////////////

void Three(void) {
   sint8 speed3=MOTORS_MAX_SPEED;
   PRESTO_MAIL_T msg;
   msg.dw.dw1=0;
   while(1) {
      motor_set_speed(3,speed3);
      presto_timer(three_tid,TIMER3,msg);
      presto_wait_for_message(&msg);
      speed3=0-speed3;
   }
}

////////////////////////////////////////////////////////////////////////////////

int main(void) {

   presto_init();
#if TIMER0 != 0
   zero_tid=presto_create_task(Zero, task_zero_stack, STACK_SIZE, 30);
#endif
#if TIMER1 != 0
   one_tid=presto_create_task(One, task_one_stack, STACK_SIZE, 35);
#endif
#if TIMER2 != 0
   two_tid=presto_create_task(Two, task_two_stack, STACK_SIZE, 40);
#endif
#if TIMER3 != 0
   three_tid=presto_create_task(Three, task_three_stack, STACK_SIZE, 45);
#endif

   motor_init();
   //lcd_init();
   //serial_init(9600);
   //debugger_init();
   presto_start_scheduler();
   // we never get here
   presto_fatal_error();
   return 0;
}

////////////////////////////////////////////////////////////////////////////////

