
#ifndef _HWTIMER_H_
#define _HWTIMER_H_

////////////////////////////////////////////////////////////////////////////////

// system timer
extern void hwtimer_start(unsigned short ms, void (*func)(void));
extern void hwtimer_restart(void);

////////////////////////////////////////////////////////////////////////////////

#endif

