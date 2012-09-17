
#ifndef _CPU_TIMER_H_
#define _CPU_TIMER_H_

////////////////////////////////////////////////////////////////////////////////

#include <avr/io.h>
#include "registers.h"
////////////////////////////////////////////////////////////////////////////////

// system timer
extern void hwtimer_start(unsigned short ms, void (*func)(void));

////////////////////////////////////////////////////////////////////////////////
//   T I M E R   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

#define CPU_TIMER_START(ms,func)   hwtimer_start(ms,func);
#define CPU_TIMER_RESTART()        ;

////////////////////////////////////////////////////////////////////////////////

#endif // _CPU_TIMER_H_
