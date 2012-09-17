////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "avr_regs.h"
#include <avr/io.h>


////////////////////////////////////////////////////////////////////////////////
//   E X P O R T E D   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void bad_intr() __attribute__((naked));
void bad_intr() {
   while(1) {
      outb(PORTB,0x00);
      outb(PORTB,0xFF);
   }
}

////////////////////////////////////////////////////////////////////////////////


void bad_intr_1()  { while(1) { outb(PORTB,0xFF^0x01); outb(PORTB,0xFF); } }
void bad_intr_2()  { while(1) { outb(PORTB,0xFF^0x02); outb(PORTB,0xFF); } }
void bad_intr_3()  { while(1) { outb(PORTB,0xFF^0x03); outb(PORTB,0xFF); } }
void bad_intr_4()  { while(1) { outb(PORTB,0xFF^0x04); outb(PORTB,0xFF); } }
void bad_intr_5()  { while(1) { outb(PORTB,0xFF^0x05); outb(PORTB,0xFF); } }
void bad_intr_6()  { while(1) { outb(PORTB,0xFF^0x06); outb(PORTB,0xFF); } }
void bad_intr_7()  { while(1) { outb(PORTB,0xFF^0x07); outb(PORTB,0xFF); } }
void bad_intr_8()  { while(1) { outb(PORTB,0xFF^0x08); outb(PORTB,0xFF); } }
void bad_intr_9()  { while(1) { outb(PORTB,0xFF^0x09); outb(PORTB,0xFF); } }
void bad_intr_10() { while(1) { outb(PORTB,0xFF^0x0A); outb(PORTB,0xFF); } }

