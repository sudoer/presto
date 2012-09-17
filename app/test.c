
#include "presto.h"
#include "types.h"
#include "error.h"

////////////////////////////////////////////////////////////////////////////////

#define TIMER1   1000
#define TIMER2    700
#define TIMER3    170
#define TIMER4     80

////////////////////////////////////////////////////////////////////////////////

#define STATIC    // static

////////////////////////////////////////////////////////////////////////////////

#define STACK_SIZE 0x100

STATIC BYTE task_one_stack[STACK_SIZE];
STATIC BYTE task_two_stack[STACK_SIZE];
STATIC BYTE task_three_stack[STACK_SIZE];
STATIC BYTE task_four_stack[STACK_SIZE];

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
   presto_repeating_timer(one_tid,TIMER1,TIMER1,msg);
   while(1) {
      presto_wait_for_message(&msg);
      light1=light1^0x01;
      assert_lights();
   }
}

////////////////////////////////////////////////////////////////////////////////

void Two(void) {
   PRESTO_MAIL_T msg;
   msg.dw.dw1=0;
   presto_repeating_timer(two_tid,TIMER2,TIMER2,msg);
   while(1) {
      presto_wait_for_message(&msg);
      light2=light2^0x02;
      assert_lights();
      presto_send_message(one_tid,msg);
      presto_send_message(three_tid,msg);
   }
}

////////////////////////////////////////////////////////////////////////////////

void Three(void) {
   PRESTO_MAIL_T msg;
   msg.dw.dw1=0;
   while(1) {
      presto_wait_for_message(&msg);
      light3=light3^0x04;
      assert_lights();
   }
}

////////////////////////////////////////////////////////////////////////////////

void Four(void) {
   PRESTO_MAIL_T msg;
   msg.dw.dw1=0;
   presto_send_message(four_tid,msg);
   while(1) {
      presto_wait_for_message(&msg);
      light4=light4^0x08;
      assert_lights();
      presto_send_message(three_tid,msg);
      presto_send_message(four_tid,msg);
   }
}

////////////////////////////////////////////////////////////////////////////////

int main(void) {
   PRESTO_MAIL_T msg;

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
   one_tid=presto_create_task(One, task_one_stack, STACK_SIZE, 35);
   two_tid=presto_create_task(Two, task_two_stack, STACK_SIZE, 30);
   three_tid=presto_create_task(Three, task_three_stack, STACK_SIZE, 20);
   four_tid=presto_create_task(Four, task_four_stack, STACK_SIZE, 5);

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
