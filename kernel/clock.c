
#include "clock.h"
#include "types.h"

////////////////////////////////////////////////////////////////////////////////

PRESTO_TIME_T clock_reset(void) {
   PRESTO_TIME_T clock;
   clock.l=0;
   clock.h=0;
   return clock;
}

////////////////////////////////////////////////////////////////////////////////

PRESTO_TIME_T clock_add(PRESTO_TIME_T clock, unsigned short time) {
   clock.l=clock.l+time;
   if(clock.l<time) {
      // carry
      clock.h++;
   }
   return clock;
}

////////////////////////////////////////////////////////////////////////////////

signed char clock_compare(PRESTO_TIME_T A,PRESTO_TIME_T B) {
   if(A.h < B.h) return -1;
   if(A.h > B.h) return 1;
   // we now know that A.h == B.h
   if(A.l < B.l) return -1;
   if(A.l > B.l) return 1;
   return 0;
}

////////////////////////////////////////////////////////////////////////////////

