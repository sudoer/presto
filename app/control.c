
#include "presto.h"
#include "types.h"
#include "error.h"
#include "messages.h"
#include "shared.h"

////////////////////////////////////////////////////////////////////////////////

void control(void) {

   BOOLEAN tf=FALSE;

   presto_mailbox_wait_for_task(empl_tid);
   presto_mailbox_wait_for_task(mngr_tid);
   presto_mailbox_wait_for_task(pres_tid);
   presto_mailbox_wait_for_task(stud_tid);

   PRESTO_ENVELOPE_T * send_p;
   while (1) {
      #ifdef FEATURE_SEMAPHORE_PRIORITYINHERITANCE
         presto_semaphore_init(&copier,1,tf);
      #else
         presto_semaphore_init(&copier,1);
      #endif // FEATURE_SEMAPHORE_PRIORITYINHERITANCE

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


