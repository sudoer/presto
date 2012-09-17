
#include "presto.h"
#include "types.h"
#include "hc11_regs.h"
#include "error.h"
#include "handyboard.h"
#include "services/serial.h"
#include "services/string.h"

////////////////////////////////////////////////////////////////////////////////

#define STACK_SIZE 0x100

static BYTE motor_stack[STACK_SIZE];
static BYTE ctrl_stack[STACK_SIZE];

////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////

#define MOTORS_NUM_MOTORS  4
#define MOTORS_MAX_SPEED   6
#define FWD 0x00
#define REV 0x0F
#define MOTOR_PORT *(unsigned char *)(0x7FFF)


#define FLAG_ALL_INIT      0xFF

#define FLAG_CONTROL_TIMER   0x01
#define FLAG_CONTROL_MAIL    0x02

#define FLAG_MOTOR_TIMER   0x01
#define FLAG_MOTOR_MAIL    0x02


////////////////////////////////////////////////////////////////////////////////
//   T Y P E   D E F I N I T I O N S
////////////////////////////////////////////////////////////////////////////////

typedef struct MOTORSTRUCT_S {
   uint8 motor;
   sint8 speed;
} MOTORSTRUCT_T;

enum {
   MSG_CHANGEMOTOR,
   MSG_QUERYMOTOR,
   MSG_MOTORSPEED,
};

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   G L O B A L   D A T A
////////////////////////////////////////////////////////////////////////////////

static WORD speed_2_pwm[MOTORS_MAX_SPEED+1]={
   0x0000,     // 0 0000000000000000   0/16
   0x8080,     // 1 1000000010000000   2/16
   0x8888,     // 2 1000100010001000   4/16
   0xAAAA,     // 3 1010101010101010   8/16
   0xEEEE,     // 4 1110111011101110  12/16
   0xFEFE,     // 5 1111111011111110  14/16
   0xFFFF      // 6 1111111111111111  16/16
};

static BYTE current_motor_speed[MOTORS_NUM_MOTORS]={0,0,0,0};
static BYTE current_motor_dir[MOTORS_NUM_MOTORS]={FWD,FWD,FWD,FWD};
static PRESTO_TASKID_T motor_tid;
static PRESTO_TASKID_T ctrl_tid;

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N   P R O T O T Y P E S
////////////////////////////////////////////////////////////////////////////////

void motor_set_speed(uint8 motor, sint8 speed);
sint8 motor_get_speed(uint8 motor);
static void apply_motor_pwm(void);


////////////////////////////////////////////////////////////////////////////////

