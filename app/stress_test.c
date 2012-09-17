
#include "presto.h"
#include "types.h"
#include "cpu/error.h"
#include "cpu/misc_hw.h"
#include "services/serial.h"
#include "services/string.h"
#include "kernel/memory.h"

////////////////////////////////////////////////////////////////////////////////

#define STACK_SIZE 0xA0

/*static*/ BYTE stud_stack[STACK_SIZE];
/*static*/ BYTE empl_stack[STACK_SIZE];
/*static*/ BYTE mngr_stack[STACK_SIZE];
/*static*/ BYTE pres_stack[STACK_SIZE];
/*static*/ BYTE ctrl_stack[STACK_SIZE];
/*static*/ BYTE dbug_stack[STACK_SIZE];

////////////////////////////////////////////////////////////////////////////////

enum {
   // to president
   MSG_CTRLtoPRES_INIT,
   MSG_CTRLtoPRES_LOOP,
   MSG_MNGRtoPRES_STATUS,
   // to manager
   MSG_CTRLtoMNGR_INIT,
   MSG_CTRLtoMNGR_LOOP,
   MSG_EMPLtoMNGR_STATUS,
   // to employee
   MSG_CTRLtoEMPL_INIT,
   MSG_CTRLtoEMPL_LOOP,
   MSG_STUDtoEMPL_STATUS,
   // to student
   MSG_CTRLtoSTUD_INIT,
   MSG_CTRLtoSTUD_LOOP,
};

////////////////////////////////////////////////////////////////////////////////

PRESTO_SEMAPHORE_T copier;

BYTE lights=0xFF;

PRESTO_TASKID_T ctrl_tid;
PRESTO_TASKID_T pres_tid;
PRESTO_TASKID_T mngr_tid;
PRESTO_TASKID_T empl_tid;
PRESTO_TASKID_T stud_tid;
PRESTO_TASKID_T dbug_tid;

////////////////////////////////////////////////////////////////////////////////

#define FLAG_ALL_INIT      0xFF
#define FLAG_ALL_COPYTICK  0x80
#define FLAG_ALL_SEM       0x40

#define FLAG_C_MAIL        0x02
#define FLAG_C_TIMER       0x01

#define FLAG_E_LOOP        0x04
#define FLAG_E_MAIL        0x02
#define FLAG_E_TIMER       0x01

#define FLAG_M_LOOP        0x04
#define FLAG_M_MAIL        0x02
#define FLAG_M_TIMER       0x01

#define FLAG_P_LOOP        0x04
#define FLAG_P_MAIL        0x02
#define FLAG_P_TIMER       0x01

#define FLAG_S_LOOP        0x04
#define FLAG_S_MAIL        0x02
#define FLAG_S_TIMER       0x01

#define FLAG_D_SERIALRX    0x01
#define FLAG_D_SERIALTX    0x02
#define FLAG_D_TIMER       0x04

////////////////////////////////////////////////////////////////////////////////

#define PRT  80
char prt[PRT];

////////////////////////////////////////////////////////////////////////////////

#define MOTOR_PORT *(unsigned char *)(0x7FFF)

void assert_lights(void) {
   MOTOR_PORT=lights;
}

////////////////////////////////////////////////////////////////////////////////

void busy_work(BYTE blink_mask,WORD worktodo) {
   PRESTO_TIMER_T ThisTimerIsOnTheStack;
   WORD work_done=0;
   BYTE count=0;
   presto_timer_start(&ThisTimerIsOnTheStack,0,25,FLAG_ALL_COPYTICK);
   while (work_done<worktodo) {
      if (presto_trigger_poll(FLAG_ALL_COPYTICK)) {
         presto_trigger_clear(FLAG_ALL_COPYTICK);
         count++;
         if(count==4) {
            count=0;
            work_done++;
         }
         MASKNOT(lights,blink_mask);
         assert_lights();
      }
   }
   presto_timer_stop(&ThisTimerIsOnTheStack);
   presto_trigger_clear(FLAG_ALL_COPYTICK);
   MASKCLR(lights,blink_mask);
   assert_lights();
}

////////////////////////////////////////////////////////////////////////////////

