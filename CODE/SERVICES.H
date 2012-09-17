
#ifndef SERVICES_H
#define SERVICES_H

#include "types.h"

////////////////////////////////////////////////////////////////////////////////

// LCD

#define LCD_ROWS 3
#define LCD_COLS 12

#define LCD_SPACE                0xA0
#define LCD_BATTERY_BOTTOM_2     0x10
#define LCD_BATTERY_BOTTOM_3     0x11
#define LCD_BATTERY_BOTTOM_4     0x12
#define LCD_BATTERY_BOTTOM_5     0x13
#define LCD_BATTERY_BOTTOM_6     0x14
#define LCD_BATTERY_BOTTOM_7     0x3D
#define LCD_BATTERY_TOP_0        0x20
#define LCD_BATTERY_TOP_1        0x21
#define LCD_BATTERY_TOP_2        0x22
#define LCD_BATTERY_TOP_3        0x23
#define LCD_BATTERY_TOP_4        0x24
#define LCD_BATTERY_TOP_5        0x25
#define LCD_BATTERY_TOP_6        0x26
#define LCD_PISTON_1             0x15
#define LCD_SINGLE_BAR_1         0x27
#define LCD_KEY                  0x3E
#define LCD_BELL                 0x3F
#define LCD_LEFT                 0x2F
#define LCD_RIGHT                0x2E
#define LCD_UP                   0x1D
#define LCD_DOWN                 0x1E
#define LCD_UP_DOWN              0x1F
#define LCD_BALL                 0x3C
#define LCD_SOLID                0x3D

extern void lcd_init(void);
extern void lcd_clear(void);
extern void lcd_goto_xy(BYTE row, BYTE col);
extern void lcd_print_at(BYTE row, BYTE col, char * string);
extern void lcd_print(char * string);
extern void lcd_raw_char(BYTE row, BYTE col, BYTE b);
extern BYTE ascii_to_lcd(char c);

////////////////////////////////////////////////////////////////////////////////

// MOTORS

#define MOTORS_MAX_SPEED 6
#define MOTORS_NUM_MOTORS 4
extern void motor_speed(BYTE motor, signed char speed);
void motor_init(void);

////////////////////////////////////////////////////////////////////////////////

// SOUND

void sound_on(unsigned short tone);
void sound_off(void);
void sound_tick(void);

////////////////////////////////////////////////////////////////////////////////

#endif