void control(void) {

   static PRESTO_MAILBOX_T ctrl_mbox;

   presto_mailbox_init(&ctrl_mbox,FLAG_CONTROL_MAIL);

   // We must pass control to each task, one by one, to give them a chance to
   // initialize their mailboxes and other resources.  Otherwise, if we tried
   // to send them a message, we would crash before they received it.

   presto_trigger_send(motor_tid,FLAG_ALL_INIT);
   presto_wait(FLAG_ALL_INIT);

   // We are ready to run normally now.

   PRESTO_ENVELOPE_T * send_p;
   PRESTO_ENVELOPE_T * recv_p;
   MOTORSTRUCT_T * command;

   send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
   command=(MOTORSTRUCT_T *)presto_memory_allocate(sizeof(MOTORSTRUCT_T));
   command->motor=0;
   command->speed=MOTORS_MAX_SPEED;
   presto_mail_send_to_task(motor_tid,send_p,MSG_CHANGEMOTOR,command);
   presto_timer_wait(1000,FLAG_CONTROL_TIMER);

   send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
   command=(MOTORSTRUCT_T *)presto_memory_allocate(sizeof(MOTORSTRUCT_T));
   command->motor=1;
   command->speed=1;
   presto_mail_send_to_task(motor_tid,send_p,MSG_CHANGEMOTOR,command);
   presto_timer_wait(1000,FLAG_CONTROL_TIMER);

   send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
   command=(MOTORSTRUCT_T *)presto_memory_allocate(sizeof(MOTORSTRUCT_T));
   command->motor=2;
   command->speed=2;
   presto_mail_send_to_task(motor_tid,send_p,MSG_CHANGEMOTOR,command);
   presto_timer_wait(1000,FLAG_CONTROL_TIMER);

   send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
   command=(MOTORSTRUCT_T *)presto_memory_allocate(sizeof(MOTORSTRUCT_T));
   command->motor=3;
   command->speed=3;
   presto_mail_send_to_task(motor_tid,send_p,MSG_CHANGEMOTOR,command);
   presto_timer_wait(1000,FLAG_CONTROL_TIMER);

   while (1) {
      uint8 m;

      for(m=0;m<MOTORS_NUM_MOTORS;m++) {
         command=(MOTORSTRUCT_T *)presto_memory_allocate(sizeof(MOTORSTRUCT_T));
         command->motor=m;
         send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
         presto_mail_send_to_task(motor_tid,send_p,MSG_QUERYMOTOR,command);

         recv_p=presto_mail_wait(&ctrl_mbox);

         command=(MOTORSTRUCT_T *)presto_envelope_payload(recv_p);
         // re-use command structure
         command->motor=m;
         command->speed=0-command->speed;
         send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
         presto_mail_send_to_task(motor_tid,send_p,MSG_CHANGEMOTOR,command);

         presto_memory_free((BYTE *)recv_p);

         presto_timer_wait(1000,FLAG_CONTROL_TIMER);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

//  The motor controllers are memory-mapped to location 0x7FFF.
//  The bits are arranged as follows:
//
//     0x80 - motor 3 on      0x08 - motor 3 reverse
//     0x40 - motor 2 on      0x04 - motor 2 reverse
//     0x20 - motor 1 on      0x02 - motor 1 reverse
//     0x10 - motor 0 on      0x01 - motor 0 reverse

void motor(void) {
   static PRESTO_MAILBOX_T motor_mbox;
   PRESTO_TIMER_T pwm_timer;

   presto_wait(FLAG_ALL_INIT);

   // initialize here

   presto_mailbox_init(&motor_mbox,FLAG_MOTOR_MAIL);

   MOTOR_PORT=0x00;
   motor_set_speed(0,0);
   motor_set_speed(1,0);
   motor_set_speed(2,0);
   motor_set_speed(3,0);
   apply_motor_pwm();

   presto_timer_start(&pwm_timer,0,2,FLAG_MOTOR_TIMER);

   // done initializing

   presto_trigger_send(ctrl_tid,FLAG_ALL_INIT);

   while (1) {
      PRESTO_TRIGGER_T t;
      t=presto_wait(FLAG_MOTOR_MAIL|FLAG_MOTOR_TIMER);

      if (t&FLAG_MOTOR_MAIL) {
         PRESTO_ENVELOPE_T * recv_p;
         recv_p=presto_mail_get(&motor_mbox);
         switch(presto_envelope_message(recv_p)) {
            case MSG_CHANGEMOTOR: {
               MOTORSTRUCT_T * parms;
               parms=(MOTORSTRUCT_T *)presto_envelope_payload(recv_p);
               motor_set_speed(parms->motor, parms->speed);
               presto_memory_free((BYTE *)parms);
               } break;
            case MSG_QUERYMOTOR: {
               PRESTO_ENVELOPE_T * send_p;
               MOTORSTRUCT_T * parms;
               parms=(MOTORSTRUCT_T *)presto_envelope_payload(recv_p);
               // re-use parms structure
               parms->speed=motor_get_speed(parms->motor);
               send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
               presto_mail_send_to_task(presto_envelope_sender(recv_p),send_p,MSG_MOTORSPEED,parms);
               } break;
         }
         presto_memory_free((BYTE *)recv_p);
      }

      if (t&FLAG_MOTOR_TIMER) {
         apply_motor_pwm();
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

int main(void) {
   presto_init();
   motor_tid=presto_task_create(motor,   motor_stack,  STACK_SIZE, 100);
   ctrl_tid=presto_task_create(control,  ctrl_stack,   STACK_SIZE, 99);
   presto_scheduler_start();
   return 0;
}

////////////////////////////////////////////////////////////////////////////////

sint8 motor_get_speed(uint8 motor) {
   sint8 speed;
   if(motor>=MOTORS_NUM_MOTORS) return 0;
   speed=(sint8)current_motor_speed[motor];
   if(current_motor_dir[motor]==REV) return (0-speed);
   return speed;
}

////////////////////////////////////////////////////////////////////////////////

void motor_set_speed(uint8 motor, sint8 speed) {
   if(motor<MOTORS_NUM_MOTORS) {
      // If intr happens while we're in this function and we happen to be
      // changing directions (say from FULL REVERSE to STOP FORWARD), do not
      // allow it reverse the motor in mid-function call.  The above case would
      // cause a transition like this: FULL-REV to FULL-FWD to STOP-FWD.
      current_motor_speed[motor]=0;

      if(speed>=0) {
         current_motor_dir[motor]=FWD;
      } else {
         speed=0-speed;
         current_motor_dir[motor]=REV;
      }
      if(speed>MOTORS_MAX_SPEED) speed=MOTORS_MAX_SPEED;
      current_motor_speed[motor]=(uint8)speed;
   }
   apply_motor_pwm();
}

////////////////////////////////////////////////////////////////////////////////

static void apply_motor_pwm(void) {
   BYTE ctrl=0x00;
   uint8 m;
   static WORD speed_mask=0x0001;
   BYTE motor_mask=0x11;

   for(m=0;m<MOTORS_NUM_MOTORS;m++) {
      if(speed_2_pwm[current_motor_speed[m]]&speed_mask) {
         ctrl=ctrl|((0xF0|current_motor_dir[m])&motor_mask);
      }
      motor_mask<<=1;
   }
   speed_mask<<=1;
   if(speed_mask==0) speed_mask=0x0001;
   MOTOR_PORT=ctrl;
}

////////////////////////////////////////////////////////////////////////////////