void use_copier(BYTE blink_mask,WORD worktodo) {
   presto_semaphore_wait(&copier,FLAG_ALL_SEM);
   busy_work(blink_mask,worktodo);
   presto_semaphore_release(&copier);
}

////////////////////////////////////////////////////////////////////////////////

void control(void) {

   static PRESTO_MAILBOX_T ctrl_mbox;
   BOOLEAN tf=FALSE;

   presto_mailbox_init(&ctrl_mbox,FLAG_C_MAIL);

   // We must pass control to each task, one by one, to give them a chance to
   // initialize their mailboxes and other resources.  Otherwise, if we tried
   // to send them a message, we would crash before they received it.

   // Let student task initilize.
   presto_trigger_send(stud_tid,FLAG_ALL_INIT);
   presto_wait(FLAG_ALL_INIT);

   // Let employee task initialize.
   presto_trigger_send(empl_tid,FLAG_ALL_INIT);
   presto_wait(FLAG_ALL_INIT);

   // Let manager task initialize.
   presto_trigger_send(mngr_tid,FLAG_ALL_INIT);
   presto_wait(FLAG_ALL_INIT);

   // Let president task initialize.
   presto_trigger_send(pres_tid,FLAG_ALL_INIT);
   presto_wait(FLAG_ALL_INIT);

   // We are ready to run normally now.

   PRESTO_ENVELOPE_T * send_p;
   while (1) {
      #ifdef FEATURE_SEMAPHORE_PRIORITYINHERITANCE
         presto_semaphore_init(&copier,1,tf);
      #else
         presto_semaphore_init(&copier,1);
      #endif

      // president is notified by mail
      send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
      presto_mail_send_to_task(pres_tid,send_p,MSG_CTRLtoPRES_LOOP,NULL);

      // manager is notified by mail
      send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
      presto_mail_send_to_task(mngr_tid,send_p,MSG_CTRLtoMNGR_LOOP,NULL);

      // employee is notified by mail
      send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
      presto_mail_send_to_task(empl_tid,send_p,MSG_CTRLtoEMPL_LOOP,NULL);

      // student is notified by mail
      send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
      presto_mail_send_to_task(stud_tid,send_p,MSG_CTRLtoSTUD_LOOP,NULL);

      // wait until next time
      presto_timer_wait(15000,FLAG_C_TIMER);

      tf=tf?FALSE:TRUE;
   }
}

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

