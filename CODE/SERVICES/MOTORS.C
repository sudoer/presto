
#include <hc11.h>
#include "hc11regs.h"
#include "presto.h"
#include "services.h"
#include "services\motors.h"

////////////////////////////////////////////////////////////////////////////////
//
//  The motor controllers are memory-mapped to location 0x7FFF.
//  The bits are arranged as follows:
//
//     0x80 - motor 3 on      0x08 - motor 3 reverse
//     0x40 - motor 2 on      0x04 - motor 2 reverse
//     0x20 - motor 1 on      0x02 - motor 1 reverse
//     0x10 - motor 0 on      0x01 - motor 0 reverse
//
////////////////////////////////////////////////////////////////////////////////

#define MOTOR_PORT *(unsigned char *)(0x7FFF)
#define MOTOR_CYCLES 20001

#define FWD 0x00
#define REV 0x0F

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

////////////////////////////////////////////////////////////////////////////////

static void start_motor_timer(void);
static void restart_motor_timer(void);
static void apply_motor_pwm(void);

////////////////////////////////////////////////////////////////////////////////

void motor_init(void) {
   MOTOR_PORT=0x00;
   motor_speed(0,0);
   motor_speed(1,0);
   motor_speed(2,0);
   motor_speed(3,0);
   // start_motor_timer();
}

////////////////////////////////////////////////////////////////////////////////

void motor_speed(BYTE motor, signed char speed) {
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
      current_motor_speed[motor]=speed;
   }
   apply_motor_pwm();
}

////////////////////////////////////////////////////////////////////////////////

static void start_motor_timer(void) {

   // store (current plus MOTOR_CYCLES)
   TOC3 = TCNT + MOTOR_CYCLES;

   // request output compare interrupt
   TMSK1 |= TMSK1_OC3I;

   // clear the OUTPUT COMPARE flag
   // writing O's makes no change, writing 1's clears the bit
   TFLG1 = TFLG1_OC3F;

   // counter disconnected from output pin logic
   TCTL1 &= ~(TCTL1_OM3|TCTL1_OL3);
}

////////////////////////////////////////////////////////////////////////////////

static void restart_motor_timer(void) {
   // store (last plus MOTOR_CYCLES)
   TOC3 = TOC3 + MOTOR_CYCLES;

   // clear the OUTPUT COMPARE flag
   // writing O's makes no change, writing 1's clears the bit
   TFLG1 = TFLG1_OC3F;
}

////////////////////////////////////////////////////////////////////////////////

void motor_isr(void) {
   apply_motor_pwm();
   restart_motor_timer();
}

////////////////////////////////////////////////////////////////////////////////

static void apply_motor_pwm(void) {
   BYTE ctrl=0x00;
   BYTE m;
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

