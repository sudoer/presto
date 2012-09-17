////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include <avr/io.h>
#include "registers.h"
#include "types.h"
#include "cpu_timer.h"


////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////

// These settings depend on your hardware platform

// internal RC frequency = 8.0 MHz
// fuse scaler = 8, clock prescaler = 1 -> CPU clock = 1.0 MHz   <-- so I thought
// fuse scaler = 1, clock prescaler = 1 -> CPU clock = 8.0 MHz   <-- a measured guess?
#define CYCLES_PER_MS       8096
#define TIMER_PRESCALE      256
#define CLOCKS_PER_MS       CYCLES_PER_MS/TIMER_PRESCALE   // = 32

// CLOCKS_PER_MS = 1000/64 = 15.625 -> 15
// PRESTO_KERNEL_MSPERTICK = 10 (see configure.h)
// set OCRA0 to (CLOCKS_PER_MS*ms) = 15*10 = 150
// 150 *WILL* fit into one byte
#define TIMER0

#ifdef TIMER0
// prescaler
#if (TIMER_PRESCALE==0)
   #define PRESCALER         0x00
#elif (TIMER_PRESCALE==1)
   #define PRESCALER         0x01
#elif (TIMER_PRESCALE==8)
   #define PRESCALER         0x02
#elif (TIMER_PRESCALE==64)
   #define PRESCALER         0x03
#elif (TIMER_PRESCALE==256)
   #define PRESCALER         0x04
#elif (TIMER_PRESCALE==1024)
   #define PRESCALER         0x05
#else
   #error "bad timer prescaler value"
#endif
#endif

#ifdef TIMER1
// prescaler
#if (TIMER_PRESCALE==0)
   #define PRESCALER         0x00
#elif (TIMER_PRESCALE==1)
   #define PRESCALER         0x01
#elif (TIMER_PRESCALE==8)
   #define PRESCALER         0x02
#elif (TIMER_PRESCALE==64)
   #define PRESCALER         0x03
#elif (TIMER_PRESCALE==256)
   #define PRESCALER         0x04
#elif (TIMER_PRESCALE==1024)
   #define PRESCALER         0x05
#else
   #error "bad timer prescaler value"
#endif
#endif

#ifdef TIMER2
// prescaler
#if (TIMER_PRESCALE==0)
   #define PRESCALER         0x00
#elif (TIMER_PRESCALE==1)
   #define PRESCALER         0x01
#elif (TIMER_PRESCALE==8)
   #define PRESCALER         0x02
#elif (TIMER_PRESCALE==32)
   #define PRESCALER         0x03
#elif (TIMER_PRESCALE==64)
   #define PRESCALER         0x04
#elif (TIMER_PRESCALE==128)
   #define PRESCALER         0x05
#elif (TIMER_PRESCALE==256)
   #define PRESCALER         0x06
#elif (TIMER_PRESCALE==1024)
   #define PRESCALER         0x07
#else
   #error "bad timer prescaler value"
#endif
#endif

#ifndef PRESCALER
   #error "at least one timer should be used: TIMER0, TIMER1, TIMER2"
#endif

////////////////////////////////////////////////////////////////////////////////
//   E X P O R T E D   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void hwtimer_start(unsigned short ms, void (*func)(void)) {

#ifdef TIMER0
   // bit 7 - force output compare A (0)
   // bit 6,3 - waveform generation mode (0,1 = CTC)
   // bit 5-4 - output compare behavior (0,0 = OC0A disconnected)
   // bit 2-0 - clock select, prescaler
   outb(TCCR0A,(1<<WGM01)|(PRESCALER<<CS00));
   // reset counter
   outb(TCNT0,0x00);
   // set period
   outw(OCR0A,(unsigned char)(CLOCKS_PER_MS*ms));
   // enable interrupt
   sbi(TIMSK0,OCIE0A);
#endif


#ifdef TIMER1
   // do not toggle OC1A, do not toggle OC1B, Clear Timer on Compare A (CTC)
   outb(TCCR1A,(0<<COM1A0)|(0<<COM1B0)|(0<<WGM10));
   // waveform generation = CTC, set prescaler
   outb(TCCR1B,(1<<WGM12)|(PRESCALER<<CS10));
   // set timer output compare 1A
   outw(OCRA1,(unsigned short)(CLOCKS_PER_MS*ms));
   // enable OC1A interrupt
   sbi(TIMSK1,OCIE1A);
#endif


#ifdef TIMER2
   // bit 7 - force output compare A (0)
   // bit 6,3 - waveform generation mode (0,1 = CTC)
   // bit 5-4 - output compare behavior (0,0 = OC2A disconnected)
   // bit 2-0 - clock select, prescaler
   outb(TCCR2A,(1<<WGM21)|PRESCALER);
   // reset counter
   outb(TCNT2,0x00);
   // set output compare register
   outb(OCR2A,(BYTE)(CLOCKS_PER_MS*ms));
   // enable compare interrupt, disable overflow interrupt
   outb(TIMSK2,(1<<OCIE2A));
#endif

}

////////////////////////////////////////////////////////////////////////////////

void hwtimer_restart(void) {
#ifdef TIMER0
   // re-enable interrupt
   sbi(TIMSK0,OCIE0A);
#endif
#ifdef TIMER1
   // re-enable OC1A interrupt
   sbi(TIMSK1,OCIE1A);
#endif
#ifdef TIMER2
   // re-enable compare interrupt, disable overflow interrupt
   outb(TIMSK2,(1<<OCIE2A));
#endif
}

////////////////////////////////////////////////////////////////////////////////

