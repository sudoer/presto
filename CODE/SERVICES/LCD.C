
#include "services\i2c.h"
#include "services.h"
#include "presto.h"
#include "types.h"
#include "priority.h"

////////////////////////////////////////////////////////////////////////////////

// FUNCTION PROTOTYPES

static void send_entire_display(void);
static void send_one_char(BYTE r, BYTE c);

////////////////////////////////////////////////////////////////////////////////

// STATIC GLOBALS

static BYTE current_row=0;
static BYTE current_col=0;
static BYTE display[LCD_ROWS][LCD_COLS];

////////////////////////////////////////////////////////////////////////////////

void lcd_init(void) {
   BYTE init[]={0X74,0X00,0X3E,0X0C,0X06,0XA0};
   presto_i2c_init();
   presto_i2c_send(init,6);
   lcd_clear();
   send_entire_display();
}

////////////////////////////////////////////////////////////////////////////////

void lcd_clear(void) {
   BYTE r;
   BYTE c;
   for(r=0;r<LCD_ROWS;r++) {
      for(c=0;c<LCD_COLS;c++) {
         display[r][c]=LCD_SPACE;
      }
   }
   current_row=0;
   current_col=0;
}

////////////////////////////////////////////////////////////////////////////////

void lcd_print_at(BYTE row, BYTE col, char * string) {
   lcd_goto_xy(row,col);
   lcd_print_at(row,col,string);
}

////////////////////////////////////////////////////////////////////////////////

void lcd_goto_xy(BYTE row, BYTE col) {
   if(col>=LCD_COLS) return;
   if(row>=LCD_ROWS) return;
   current_row=row;
   current_col=col;
}

////////////////////////////////////////////////////////////////////////////////

void lcd_print(char * string) {
   BYTE p=0;
   while(string[p]!=0) {
      display[current_row][current_col]=ascii_to_lcd(string[p]);
      current_col++;
      if(current_col>=LCD_COLS) {
         current_col=0;
         current_row++;
         if(current_row>=LCD_ROWS) {
            current_row=0;
         }
      }
      p++;
   }
   send_entire_display();
}

////////////////////////////////////////////////////////////////////////////////

BYTE ascii_to_lcd(char c) {
   BYTE b=(BYTE)c;
   if(b=='$') {
      return 0x82;
   } else if(b=='@') {
      return 0xC0;
   } else if((b>='!')&&(b<='Z')) {
      return b-'!'+0xA1;
   } else if((b>='a')&&(b<='z')) {
      return b-'a'+0xE1;
   } else {
      return LCD_SPACE;
   }
}

////////////////////////////////////////////////////////////////////////////////

void lcd_raw_char(BYTE row, BYTE col, BYTE b) {
   if(col>=LCD_COLS) return;
   if(row>=LCD_ROWS) return;
   display[row][col]=b;
   send_entire_display();
   send_one_char(row,col);
}

////////////////////////////////////////////////////////////////////////////////

static void send_entire_display(void) {
   BYTE r;
   BYTE c;

   for(r=0;r<LCD_ROWS;r++) {
      presto_i2c_start();
      presto_i2c_send_byte(0x74);             // address of our LCD
      presto_i2c_send_byte(0x00);             // control byte - command mode
      presto_i2c_send_byte(0x80|((r+1)<<5));  // set DDRAM address
      presto_i2c_stop();

      presto_i2c_start();
      presto_i2c_send_byte(0x74);             // address of our LCD
      presto_i2c_send_byte(0x40);             // control byte - data mode
      for(c=0;c<LCD_COLS;c++) {
         presto_i2c_send_byte(display[r][c]);
      }
      presto_i2c_stop();
   }
}

////////////////////////////////////////////////////////////////////////////////

static void send_one_char(BYTE r, BYTE c) {
   presto_i2c_start();
   presto_i2c_send_byte(0x74);               // address of our LCD
   presto_i2c_send_byte(0x00);               // control byte - command mode
   presto_i2c_send_byte(0x80|((r+1)<<5)+c);  // set DDRAM address
   presto_i2c_stop();

   presto_i2c_start();
   presto_i2c_send_byte(0x74);             // address of our LCD
   presto_i2c_send_byte(0x40);             // control byte - data mode
   presto_i2c_send_byte(display[r][c]);
   presto_i2c_stop();
}

////////////////////////////////////////////////////////////////////////////////

