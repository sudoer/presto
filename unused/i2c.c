
#include "hc11regs.h"
#include "presto.h"
#include "services\i2c.h"
#include "types.h"

////////////////////////////////////////////////////////////////////////////////

#define SCL_BIT  2
#define SDA_BIT  5
#define SCL_PORT PORTD
#define SDA_PORT PORTD
#define SCL_DDR  DDRD
#define SDA_DDR  DDRD

////////////////////////////////////////////////////////////////////////////////

void presto_i2c_init(void) {
   // set up lines
   BITSET(SCL_DDR,SCL_BIT);
   BITSET(SDA_DDR,SDA_BIT);
   // send initial STOP condition
   BITCLR(SDA_PORT,SDA_BIT);
   BITSET(SCL_PORT,SCL_BIT);
   BITSET(SDA_PORT,SDA_BIT);
}

////////////////////////////////////////////////////////////////////////////////

void presto_i2c_send(BYTE * b, BYTE count) {
   BYTE c=0;
   presto_i2c_start();
   while(c<count) {
      presto_i2c_send_byte(b[c++]);
   }
   presto_i2c_stop();
}

////////////////////////////////////////////////////////////////////////////////

void presto_i2c_start(void) {
   BITCLR(SDA_PORT,SDA_BIT);
   BITCLR(SCL_PORT,SCL_BIT);
}

////////////////////////////////////////////////////////////////////////////////

void presto_i2c_send_byte(BYTE b) {
   BYTE mask;
   for(mask=0x80;mask;mask>>=1) {
      // present the data bit while the clock is low
      if(b&mask) {
         BITSET(SDA_PORT,SDA_BIT);
      } else {
         BITCLR(SDA_PORT,SDA_BIT);
      }
      // pulse clock line
      BITSET(SCL_PORT,SCL_BIT);
      BITCLR(SCL_PORT,SCL_BIT);
   }
   // send ACK pulse
   BITCLR(SDA_DDR,SDA_BIT);
   BITSET(SCL_PORT,SCL_BIT);
   BITCLR(SCL_PORT,SCL_BIT);
   BITSET(SDA_DDR,SDA_BIT);
}

////////////////////////////////////////////////////////////////////////////////

void presto_i2c_stop(void) {
   BITCLR(SDA_PORT,SDA_BIT);
   BITSET(SCL_PORT,SCL_BIT);
   BITSET(SDA_PORT,SDA_BIT);
}

////////////////////////////////////////////////////////////////////////////////


