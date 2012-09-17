
#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

////////////////////////////////////////////////////////////////////////////////

#include "types.h"

////////////////////////////////////////////////////////////////////////////////

extern void lcd_init(void);
extern void lcd_contrast(BYTE level);

extern void lcd_clear(void);
extern void lcd_dashes(BYTE number, BOOLEAN on);
extern void lcd_numbers(BYTE number, BOOLEAN on);
extern void lcd_text_segments(BYTE position, WORD segments);
extern void lcd_text_digit(BYTE position, char ascii);
extern void lcd_text_string(char * string);

extern void lcd_SOF_interrupt(void);

////////////////////////////////////////////////////////////////////////////////


#endif // LCD_DRIVER_H

