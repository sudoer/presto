
#ifndef SHARED_H
#define SHARED_H

////////////////////////////////////////////////////////////////////////////////

extern PRESTO_SEMRESOURCE_T copier;
extern BYTE lights;

extern void assert_lights(void);
extern void busy_work(BYTE blink_mask,WORD worktodo);

////////////////////////////////////////////////////////////////////////////////

#endif // SHARED_H
