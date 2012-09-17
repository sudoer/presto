
#ifndef _LOCKS_H_
#define _LOCKS_H_

////////////////////////////////////////////////////////////////////////////////

extern unsigned char flag_mirror;
#define INTR_ON()        asm("cli");
#define INTR_OFF()       asm("sei");
#define INTR_SAVE(x)     asm("psha"); asm("tpa"); asm("staa flag_mirror"); asm("pula"); asm("sei"); (x)=flag_mirror;
#define INTR_RESTORE(x)  if(!((x)&0x10)) asm("cli");

////////////////////////////////////////////////////////////////////////////////

unsigned short presto_lock(void);
void presto_unlock_all(void);
void presto_restore_lock(unsigned short mask);
void presto_interruption_point(void);

////////////////////////////////////////////////////////////////////////////////

#endif

