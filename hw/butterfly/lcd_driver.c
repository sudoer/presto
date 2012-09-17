

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "lcd_driver.h"
#include "types.h"

////////////////////////////////////////////////////////////////////////////////

// DEVICE SPECIFIC!!! (ATmega169)
#define pLCDREG ((unsigned char *)(0xEC))
#define LCD_NUM_REGISTERS       20
#define LCD_INITIAL_CONTRAST    0x0F

////////////////////////////////////////////////////////////////////////////////

void lcd_SOF_interrupt(void) __attribute__((interrupt));

////////////////////////////////////////////////////////////////////////////////
//
//
//      AAAAAAA
//     BC  D  EF
//     B C D E F
//     B  CDE  F
//      GGG HHH
//     I  JKL  M
//     I J K L M
//     IJ  K  LM
//      NNNNNNN
//
//
////////////////////////////////////////////////////////////////////////////////

// Look-up table used when converting ASCII to
// LCD display data (segment control)
// mt __flash unsigned int LCD_character_table[] =
//WORD LCD_character_table[] = {
WORD LCD_character_table[] PROGMEM = {
    0x0A51,     // '*' (?)
    0x2A80,     // '+'
    0x0000,     // ',' (Not defined)
    0x0A00,     // '-'
    0x0A51,     // '.' Degree sign
    0x0000,     // '/' (Not defined)
    0x5559,     // '0'
    0x0118,     // '1'
    0x1e11,     // '2
    0x1b11,     // '3
    0x0b50,     // '4
    0x1b41,     // '5
    0x1f41,     // '6
    0x0111,     // '7
    0x1f51,     // '8
    0x1b51,     // '9'
    0x0000,     // ':' (Not defined)
    0x0000,     // ';' (Not defined)
    0x0000,     // '<' (Not defined)
    0x0000,     // '=' (Not defined)
    0x0000,     // '>' (Not defined)
    0x0000,     // '?' (Not defined)
    0x0000,     // '@' (Not defined)
    0x0f51,     // 'A' (+ 'a')
    0x3991,     // 'B' (+ 'b')
    0x1441,     // 'C' (+ 'c')
    0x3191,     // 'D' (+ 'd')
    0x1e41,     // 'E' (+ 'e')
    0x0e41,     // 'F' (+ 'f')
    0x1d41,     // 'G' (+ 'g')
    0x0f50,     // 'H' (+ 'h')
    0x2080,     // 'I' (+ 'i')
    0x1510,     // 'J' (+ 'j')
    0x8648,     // 'K' (+ 'k')
    0x1440,     // 'L' (+ 'l')
    0x0578,     // 'M' (+ 'm')
    0x8570,     // 'N' (+ 'n')
    0x1551,     // 'O' (+ 'o')
    0x0e51,     // 'P' (+ 'p')
    0x9551,     // 'Q' (+ 'q')
    0x8e51,     // 'R' (+ 'r')
    0x9021,     // 'S' (+ 's')
    0x2081,     // 'T' (+ 't')
    0x1550,     // 'U' (+ 'u')
    0x4448,     // 'V' (+ 'v')
    0xc550,     // 'W' (+ 'w')
    0xc028,     // 'X' (+ 'x')
    0x2028,     // 'Y' (+ 'y')
    0x5009,     // 'Z' (+ 'z')
    0x0000,     // '[' (Not defined)
    0x0000,     // '\' (Not defined)
    0x0000,     // ']' (Not defined)
    0x0000,     // '^' (Not defined)
    0x0000      // '_'
};

////////////////////////////////////////////////////////////////////////////////

void lcd_contrast(BYTE level) {
   LCDCCR=0x0F&level;
}

////////////////////////////////////////////////////////////////////////////////

void lcd_init(void) {
             
   lcd_contrast(LCD_INITIAL_CONTRAST);

   // Select asynchronous clock source,
   // enable all COM pins and enable all segment pins.
   LCDCRB = (1<<LCDCS) | (3<<LCDMUX0) | (7<<LCDPM0);

   // Set LCD prescaler to give a framerate of 32,0 Hz
   LCDFRR = (0<<LCDPS0) | (7<<LCDCD0);    

   // Enable LCD and set low power waveform
   LCDCRA = (1<<LCDEN) | (1<<LCDAB);

   //Enable LCD start of frame interrupt
   LCDCRA |= (1<<LCDIE);

   lcd_clear();
}

////////////////////////////////////////////////////////////////////////////////

void lcd_clear(void) {
   BYTE * p;
   int i;

   p=pLCDREG;
   for (i=0; i<LCD_NUM_REGISTERS; i++) {
      *p++ = 0x00;
   }
}

////////////////////////////////////////////////////////////////////////////////

void lcd_numbers(BYTE number, BOOLEAN on) {

}

////////////////////////////////////////////////////////////////////////////////

void lcd_dashes(BYTE number, BOOLEAN on) {

}

////////////////////////////////////////////////////////////////////////////////

void lcd_text_segments(BYTE position, WORD segments) {
   BYTE mask;
   BYTE nibble;
   BYTE * ptr;
   int i;

   // Adjust mask according to LCD segment mapping
   if (position & 0x01) mask = 0x0F;    // position 1,3,5
   else mask = 0xF0;                    // position 0,2,4
   ptr = pLCDREG + (position >> 1);     // position = {0,0,1,1,2,2}

   for (i = 0; i < 4; i++) {
      nibble = segments & 0x000F;
      segments >>= 4;
      if (position & 0x01) nibble <<= 4;
      *ptr = (*ptr & mask) | nibble;
      ptr += 5;
   }
}

////////////////////////////////////////////////////////////////////////////////

void lcd_text_digit(BYTE position, char ascii) {
   WORD segments;
   //Lookup character table for segmet data
   if ((ascii >= '*') && (ascii <= 'z')) {
      // ascii is a letter
      if (ascii >= 'a') ascii &= ~0x20;    // Convert to upper case
      ascii -= '*';
      //segments = LCD_character_table[(unsigned char)ascii]; 
      segments = (WORD)pgm_read_word(&LCD_character_table[(unsigned char)ascii]); 
      lcd_text_segments(position, segments);
   }
}

////////////////////////////////////////////////////////////////////////////////

void lcd_text_string(char * string) {
   BYTE position;
   for(position=0; position<6; position++) {
      if (*string==0) lcd_text_digit(position,' ');
      else lcd_text_digit(position,*string++);
   }
}

////////////////////////////////////////////////////////////////////////////////

void lcd_SOF_interrupt(void) {

}

////////////////////////////////////////////////////////////////////////////////


