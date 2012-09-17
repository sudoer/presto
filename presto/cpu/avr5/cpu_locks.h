
#ifndef _LOCKS_H_
#define _LOCKS_H_

////////////////////////////////////////////////////////////////////////////////

#include "registers.h"

////////////////////////////////////////////////////////////////////////////////

#define cpu_lock(x)                asm volatile ("cli");
#define cpu_unlock(x)              asm volatile ("sei");
#define cpu_interruption_point(x)  //asm("CBI SREG,B_SRE\nNOP\nSBI SREG,B_SRE");
#define cpu_lock_save(mask)        { mask=(unsigned char)((unsigned char)inb(SREG))&0x80; asm volatile ("cli"); }
#define cpu_unlock_restore(mask)   { if (mask) { asm volatile ("sei"); } }

////////////////////////////////////////////////////////////////////////////////

typedef unsigned char CPU_LOCK_T;

////////////////////////////////////////////////////////////////////////////////

#endif // _LOCKS_H_

