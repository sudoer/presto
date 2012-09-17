
#ifndef _MISC_HW_H_
#define _MISC_HW_H_

////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "chip/hc11regs.h"

////////////////////////////////////////////////////////////////////////////////

// this is the memory location for the motor controller
//   0x80 - motor 3 on    0x08 - motor 3 reverse
//   0x40 - motor 2 on    0x04 - motor 2 reverse
//   0x20 - motor 1 on    0x02 - motor 1 reverse
//   0x10 - motor 0 on    0x01 - motor 0 reverse
#define MOTOR_LED_PORT *(unsigned char *)(0x7FFF)

#define TOGGLE_SPEAKER()  BITNOT(PORTA,3);  // toggle speaker

////////////////////////////////////////////////////////////////////////////////

#endif

