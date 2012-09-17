
#include "presto.h"
#include "types.h"
#include "error.h"

////////////////////////////////////////////////////////////////////////////////

#define STACK_SIZE 0x100

static BYTE task_one_stack[STACK_SIZE];
static BYTE task_two_stack[STACK_SIZE];
static BYTE task_three_stack[STACK_SIZE];
static BYTE task_four_stack[STACK_SIZE];

PRESTO_SEMAPHORE_T copier;

BYTE lights=0xF0;

////////////////////////////////////////////////////////////////////////////////

#define FLAG_TIMER    0x01
#define FLAG_SEM      0x20
#define FLAG_COPYTICK 0x40


////////////////////////////////////////////////////////////////////////////////

#define MOTOR_PORT *(unsigned char *)(0x7FFF)

void assert_lights(void) {
   MOTOR_PORT=lights;
}

////////////////////////////////////////////////////////////////////////////////

void busy_work(BYTE blink_mask,WORD work_to_do) {
   PRESTO_TIMER_T timer;
   WORD work_done=0;
   presto_timer_start(&timer,100,0,FLAG_COPYTICK);
   presto_trigger_clear(FLAG_COPYTICK);
   while(work_done<work_to_do) {
      if(presto_trigger_poll(FLAG_COPYTICK)) {
         presto_trigger_clear(FLAG_COPYTICK);
         presto_timer_start(&timer,100,0,FLAG_COPYTICK);
         work_done++;
         lights^=blink_mask;
         assert_lights();
      }
   }
   presto_trigger_clear(FLAG_COPYTICK);
}

////////////////////////////////////////////////////////////////////////////////

void use_copier(BYTE blink_mask,WORD work_to_do) {
   WORD work_done=0;
   presto_semaphore_wait(&copier,FLAG_SEM);
   busy_work(blink_mask,work_to_do);
   presto_semaphore_release(&copier);
   presto_trigger_clear(FLAG_SEM);
}

////////////////////////////////////////////////////////////////////////////////

void president(void) {
   PRESTO_TIMER_T ticker4;
   presto_timer_start(&ticker4,3000,0,FLAG_TIMER);
   while(1) {
      presto_wait(FLAG_TIMER);
      presto_trigger_clear(FLAG_TIMER);
      use_copier(0x01,10);
   }
}

////////////////////////////////////////////////////////////////////////////////

void manager(void) {
   PRESTO_TIMER_T ticker3;
   presto_timer_start(&ticker3,4000,0,FLAG_TIMER);
   while(1) {
      presto_wait(FLAG_TIMER);
      presto_trigger_clear(FLAG_TIMER);
      busy_work(0x02,50);
   }
}

////////////////////////////////////////////////////////////////////////////////

void employee(void) {
   PRESTO_TIMER_T ticker2;
   presto_timer_start(&ticker2,1000,0,FLAG_TIMER);
   while(1) {
      presto_wait(FLAG_TIMER);
      presto_trigger_clear(FLAG_TIMER);
      use_copier(0x04,50);
   }
}

////////////////////////////////////////////////////////////////////////////////

void co_op(void) {
   PRESTO_TIMER_T ticker1;
   //busy_work(0x08,100);

   presto_timer_start(&ticker1,0,200,FLAG_TIMER);
   while(1) {
      presto_wait(FLAG_TIMER);
      presto_trigger_clear(FLAG_TIMER);
      lights^=0x08;
      assert_lights();
   }
}

////////////////////////////////////////////////////////////////////////////////

int main(void) {
   PRESTO_MAIL_T msg;

   assert_lights();
   presto_init();
   presto_create_task(president,  task_four_stack,  STACK_SIZE, 14);
   presto_create_task(manager,    task_three_stack, STACK_SIZE, 13);
   presto_create_task(employee,   task_two_stack,   STACK_SIZE, 12);
   presto_create_task(co_op,      task_one_stack,   STACK_SIZE, 11);
   presto_semaphore_init(&copier,1,TRUE);
   presto_start_scheduler();

   // we never get here
   presto_fatal_error(ERROR_MAIN_AFTERSTART);
   return 0;
}

////////////////////////////////////////////////////////////////////////////////
