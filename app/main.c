
#include "presto.h"
#include "types.h"
#include "error_codes.h"
#include "cpu/misc_hw.h"
#include "app/debugger.h"

////////////////////////////////////////////////////////////////////////////////

#define STACK_SIZE 0x100

static BYTE coop_stack[STACK_SIZE];
static BYTE emp_stack[STACK_SIZE];
static BYTE mgr_stack[STACK_SIZE];
static BYTE pres_stack[STACK_SIZE];

BYTE lights=0xF0;

PRESTO_TID_T pres_tid;
PRESTO_TID_T mgr_tid;
PRESTO_TID_T emp_tid;
PRESTO_TID_T coop_tid;

PRESTO_SEMAPHORE_T lightsem;

////////////////////////////////////////////////////////////////////////////////

#define FLAG_INIT     0x80
#define FLAG_START    0x40
#define FLAG_COPYTICK 0x20
#define FLAG_SEM      0x10
#define FLAG_MAIL     0x04
#define FLAG_COOP     0x02
#define FLAG_TIMER    0x01

////////////////////////////////////////////////////////////////////////////////

void assert_lights(BYTE reverse) {
   //presto_semaphore_wait(&lightsem,FLAG_SEM);
   //presto_trigger_clear(FLAG_SEM);
   lights^=(reverse);
   MOTOR_LED_PORT=lights;
   //presto_semaphore_release(&lightsem);
}

////////////////////////////////////////////////////////////////////////////////

void president(void) {
   PRESTO_TIMER_T ticker1;
   presto_timer_start(&ticker1,1000,1000,FLAG_TIMER);
   while (1) {
      presto_wait(FLAG_TIMER);
      presto_trigger_clear(FLAG_TIMER);
      assert_lights(0x01);
   }
}

////////////////////////////////////////////////////////////////////////////////

void manager(void) {
   PRESTO_TIMER_T ticker1;
   presto_timer_start(&ticker1,750,1000,FLAG_TIMER);
   while (1) {
      presto_wait(FLAG_TIMER);
      presto_trigger_clear(FLAG_TIMER);
      assert_lights(0x02);
   }
}

////////////////////////////////////////////////////////////////////////////////

void employee(void) {
   PRESTO_TIMER_T ticker1;
   presto_timer_start(&ticker1,500,1000,FLAG_TIMER);
   while (1) {
      presto_wait(FLAG_TIMER);
      presto_trigger_clear(FLAG_TIMER);
      assert_lights(0x04);
   }
}

////////////////////////////////////////////////////////////////////////////////

void co_op(void) {
   PRESTO_TIMER_T ticker1;
   presto_timer_start(&ticker1,250,1000,FLAG_TIMER);
   while (1) {
      presto_wait(FLAG_TIMER);
      presto_trigger_clear(FLAG_TIMER);
      assert_lights(0x08);
   }
}

////////////////////////////////////////////////////////////////////////////////

int main(void) {
   presto_init();
   presto_semaphore_init(&lightsem,1,FALSE);
   pres_tid=presto_task_create(president, pres_stack, STACK_SIZE, 14);
   mgr_tid= presto_task_create(manager,   mgr_stack,  STACK_SIZE, 13);
   emp_tid= presto_task_create(employee,  emp_stack,  STACK_SIZE, 12);
   coop_tid=presto_task_create(co_op,     coop_stack, STACK_SIZE, 11);

   debugger_init();
   assert_lights(0x00);

   presto_start_scheduler();

   // we never get here
   presto_fatal_error(ERROR_MAIN_AFTERSTART);
   return 0;
}

////////////////////////////////////////////////////////////////////////////////


