
#include "presto.h"
#include "types.h"
#include "error.h"

////////////////////////////////////////////////////////////////////////////////

#define STACK_SIZE 0x100

static BYTE task_one_stack[STACK_SIZE];
static BYTE task_two_stack[STACK_SIZE];
static BYTE task_three_stack[STACK_SIZE];
static BYTE task_four_stack[STACK_SIZE];
static BYTE task_five_stack[STACK_SIZE];

PRESTO_SEMAPHORE_T copier;

BYTE lights=0xF0;

PRESTO_TID_T control_tid;
PRESTO_TID_T pres_tid;
PRESTO_TID_T mgr_tid;
PRESTO_TID_T emp_tid;
PRESTO_TID_T coop_tid;

////////////////////////////////////////////////////////////////////////////////

#define FLAG_INIT     0x80
#define FLAG_LOOP     0x40
#define FLAG_COPYTICK 0x20
#define FLAG_SEM      0x10
#define FLAG_MAIL     0x04
#define FLAG_COOP     0x02
#define FLAG_TIMER    0x01


////////////////////////////////////////////////////////////////////////////////

#define MOTOR_PORT *(unsigned char *)(0x7FFF)

void assert_lights(void) {
   MOTOR_PORT=lights;
}

////////////////////////////////////////////////////////////////////////////////

void busy_work(BYTE blink_mask,WORD work_to_do) {
   PRESTO_TIMER_T timer;
   WORD work_done=0;
   presto_timer_start(&timer,0,100,FLAG_COPYTICK);
   while(work_done<work_to_do) {
      if(presto_trigger_poll(FLAG_COPYTICK)) {
         presto_trigger_clear(FLAG_COPYTICK);
         work_done++;
         lights^=blink_mask;
         assert_lights();
      }
   }
   presto_timer_disable(&timer);
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

void control(void) {

   BOOLEAN tf=FALSE;

   // tell each task to initialize here

   presto_trigger_send(pres_tid,FLAG_INIT);
   presto_wait(FLAG_INIT);
   presto_trigger_clear(FLAG_INIT);

   presto_trigger_send(mgr_tid,FLAG_INIT);
   presto_wait(FLAG_INIT);
   presto_trigger_clear(FLAG_INIT);

   presto_trigger_send(emp_tid,FLAG_INIT);
   presto_wait(FLAG_INIT);
   presto_trigger_clear(FLAG_INIT);

   presto_trigger_send(coop_tid,FLAG_INIT);
   presto_wait(FLAG_INIT);
   presto_trigger_clear(FLAG_INIT);

   while(1) {
      presto_semaphore_init(&copier,1,tf);
      presto_trigger_send(pres_tid,FLAG_LOOP);
      presto_trigger_send(mgr_tid,FLAG_LOOP);
      presto_trigger_send(emp_tid,FLAG_LOOP);
      presto_trigger_send(coop_tid,FLAG_LOOP);
      presto_timer_wait(15000,FLAG_TIMER);
      presto_trigger_clear(FLAG_TIMER);
      tf=tf?FALSE:TRUE;
   }
}

////////////////////////////////////////////////////////////////////////////////

void president(void) {
   presto_wait(FLAG_INIT);
   // initialize here
   presto_trigger_send(control_tid, FLAG_INIT);
   while(1) {
      presto_wait(FLAG_LOOP);
      presto_trigger_clear(FLAG_LOOP);

      presto_timer_wait(3000,FLAG_TIMER);
      presto_trigger_clear(FLAG_TIMER);
      use_copier(0x01,10);
   }
}

////////////////////////////////////////////////////////////////////////////////

void manager(void) {
   presto_wait(FLAG_INIT);
   // initialize here
   presto_trigger_send(control_tid, FLAG_INIT);
   while(1) {
      presto_wait(FLAG_LOOP);
      presto_trigger_clear(FLAG_LOOP);

      presto_timer_wait(4000,FLAG_TIMER);
      presto_trigger_clear(FLAG_TIMER);
      busy_work(0x02,50);
   }
}

////////////////////////////////////////////////////////////////////////////////

void employee(void) {
   presto_wait(FLAG_INIT);
   // initialize here
   presto_trigger_send(control_tid, FLAG_INIT);
   while(1) {
      presto_wait(FLAG_LOOP);
      presto_trigger_clear(FLAG_LOOP);

      presto_timer_wait(1000,FLAG_TIMER);
      presto_trigger_clear(FLAG_TIMER);
      use_copier(0x04,50);
   }
}

////////////////////////////////////////////////////////////////////////////////

PRESTO_TIMER_T ticker1;

void co_op(void) {
   presto_wait(FLAG_INIT);
   // initialize here
   presto_timer_start(&ticker1,0,200,FLAG_COOP);
   presto_trigger_send(control_tid, FLAG_INIT);
   while(1) {
      presto_wait(FLAG_COOP);
      presto_trigger_clear(FLAG_COOP);
      lights^=0x08;
      assert_lights();
   }
}

////////////////////////////////////////////////////////////////////////////////

int main(void) {
   PRESTO_MAIL_T msg;

   assert_lights();
   presto_init();
   control_tid=presto_create_task(control,    task_five_stack,  STACK_SIZE, 99);
   pres_tid=   presto_create_task(president,  task_four_stack,  STACK_SIZE, 14);
   mgr_tid=    presto_create_task(manager,    task_three_stack, STACK_SIZE, 13);
   emp_tid=    presto_create_task(employee,   task_two_stack,   STACK_SIZE, 12);
   coop_tid=   presto_create_task(co_op,      task_one_stack,   STACK_SIZE, 11);
   presto_start_scheduler();

   // we never get here
   presto_fatal_error(ERROR_MAIN_AFTERSTART);
   return 0;
}

////////////////////////////////////////////////////////////////////////////////
