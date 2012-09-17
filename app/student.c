
#include "presto.h"
#include "types.h"
#include "error.h"
#include "messages.h"
#include "shared.h"
#include "debug.h"

////////////////////////////////////////////////////////////////////////////////

#define FLAG_S_LOOP        0x04
#define FLAG_S_MAIL        0x02
#define FLAG_S_TIMER       0x10

////////////////////////////////////////////////////////////////////////////////

void student(void) {
   static PRESTO_MAILBOX_T stud_mbox;
   PRESTO_TIMER_T ticker1;

   presto_mailbox_init(&stud_mbox,FLAG_S_MAIL);

   presto_wait_for_idle();

   presto_timer_start(&ticker1,1,125,FLAG_S_TIMER);

   int count=0;
   while (1) {
      presto_wait(FLAG_S_TIMER);

      // throw away any mail in the student mailbox
      PRESTO_ENVELOPE_T * recv_p;
      while ((recv_p=presto_mail_get(&stud_mbox))!=NULL) {
         presto_memory_free((BYTE *)recv_p);
      }

      MASKNOT(lights,0x08);
      assert_lights();

      //MASKNOT(PORTD,0x3C);

      if(++count==25) {
         count=0;
         PRESTO_ENVELOPE_T * send_p;
         send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
         presto_mail_send_to_task(empl_tid,send_p,MSG_STUDtoEMPL_STATUS,NULL);
      }

   }
}

////////////////////////////////////////////////////////////////////////////////


