
#include "presto.h"
#include "types.h"
#include "error.h"
#include "messages.h"
#include "shared.h"

////////////////////////////////////////////////////////////////////////////////

#define FLAG_M_LOOP        0x04
#define FLAG_M_MAIL        0x02
#define FLAG_M_TIMER       0x01

////////////////////////////////////////////////////////////////////////////////

void manager(void) {

   static PRESTO_MAILBOX_T mngr_mbox;
   presto_mailbox_init(&mngr_mbox,FLAG_M_MAIL);

   while (1) {
      PRESTO_ENVELOPE_T * recv_p;
      recv_p=presto_mail_wait(&mngr_mbox);
      switch(presto_envelope_message(recv_p)) {
         case MSG_CTRLtoMNGR_LOOP:
            presto_timer_wait(4000);
            busy_work(0x02,50);
            break;
         case MSG_EMPLtoMNGR_STATUS: {
            PRESTO_ENVELOPE_T * send_p;
            send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
            presto_mail_send_to_task(pres_tid,send_p,MSG_MNGRtoPRES_STATUS,NULL);
            } break;
      }
      presto_memory_free((BYTE *)recv_p);
   }
}

////////////////////////////////////////////////////////////////////////////////


