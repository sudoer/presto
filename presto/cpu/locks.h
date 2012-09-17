
#ifndef _LOCKS_H_
#define _LOCKS_H_

////////////////////////////////////////////////////////////////////////////////

#define cpu_lock(x)                asm("sei");
#define cpu_unlock(x)              asm("cli");
#define cpu_interruption_point(x)  asm("cli\nnop\nsei");
#define cpu_lock_save(mask)        asm volatile ("tpa\n\tsei" : "=d"(mask));
#define cpu_unlock_restore(mask)   asm volatile ("tap" : : "d"(mask));

////////////////////////////////////////////////////////////////////////////////

typedef unsigned short CPU_LOCK_T;

////////////////////////////////////////////////////////////////////////////////

#endif

