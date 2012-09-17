
#include <stdio.h>
#include <hc11.h>
#include "presto.h"
#include "types.h"
#include "services.h"
#include "priority.h"

#define STACK_SIZE 256

static BYTE task_one_stack[STACK_SIZE];

static PRESTO_TID_T one_tid;

////////////////////////////////////////////////////////////////////////////////

void One(void) {
   int speed1=MOTORS_MAX_SPEED;
   PRESTO_MAIL_T msg;
   msg.dw.dw1=0;
   while(1) {
      motor_set_speed(0,MOTORS_MAX_SPEED);
      motor_set_speed(1,0-MOTORS_MAX_SPEED);
      presto_timer(one_tid,8000,msg);
      presto_sleep();
      presto_get_message(&msg);

      motor_set_speed(0,MOTORS_MAX_SPEED);
      motor_set_speed(1,MOTORS_MAX_SPEED);
      presto_timer(one_tid,2000,msg);
      presto_sleep();
      presto_get_message(&msg);

      motor_set_speed(0,MOTORS_MAX_SPEED);
      motor_set_speed(1,MOTORS_MAX_SPEED);
      presto_timer(one_tid,6000,msg);
      presto_sleep();
      presto_get_message(&msg);

      motor_set_speed(0,0-MOTORS_MAX_SPEED);
      motor_set_speed(1,0-MOTORS_MAX_SPEED);
      presto_timer(one_tid,1250,msg);
      presto_sleep();
      presto_get_message(&msg);

   }
}

////////////////////////////////////////////////////////////////////////////////

void main(void) {
   presto_init();
   one_tid=presto_create_task(One, task_one_stack, STACK_SIZE, 40);
   motor_init();
   presto_start_scheduler();
}

////////////////////////////////////////////////////////////////////////////////

