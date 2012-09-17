
#ifndef _CPU_TIMER_H_
#define _CPU_TIMER_H_

////////////////////////////////////////////////////////////////////////////////

// system timer
extern void hwtimer_start(unsigned short ms, void (*func)(void));
extern void hwtimer_restart(void);

////////////////////////////////////////////////////////////////////////////////

#define CPU_TIMER_START(ms,func)   hwtimer_start(ms,func);
#define CPU_TIMER_RESTART()        hwtimer_restart();

////////////////////////////////////////////////////////////////////////////////

#endif // _CPU_TIMER_H_
