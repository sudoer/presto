
#ifndef _I2C_H_
#define _I2C_H_

#include "types.h"

////////////////////////////////////////////////////////////////////////////////

// FUNCTION PROTOTYPES

extern void presto_i2c_init(void);
extern void presto_i2c_send(BYTE * b, BYTE count);
extern void presto_i2c_stop(void);
extern void presto_i2c_send_byte(BYTE b);
extern void presto_i2c_start(void);

////////////////////////////////////////////////////////////////////////////////

#endif

