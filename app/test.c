
#include "presto.h"
#include "types.h"
#include "services.h"
#include "priority.h"

////////////////////////////////////////////////////////////////////////////////

#define TIMER0     0
#define TIMER1     0
#define TIMER2     0
#define TIMER3   279

////////////////////////////////////////////////////////////////////////////////

/*
// system crashes after 21 seconds
#define TIMER0    500      // 42*500=21000
#define TIMER1    600      // 35*600=21000
#define TIMER2    700      // 30*700=21000
#define TIMER3    210      //
*/

////////////////////////////////////////////////////////////////////////////////

#define STACK_SIZE 256

static BYTE lcd_task_stack[STACK_SIZE];
static BYTE task_one_stack[STACK_SIZE];
static BYTE task_two_stack[STACK_SIZE];
static BYTE task_three_stack[STACK_SIZE];
static BYTE task_zero_stack[STACK_SIZE];

static PRESTO_TID_T lcd_task_tid;
static PRESTO_TID_T one_tid;
static PRESTO_TID_T two_tid;
static PRESTO_TID_T three_tid;
static PRESTO_TID_T zero_tid;

////////////////////////////////////////////////////////////////////////////////

#define MSG_LCD_BATT    1
#define MSG_LCD_LETTER  2

////////////////////////////////////////////////////////////////////////////////

void LcdTask(void) {
   char str[2];
   BYTE batt=LCD_BATTERY_TOP_0;
   PRESTO_MAIL_T msg;
   str[1]=0;
   while(1) {
      presto_sleep();
      if(presto_get_message(&msg)) {
         switch(msg.b.b1) {
            case MSG_LCD_BATT:
               batt++;
               if(batt>LCD_BATTERY_TOP_6) batt=LCD_BATTERY_TOP_1;
               lcd_raw_char(0,LCD_COLS-1,batt);
               break;
            case MSG_LCD_LETTER:
               str[0]=msg.b.b2;
               lcd_print(str);
               break;
         }
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void Zero(void) {
   sint8 speed0=MOTORS_MAX_SPEED;
   PRESTO_MAIL_T msg;
   msg.dw.dw1=0;
   while(1) {
      motor_set_speed(0,speed0);
      presto_timer(zero_tid,TIMER0,msg);
      presto_sleep();
      presto_get_message(&msg);
      speed0=0-speed0;
      msg.b.b1=MSG_LCD_BATT;
      presto_send_message(lcd_task_tid,msg);
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
      presto_sleep();
      presto_get_message(&msg);
      speed1=0-speed1;
   }
}

////////////////////////////////////////////////////////////////////////////////

void Two(void) {
   sint8 speed2=MOTORS_MAX_SPEED;
   PRESTO_MAIL_T msg;
   char c='A';
   msg.dw.dw1=0;
   while(1) {
      motor_set_speed(2,speed2);
      presto_timer(two_tid,TIMER2,msg);
      presto_sleep();
      presto_get_message(&msg);
      speed2=0-speed2;
   }
}

////////////////////////////////////////////////////////////////////////////////

void Three(void) {
   sint8 speed3=MOTORS_MAX_SPEED;
   PRESTO_MAIL_T msg;
   PRESTO_MAIL_T msg3;

   msg.dw.dw1=0;
   while(1) {
      motor_set_speed(3,speed3);
      presto_timer(three_tid,TIMER3,msg);
      msg.b.b1=MSG_LCD_BATT;
      presto_send_message(lcd_task_tid,msg);
      presto_sleep();
      presto_get_message(&msg3);
      speed3=0-speed3;
   }
}

////////////////////////////////////////////////////////////////////////////////

int main(void) {
   presto_init();
   lcd_task_tid=presto_create_task(LcdTask, lcd_task_stack, STACK_SIZE, 50);
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
   lcd_init();
   serial_init(9600);
   debugger_init();
   presto_start_scheduler();
   // we never get here
   return 0;
}

////////////////////////////////////////////////////////////////////////////////

