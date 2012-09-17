
#include "presto.h"
#include "types.h"
#include "error.h"
#include "hc11regs.h"

////////////////////////////////////////////////////////////////////////////////

#define STATIC    // static

////////////////////////////////////////////////////////////////////////////////

#define STACK_SIZE 0x100

STATIC BYTE task_zero_stack[STACK_SIZE];
STATIC BYTE task_one_stack[STACK_SIZE];
STATIC BYTE task_two_stack[STACK_SIZE];
STATIC BYTE task_three_stack[STACK_SIZE];
STATIC BYTE task_four_stack[STACK_SIZE];

/*
PRESTO_TID_T zero_tid;
PRESTO_TID_T one_tid;
PRESTO_TID_T two_tid;
PRESTO_TID_T three_tid;
PRESTO_TID_T four_tid;
*/

PRESTO_MAIL_T msg1;
PRESTO_MAIL_T msg2;
PRESTO_MAIL_T msg3;
PRESTO_MAIL_T msg4;
PRESTO_MAILBOX_T box1;
PRESTO_MAILBOX_T box2;
PRESTO_MAILBOX_T box3;
PRESTO_MAILBOX_T box4;

BYTE lights=0xF0;

////////////////////////////////////////////////////////////////////////////////

#define FLAG_MAIL  0x10
#define FLAG_TIMER 0x01

////////////////////////////////////////////////////////////////////////////////

#define MOTOR_PORT *(unsigned char *)(0x7FFF)

void assert_lights(void) {
   MOTOR_PORT=lights;
}

////////////////////////////////////////////////////////////////////////////////

void Zero(void) {
   PRESTO_MAIL_T msg;
   PRESTO_MAIL_T off;
   PRESTO_MAIL_T on;
   PRESTO_TIMER_T ticker;
   unsigned char count=1;
   msg.dw.dw1=0;
   off.b.b1=0;
   on.b.b1=1;
   lights=0xFF;
   assert_lights();

   presto_timer(&ticker,250,100,FLAG_TIMER);
   while(1) {
      presto_wait(FLAG_TIMER);
      clear_flag(FLAG_TIMER);

      BITNOT(PORTA,3);   // toggle speaker
      switch(count) {
         case 1: {
            presto_mail_send(&box1,off);
         } break;
         case 2: {
            presto_mail_send(&box2,off);
         } break;
         case 3: {
            presto_mail_send(&box3,off);
         } break;
         case 4: {
            presto_mail_send(&box4,off);
         } break;
         case 5: {
            presto_mail_send(&box1,on);
         } break;
         case 6: {
            presto_mail_send(&box2,on);
         } break;
         case 7: {
            presto_mail_send(&box3,on);
         } break;
         case 8: {
            presto_mail_send(&box4,on);
         } break;
      }
      count++;
      if(count==9) count=1;
      presto_timer(&ticker,125,0,FLAG_TIMER);
   }
}

////////////////////////////////////////////////////////////////////////////////

void One(void) {
   presto_mailbox_init(&box1,FLAG_MAIL);
   while(1) {
      presto_wait(FLAG_MAIL);
      clear_flag(FLAG_MAIL);
      presto_mail_get(&box1,&msg1);
      switch(msg1.b.b1) {
         case 0: lights&=~0x01; break;
         case 1: lights|=0x01; break;
      }
      assert_lights();
   }
}

////////////////////////////////////////////////////////////////////////////////

void Two(void) {
   presto_mailbox_init(&box2,FLAG_MAIL);
   while(1) {
      presto_wait(FLAG_MAIL);
      clear_flag(FLAG_MAIL);
      presto_mail_get(&box2,&msg2);
      switch(msg2.b.b1) {
         case 0: lights&=~0x02; break;
         case 1: lights|=0x02; break;
      }
      assert_lights();
   }
}

////////////////////////////////////////////////////////////////////////////////

void Three(void) {
   presto_mailbox_init(&box3,FLAG_MAIL);
   while(1) {
      presto_wait(FLAG_MAIL);
      clear_flag(FLAG_MAIL);
      presto_mail_get(&box3,&msg3);
      switch(msg3.b.b1) {
         case 0: lights&=~0x04; break;
         case 1: lights|=0x04; break;
      }
      assert_lights();
   }
}

////////////////////////////////////////////////////////////////////////////////

void Four(void) {
   presto_mailbox_init(&box4,FLAG_MAIL);
   while(1) {
      presto_wait(FLAG_MAIL);
      clear_flag(FLAG_MAIL);
      presto_mail_get(&box4,&msg4);
      switch(msg4.b.b1) {
         case 0: lights&=~0x08; break;
         case 1: lights|=0x08; break;
      }
      assert_lights();
   }
}

////////////////////////////////////////////////////////////////////////////////

int main(void) {
   PRESTO_MAIL_T msg;

   presto_init();
/*
   zero_tid=presto_create_task(Zero, task_zero_stack, STACK_SIZE, 50);
   one_tid=presto_create_task(One, task_one_stack, STACK_SIZE, 35);
   two_tid=presto_create_task(Two, task_two_stack, STACK_SIZE, 30);
   three_tid=presto_create_task(Three, task_three_stack, STACK_SIZE, 20);
   four_tid=presto_create_task(Four, task_four_stack, STACK_SIZE, 5);
*/
   presto_create_task(Zero,  task_zero_stack,  STACK_SIZE, 50);
   presto_create_task(One,   task_one_stack,   STACK_SIZE, 35);
   presto_create_task(Two,   task_two_stack,   STACK_SIZE, 30);
   presto_create_task(Three, task_three_stack, STACK_SIZE, 20);
   presto_create_task(Four,  task_four_stack,  STACK_SIZE, 5);

   presto_start_scheduler();
   // we never get here
   presto_fatal_error(ERROR_MAIN_END);
   return 0;
}

////////////////////////////////////////////////////////////////////////////////
