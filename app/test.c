
#include "presto.h"
#include "types.h"
#include "error.h"

////////////////////////////////////////////////////////////////////////////////

#define TIMER1    175
#define TIMER2    250
#define TIMER3    333
#define TIMER4    500

////////////////////////////////////////////////////////////////////////////////

#define STACK_SIZE 0x100

/*static*/ BYTE task_one_stack[STACK_SIZE];
/*static*/ BYTE task_two_stack[STACK_SIZE];
/*static*/ BYTE task_three_stack[STACK_SIZE];
/*static*/ BYTE task_four_stack[STACK_SIZE];

PRESTO_TID_T one_tid=0;
PRESTO_TID_T two_tid=0;
PRESTO_TID_T three_tid=0;
PRESTO_TID_T four_tid=0;

BYTE light1=0x00;
BYTE light2=0x00;
BYTE light3=0x00;
BYTE light4=0x00;

////////////////////////////////////////////////////////////////////////////////

#define MOTOR_PORT *(unsigned char *)(0x7FFF)

void assert_lights(void) {
   BYTE lights;
   lights=0xF0|light1|light2|light3|light4;
   MOTOR_PORT=lights;
}

////////////////////////////////////////////////////////////////////////////////

void One(void) {
   PRESTO_MAIL_T msg;
   msg.dw.dw1=0;
   while(1) {
      light1=light1^0x01;
      assert_lights();
      presto_timer(one_tid,TIMER1,msg);
      presto_wait_for_message(&msg);
   }
}

////////////////////////////////////////////////////////////////////////////////

void Two(void) {
   PRESTO_MAIL_T msg;
   msg.dw.dw1=0;
   while(1) {
      light2=light2^0x02;
      assert_lights();
      presto_timer(two_tid,TIMER2,msg);
      presto_wait_for_message(&msg);
   }
}

////////////////////////////////////////////////////////////////////////////////

void Three(void) {
   PRESTO_MAIL_T msg;
   msg.dw.dw1=0;
   while(1) {
      light3=light3^0x04;
      assert_lights();
      presto_timer(three_tid,TIMER3,msg);
      presto_wait_for_message(&msg);
   }
}

////////////////////////////////////////////////////////////////////////////////

void Four(void) {
   PRESTO_MAIL_T msg;
   msg.dw.dw1=0;
   while(1) {
      light4=light4^0x08;
      assert_lights();
      presto_timer(four_tid,TIMER4,msg);
      presto_wait_for_message(&msg);
   }
}

////////////////////////////////////////////////////////////////////////////////

int main(void) {

   // initialization
   one_tid=0;
   two_tid=0;
   three_tid=0;
   four_tid=0;
   light1=0x00;
   light2=0x00;
   light3=0x00;
   light4=0x00;

   presto_init();
#if TIMER1 != 0
   one_tid=presto_create_task(One, task_one_stack, STACK_SIZE, 35);
#endif
#if TIMER2 != 0
   two_tid=presto_create_task(Two, task_two_stack, STACK_SIZE, 40);
#endif
#if TIMER3 != 0
   three_tid=presto_create_task(Three, task_three_stack, STACK_SIZE, 45);
#endif
#if TIMER4 != 0
   four_tid=presto_create_task(Four, task_four_stack, STACK_SIZE, 50);
#endif

   //motor_init();
   //lcd_init();
   //serial_init(9600);
   //debugger_init();
   presto_start_scheduler();
   // we never get here
   presto_fatal_error(ERROR_MAIN_END);
   return 0;
}

////////////////////////////////////////////////////////////////////////////////