void president(void) {
   static PRESTO_MAILBOX_T pres_mbox;
   PRESTO_TIMER_T * timer_p=NULL;

   presto_wait(FLAG_ALL_INIT);
   // initialize here
   presto_mailbox_init(&pres_mbox,FLAG_P_MAIL);
   // done initializing
   presto_trigger_send(ctrl_tid,FLAG_ALL_INIT);

   while (1) {
      PRESTO_TRIGGER_T t;
      t=presto_wait(FLAG_P_MAIL|FLAG_P_TIMER);

      if (t&FLAG_P_MAIL) {
         PRESTO_ENVELOPE_T * recv_p;
         recv_p=presto_mail_get(&pres_mbox);
         switch(presto_envelope_message(recv_p)) {
            case MSG_CTRLtoPRES_LOOP: 
               timer_p=(PRESTO_TIMER_T *)presto_memory_allocate(sizeof(PRESTO_TIMER_T));
               presto_timer_start(timer_p,3000,0,FLAG_P_TIMER);
               break;
            case MSG_MNGRtoPRES_STATUS:
               busy_work(0x0F,5);
               break;
         }
         presto_memory_free((BYTE *)recv_p);
      }

      if (t&FLAG_P_TIMER) {
         presto_memory_free((BYTE *)timer_p);
         use_copier(0x01,10);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void manager(void) {
   static PRESTO_MAILBOX_T mngr_mbox;
   presto_wait(FLAG_ALL_INIT);
   // initialize here
   presto_mailbox_init(&mngr_mbox,FLAG_M_MAIL);
   // done initializing
   presto_trigger_send(ctrl_tid,FLAG_ALL_INIT);

   while (1) {
      PRESTO_ENVELOPE_T * recv_p;
      recv_p=presto_mail_wait(&mngr_mbox);
      switch(presto_envelope_message(recv_p)) {
         case MSG_CTRLtoMNGR_LOOP:
            presto_timer_wait(4000,FLAG_M_TIMER);
            busy_work(0x02,50);
            break;
         case MSG_EMPLtoMNGR_STATUS: {
            PRESTO_ENVELOPE_T * send_p;
            send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
            presto_mail_send_to_task(pres_tid,send_p,MSG_MNGRtoPRES_STATUS,NULL);
            } break;
      }
      presto_memory_free((BYTE *)recv_p);
   }
}

////////////////////////////////////////////////////////////////////////////////

void employee(void) {
   static PRESTO_MAILBOX_T empl_mbox;
   presto_wait(FLAG_ALL_INIT);
   // initialize here
   presto_mailbox_init(&empl_mbox,FLAG_E_MAIL);
   // done initializing
   presto_trigger_send(ctrl_tid,FLAG_ALL_INIT);

   while (1) {
      PRESTO_ENVELOPE_T * recv_p;
      recv_p=presto_mail_wait(&empl_mbox);
      switch(presto_envelope_message(recv_p)) {
         case MSG_CTRLtoEMPL_LOOP:
            presto_timer_wait(1000,FLAG_E_TIMER);
            use_copier(0x04,50);
            break;
         case MSG_STUDtoEMPL_STATUS: {
            PRESTO_ENVELOPE_T * send_p;
            send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
            presto_mail_send_to_task(mngr_tid,send_p,MSG_EMPLtoMNGR_STATUS,NULL);
            } break;
      }
      presto_memory_free((BYTE *)recv_p);
   }
}

////////////////////////////////////////////////////////////////////////////////

void student(void) {
   static PRESTO_MAILBOX_T stud_mbox;
   PRESTO_TIMER_T ticker1;

   presto_wait(FLAG_ALL_INIT);
   // initialize here
   presto_mailbox_init(&stud_mbox,FLAG_S_MAIL);
   presto_timer_start(&ticker1,0,250,FLAG_S_TIMER);
   // done initializing
   presto_trigger_send(ctrl_tid,FLAG_ALL_INIT);

   int count=0;
   while (1) {
      presto_wait(FLAG_S_TIMER);

      // throw away any mail in the student mailbox
      PRESTO_ENVELOPE_T * recv_p;
      while ((recv_p=presto_mail_get(&stud_mbox))!=NULL) {
         presto_memory_free((BYTE *)recv_p);
      }

      MASKNOT(lights,0x08);
      assert_lights();

      if(++count==25) {
         count=0;
         PRESTO_ENVELOPE_T * send_p;
         send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
         presto_mail_send_to_task(empl_tid,send_p,MSG_STUDtoEMPL_STATUS,NULL);
      }

   }
}

////////////////////////////////////////////////////////////////////////////////

static void memdump(BYTE * start, int size) {
   int count;
   int column=0;
   for(count=0;count<size;count++) {
      string_IntegerToHex(start[count],prt,2);
      serial_send_string(prt);
      if(++column<16) {
         serial_send_string(" ");
      } else {
         serial_send_string("\r\n");
         column=0;
      }
   }
   if(column!=0) serial_send_string("\r\n");
   serial_send_string("\r\n");
}

////////////////////////////////////////////////////////////////////////////////

KERNEL_MEMORYPOOLSTATS_T poolstats;

static void mempool_info(void) {

   int pool;
   for(pool=0;pool<PRESTO_MEM_NUMPOOLS;pool++) {

      memory_debug(pool, &poolstats);

      serial_send_string("pool=");
      string_IntegerToString(pool,prt,PRT);
      serial_send_string(prt);
      serial_send_string("\r\n");

      serial_send_string("SIZE ");
      #ifdef FEATURE_MEMORY_STATISTICS
         serial_send_string("max/");
      #endif
      serial_send_string("limit=");
      #ifdef FEATURE_MEMORY_STATISTICS
         string_IntegerToString(poolstats.max_requested_size,prt,PRT);
         serial_send_string(prt);
         serial_send_string("/");
      #endif
      string_IntegerToString(poolstats.mempool_item_size,prt,PRT);
      serial_send_string(prt);
      serial_send_string("\r\n");


      serial_send_string("QTY current/");
      #ifdef FEATURE_MEMORY_STATISTICS
         serial_send_string("max/");
      #endif
      serial_send_string("limit=");
      string_IntegerToString(poolstats.current_used_items,prt,PRT);
      serial_send_string(prt);
      serial_send_string("/");
      #ifdef FEATURE_MEMORY_STATISTICS
         string_IntegerToString(poolstats.max_used_items,prt,PRT);
         serial_send_string(prt);
         serial_send_string("/");
      #endif
      string_IntegerToString(poolstats.mempool_num_items,prt,PRT);
      serial_send_string(prt);
      serial_send_string("\r\n");


      #ifdef FEATURE_MEMORY_STATISTICS
         serial_send_string("current_total_bytes=");
         string_IntegerToString(poolstats.current_total_bytes,prt,PRT);
         serial_send_string(prt);
         serial_send_string("\r\n");
      #endif

      serial_send_string("\r\n");
   }
   serial_send_string("\r\n");
}

////////////////////////////////////////////////////////////////////////////////

static void debugger(void) {
   extern BYTE initial_stack[];

   serial_init(dbug_tid,FLAG_D_SERIALTX,FLAG_D_SERIALRX);
   serial_send_string("hello world\r\n");

   serial_send_string("ctrl_tid=");
   string_IntegerToString(ctrl_tid,prt,PRT);
   serial_send_string(prt);
   serial_send_string("\r\n");

   serial_send_string("pres_tid=");
   string_IntegerToString(pres_tid,prt,PRT);
   serial_send_string(prt);
   serial_send_string("\r\n");

   serial_send_string("mngr_tid=");
   string_IntegerToString(mngr_tid,prt,PRT);
   serial_send_string(prt);
   serial_send_string("\r\n");

   serial_send_string("empl_tid=");
   string_IntegerToString(empl_tid,prt,PRT);
   serial_send_string(prt);
   serial_send_string("\r\n");

   serial_send_string("stud_tid=");
   string_IntegerToString(stud_tid,prt,PRT);
   serial_send_string(prt);
   serial_send_string("\r\n");

   serial_send_string("dbug_tid=");
   string_IntegerToString(dbug_tid,prt,PRT);
   serial_send_string(prt);
   serial_send_string("\r\n");

   presto_timer_wait(500,FLAG_D_TIMER);

   serial_send_string("initial_stack:\r\n");
   memdump(initial_stack,BOOT_INITIALSTACKSIZE);

   int stack=0;
   while (1) {
      mempool_info();
      presto_timer_wait(500,FLAG_D_TIMER);

      switch(stack++) {
         case 0:
            serial_send_string("pres stack:\r\n");
            memdump(pres_stack, STACK_SIZE);
            break;

         case 1:
            serial_send_string("mngr stack:\r\n");
            memdump(mngr_stack, STACK_SIZE);
            break;

         case 2:
            serial_send_string("empl stack:\r\n");
            memdump(empl_stack, STACK_SIZE);
            break;

         case 3:
            serial_send_string("stud stack:\r\n");
            memdump(stud_stack, STACK_SIZE);
            break;

         case 4:
            serial_send_string("ctrl stack:\r\n");
            memdump(ctrl_stack, STACK_SIZE);
            break;

         case 5:
            serial_send_string("dbug stack:\r\n");
            memdump(dbug_stack, STACK_SIZE);
            break;

         default:
            serial_send_string("---\r\n\r\n");
            stack=0;
            break;
      }
      presto_timer_wait(500,FLAG_D_TIMER);

   }
}

////////////////////////////////////////////////////////////////////////////////

int main(void) {

   assert_lights();
   presto_init();
   pres_tid=presto_task_create(president,  pres_stack,  STACK_SIZE, 14);
   mngr_tid=presto_task_create(manager,    mngr_stack,  STACK_SIZE, 13);
   empl_tid=presto_task_create(employee,   empl_stack,  STACK_SIZE, 12);
   stud_tid=presto_task_create(student,    stud_stack,  STACK_SIZE, 11);
   ctrl_tid=presto_task_create(control,    ctrl_stack,  STACK_SIZE, 99);
   dbug_tid=presto_task_create(debugger,   dbug_stack,  STACK_SIZE, 50);
   presto_scheduler_start();

   // we never get here
   error_fatal(ERROR_MAIN_AFTERSTART);
   return 0;
}

////////////////////////////////////////////////////////////////////////////////


