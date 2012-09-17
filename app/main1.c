
#include "presto.h"
#include "types.h"
#include "error.h"

////////////////////////////////////////////////////////////////////////////////

#define STACK_SIZE 0x100

static BYTE task_zero_stack[STACK_SIZE];
static BYTE task_one_stack[STACK_SIZE];
static BYTE task_two_stack[STACK_SIZE];
static BYTE task_three_stack[STACK_SIZE];
static BYTE task_four_stack[STACK_SIZE];

PRESTO_MAIL_T msg1;
PRESTO_MAIL_T msg2;
PRESTO_MAIL_T msg3;
PRESTO_MAIL_T msg4;
PRESTO_MAILBOX_T box1;
PRESTO_MAILBOX_T box2;
PRESTO_MAILBOX_T box3;
PRESTO_MAILBOX_T box4;

PRESTO_SEMAPHORE_T leds_sem;

BYTE lights=0xF0;

////////////////////////////////////////////////////////////////////////////////

#define FLAG_MAIL   0x10
#define FLAG_SEM    0x20
#define FLAG_TIMER1 0x01
#define FLAG_TIMER2 0x02
#define FLAG_TIMER3 0x04
#define FLAG_TIMER4 0x08
#define FLAG_TIMERS (FLAG_TIMER1|FLAG_TIMER2|FLAG_TIMER3|FLAG_TIMER4)

////////////////////////////////////////////////////////////////////////////////

#define MOTOR_PORT *(unsigned char *)(0x7FFF)

void assert_lights(void) {
   MOTOR_PORT=lights;
}

////////////////////////////////////////////////////////////////////////////////

void Zero(void) {
   PRESTO_MAIL_T msg;
   PRESTO_MAIL_T off;
   PRESTO_TIMER_T ticker1;
   PRESTO_TIMER_T ticker2;
   PRESTO_TIMER_T ticker3;
   PRESTO_TIMER_T ticker4;
   PRESTO_TRIGGER_T triggers;
   msg.dw.dw1=0;
   off.b.b1=0;
   lights=0xFF;
   assert_lights();

   presto_semaphore_init(&leds_sem,1);


   presto_timer_start(&ticker1,1000,250,FLAG_TIMER1);
   presto_timer_start(&ticker2,900,125,FLAG_TIMER2);
   presto_timer_start(&ticker3,300,620,FLAG_TIMER3);
   presto_timer_start(&ticker4,1000,335,FLAG_TIMER4);
   while(1) {
      triggers=presto_wait(FLAG_TIMERS);
      presto_trigger_clear(triggers);
      if(triggers&FLAG_TIMER1) {
         presto_mail_send(&box1,off);
      }
      if(triggers&FLAG_TIMER2) {
         presto_mail_send(&box2,off);
      }
      if(triggers&FLAG_TIMER3) {
         presto_mail_send(&box3,off);
      }
      if(triggers&FLAG_TIMER4) {
         presto_mail_send(&box4,off);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void One(void) {
   presto_mailbox_init(&box1,FLAG_MAIL);
   while(1) {
      presto_wait(FLAG_MAIL);
      presto_trigger_clear(FLAG_MAIL);
      presto_mail_get(&box1,&msg1);
      presto_semaphore_wait(&leds_sem,FLAG_SEM);
      lights^=0x01;
      assert_lights();
      presto_semaphore_release(&leds_sem);
   }
}

////////////////////////////////////////////////////////////////////////////////

void Two(void) {
   presto_mailbox_init(&box2,FLAG_MAIL);
   while(1) {
      presto_wait(FLAG_MAIL);
      presto_trigger_clear(FLAG_MAIL);
      presto_mail_get(&box2,&msg2);
      presto_semaphore_wait(&leds_sem,FLAG_SEM);
      lights^=0x02;
      assert_lights();
      presto_semaphore_release(&leds_sem);
   }
}

////////////////////////////////////////////////////////////////////////////////

void Three(void) {
   presto_mailbox_init(&box3,FLAG_MAIL);
   while(1) {
      presto_wait(FLAG_MAIL);
      presto_trigger_clear(FLAG_MAIL);
      presto_mail_get(&box3,&msg3);
      presto_semaphore_wait(&leds_sem,FLAG_SEM);
      lights^=0x04;
      assert_lights();
      presto_semaphore_release(&leds_sem);
   }
}

////////////////////////////////////////////////////////////////////////////////

void Four(void) {
   presto_mailbox_init(&box4,FLAG_MAIL);
   while(1) {
      presto_wait(FLAG_MAIL);
      presto_trigger_clear(FLAG_MAIL);
      presto_mail_get(&box4,&msg4);
      presto_semaphore_wait(&leds_sem,FLAG_SEM);
      lights^=0x08;
/*
      switch(msg4.b.b1) {
         case 0: lights&=~0x08; break;
         case 1: lights|=0x08; break;
      }
*/
      assert_lights();
      presto_semaphore_release(&leds_sem);
   }
}

////////////////////////////////////////////////////////////////////////////////

int main(void) {
   PRESTO_MAIL_T msg;

   presto_init();
   presto_create_task(Zero,  task_zero_stack,  STACK_SIZE, 10);
   presto_create_task(One,   task_one_stack,   STACK_SIZE, 11);
   presto_create_task(Two,   task_two_stack,   STACK_SIZE, 12);
   presto_create_task(Three, task_three_stack, STACK_SIZE, 13);
   presto_create_task(Four,  task_four_stack,  STACK_SIZE, 14);

   presto_start_scheduler();
   // we never get here
   presto_fatal_error(ERROR_MAIN_AFTERSTART);
   return 0;
}

////////////////////////////////////////////////////////////////////////////////
