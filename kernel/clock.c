
#include "clock.h"
#include "types.h"

////////////////////////////////////////////////////////////////////////////////

PRESTO_TIME_T clock_reset(void) {
   PRESTO_TIME_T clock;
   clock.w.l=0;
   clock.w.h=0;
   return clock;
}

////////////////////////////////////////////////////////////////////////////////

PRESTO_TIME_T clock_add(PRESTO_TIME_T clock, unsigned short time) {
   clock.w.l=clock.w.l+time;
   if(clock.w.l<time) {
      // carry
      clock.w.h++;
   }
   return clock;
}

////////////////////////////////////////////////////////////////////////////////

signed char clock_compare(PRESTO_TIME_T A,PRESTO_TIME_T B) {
   if(A.l == B.l) return 0;
   if(A.w.h < B.w.h) return -1;
   if(A.w.h > B.w.h) return 1;
   // we now know that A.w.h == B.w.h
   if(A.w.l < B.w.l) return -1;
   if(A.w.l > B.w.l) return 1;
   return 0;
}

////////////////////////////////////////////////////////////////////////////////

