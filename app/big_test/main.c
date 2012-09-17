
#include "presto.h"
#include "types.h"
#include "error.h"
#include "board.h"
#include "serial.h"
#include "string.h"
#include "kernel/memory.h"
#include "messages.h"
#include "shared.h"

////////////////////////////////////////////////////////////////////////////////

#define STACK_SIZE 0xA0

static BYTE stud_stack[STACK_SIZE];
static BYTE empl_stack[STACK_SIZE];
static BYTE mngr_stack[STACK_SIZE];
static BYTE pres_stack[STACK_SIZE];
static BYTE ctrl_stack[STACK_SIZE];

PRESTO_TASKID_T ctrl_tid;
PRESTO_TASKID_T pres_tid;
PRESTO_TASKID_T mngr_tid;
PRESTO_TASKID_T empl_tid;
PRESTO_TASKID_T stud_tid;

////////////////////////////////////////////////////////////////////////////////

//    TIME  HAPPENINGS
//   -----  --------------
//       0  all tasks signaled to LOOP
//    1000  EMPLOYEE takes copier for 5 seconds work
//    3000  PRESIDENT requests copier for 1 second work, has to wait
//    4000  MANAGER asks EMPLOYEE into a 5 second meeting


//   WITH NORMAL SEMAPHORES                                      RUNNING
//   ----------------------                                      -------
//       0  all tasks signaled to LOOP                           COOP (1)
//    1000  EMPLOYEE takes copier                                EMP  (2)
//    3000  PRESIDENT requests copier, has to wait               EMP  (1)
//    4000  MANAGER and EMPLOYEE enter meeting                   MGR  (5)
//    9000  meeting over, EMPLOYEE resumes copying               EMP  (2)
//   11000  EMPLOYEE finishes copying, PRESIDENT takes copier    PRES (1)
//   12000  PRESIDENT finishes copying                           COOP


//   WITH PRIORITY INHERITANCE                                   RUNNING
//   -------------------------                                   -------
//       0  all tasks signaled to LOOP                           COOP (1)
//    1000  EMPLOYEE takes copier                                EMP  (2)
//    3000  PRESIDENT requests copier, has to wait               EMP  (1)
//    4000  MANAGER asks EMPLOYEE into meeting, has to wait      EMP  (2)
//    6000  EMPLOYEE finishes copying, PRESIDENT takes copier    PRES (1)
//    7000  PRESIDENT finishes copying, meeting starts           MGR  (5)
//   12000  meeting over                                         COOP

////////////////////////////////////////////////////////////////////////////////

extern void control(void);
extern void president(void);
extern void manager(void);
extern void employee(void);
extern void student(void);

////////////////////////////////////////////////////////////////////////////////

int main(void) {

   //DDRD=0x3F;
   assert_lights();
   presto_init();

   ctrl_tid=presto_task_create(control,   ctrl_stack,  STACK_SIZE, 99);
   pres_tid=presto_task_create(president, pres_stack,  STACK_SIZE, 14);
   mngr_tid=presto_task_create(manager,   mngr_stack,  STACK_SIZE, 13);
   empl_tid=presto_task_create(employee,  empl_stack,  STACK_SIZE, 12);
   stud_tid=presto_task_create(student,   stud_stack,  STACK_SIZE, 11);

   presto_scheduler_start();

   // we never get here
   error_fatal(ERROR_MAIN_AFTERSTART);
   return 0;
}

////////////////////////////////////////////////////////////////////////////////


