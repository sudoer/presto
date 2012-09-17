
#include "serial.h"
#include "string.h"

////////////////////////////////////////////////////////////////////////////////

#define PRT  20
char prt[PRT];

////////////////////////////////////////////////////////////////////////////////

void debug_init(void) {
   serial_init(0,0,0);
   serial_send_string("hello world\r\n");
}

////////////////////////////////////////////////////////////////////////////////

void debug_string(char * string) {
   serial_send_string(string);
}

////////////////////////////////////////////////////////////////////////////////

void debug_int(int i) {
   string_IntegerToString(i,prt,PRT);
   serial_send_string(prt);
}

////////////////////////////////////////////////////////////////////////////////


