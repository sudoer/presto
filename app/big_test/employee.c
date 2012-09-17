
#include "presto.h"
#include "types.h"
#include "error.h"
#include "messages.h"
#include "shared.h"

////////////////////////////////////////////////////////////////////////////////

#define FLAG_E_LOOP        0x04
#define FLAG_E_MAIL        0x02
#define FLAG_E_TIMER       0x01

////////////////////////////////////////////////////////////////////////////////

void employee(void) {

   static PRESTO_MAILBOX_T empl_mbox;
   presto_mailbox_init(&empl_mbox,FLAG_E_MAIL);

   presto_wait_for_idle();

   while (1) {
      PRESTO_ENVELOPE_T * recv_p;
      recv_p=presto_mail_wait(&empl_mbox);
      switch(presto_envelope_message(recv_p)) {
         case MSG_CTRLtoEMPL_LOOP:
            presto_timer_wait(1000);
            use_copier(0x04,50);
            break;
         case MSG_STUDtoEMPL_STATUS: {
            PRESTO_ENVELOPE_T * send_p;
            send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
            presto_mail_send_to_task(mngr_tid,send_p,MSG_EMPLtoMNGR_STATUS,NULL);
            } break;
      }
      presto_memory_free((BYTE *)recv_p);
   }
}

////////////////////////////////////////////////////////////////////////////////


