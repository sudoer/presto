
#include "presto.h"
#include "types.h"
#include "error.h"
#include "board.h"
#include "messages.h"
#include "shared.h"

////////////////////////////////////////////////////////////////////////////////

PRESTO_SEMRESOURCE_T copier;
BYTE lights=0xFF;

////////////////////////////////////////////////////////////////////////////////

#define FLAG_ALL_COPYTICK  0x80
#define FLAG_ALL_SEM       0x40

////////////////////////////////////////////////////////////////////////////////

#define MOTOR_PORT *(unsigned char *)(0x7FFF)

void assert_lights(void) {
   MOTOR_PORT=lights;
}

////////////////////////////////////////////////////////////////////////////////

void busy_work(BYTE blink_mask,WORD worktodo) {
   PRESTO_TIMER_T ThisTimerIsOnTheStack;
   WORD work_done=0;
   BYTE count=0;
   presto_timer_start(&ThisTimerIsOnTheStack,0,25,FLAG_ALL_COPYTICK);
   while (work_done<worktodo) {
      if (presto_trigger_poll(FLAG_ALL_COPYTICK)) {
         presto_trigger_clear(FLAG_ALL_COPYTICK);
         count++;
         if(count==4) {
            count=0;
            work_done++;
         }
         MASKNOT(lights,blink_mask);
         assert_lights();
      }
   }
   presto_timer_stop(&ThisTimerIsOnTheStack);
   presto_trigger_clear(FLAG_ALL_COPYTICK);
   MASKCLR(lights,blink_mask);
   assert_lights();
}

////////////////////////////////////////////////////////////////////////////////

