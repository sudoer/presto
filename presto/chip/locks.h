
#ifndef _LOCKS_H_
#define _LOCKS_H_

////////////////////////////////////////////////////////////////////////////////

#define presto_lock(x)                asm("sei");
#define presto_unlock(x)              asm("cli");
#define presto_interruption_point(x)  asm("cli\nnop\nsei");
#define presto_lock_save(mask)        asm volatile ("tpa\n\tsei" : "=d"(mask));
#define presto_unlock_restore(mask)   asm volatile ("tap" : : "d"(mask));

////////////////////////////////////////////////////////////////////////////////

typedef unsigned short KERNEL_LOCK_T;

////////////////////////////////////////////////////////////////////////////////

#endif

