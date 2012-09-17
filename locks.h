
#ifndef _LOCKS_H_
#define _LOCKS_H_

////////////////////////////////////////////////////////////////////////////////

unsigned short presto_lock(void);
void presto_unlock_all(void);
void presto_restore_lock(unsigned short mask);
void presto_interruption_point(void);

////////////////////////////////////////////////////////////////////////////////

#endif

