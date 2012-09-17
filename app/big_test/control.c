
#include "presto.h"
#include "types.h"
#include "error.h"
#include "messages.h"
#include "shared.h"

////////////////////////////////////////////////////////////////////////////////

#define FLAG_C_MAIL 0x01

static PRESTO_MAILBOX_T ctrl_mbox;

////////////////////////////////////////////////////////////////////////////////

void control(void) {

   BOOLEAN tf=FALSE;
   presto_mailbox_init(&ctrl_mbox,FLAG_C_MAIL);
   presto_semaphore_init(&copier,1);   // must be higher priority than sem users
   presto_wait_for_idle();

   PRESTO_ENVELOPE_T * send_p;
   while (1) {
      presto_semaphore_protocol(&copier,tf?
         PRESTO_SEMAPHORE_PRIORITY_INHERITANCE:
         PRESTO_SEMAPHORE_PRIORITY_NORMAL);

      // president is notified by mail
      send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
      presto_mail_send_to_task(pres_tid,send_p,MSG_CTRLtoPRES_LOOP,NULL);

      // manager is notified by mail
      send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
      presto_mail_send_to_task(mngr_tid,send_p,MSG_CTRLtoMNGR_LOOP,NULL);

      // employee is notified by mail
      send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
      presto_mail_send_to_task(empl_tid,send_p,MSG_CTRLtoEMPL_LOOP,NULL);

      // student is notified by mail
      send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
      presto_mail_send_to_task(stud_tid,send_p,MSG_CTRLtoSTUD_LOOP,NULL);

      // wait until next time
      presto_timer_wait(15000);

      tf=tf?FALSE:TRUE;
   }
}

////////////////////////////////////////////////////////////////////////////////


