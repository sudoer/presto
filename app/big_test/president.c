
#include "presto.h"
#include "types.h"
#include "error.h"
#include "messages.h"
#include "shared.h"

////////////////////////////////////////////////////////////////////////////////

#define FLAG_P_LOOP        0x04
#define FLAG_P_MAIL        0x02
#define FLAG_P_TIMER       0x01

////////////////////////////////////////////////////////////////////////////////

void president(void) {

   PRESTO_TIMER_T * timer_p=NULL;

   static PRESTO_MAILBOX_T pres_mbox;
   presto_mailbox_init(&pres_mbox,FLAG_P_MAIL);

   static PRESTO_SEMLOCK_T copier_pres;
   presto_semaphore_register(&copier,&copier_pres);

   presto_wait_for_idle();

   while (1) {
      PRESTO_TRIGGER_T t;
      t=presto_wait(FLAG_P_MAIL|FLAG_P_TIMER);

      if (t&FLAG_P_MAIL) {
         PRESTO_ENVELOPE_T * recv_p;
         recv_p=presto_mail_get(&pres_mbox);
         switch(presto_envelope_message(recv_p)) {
            case MSG_CTRLtoPRES_LOOP: 
               timer_p=(PRESTO_TIMER_T *)presto_memory_allocate(sizeof(PRESTO_TIMER_T));
               presto_timer_start(timer_p,3000,0,FLAG_P_TIMER);
               break;
            case MSG_MNGRtoPRES_STATUS:
               busy_work(0x0F,5);
               break;
         }
         presto_memory_free((BYTE *)recv_p);
      }

      if (t&FLAG_P_TIMER) {
         presto_memory_free((BYTE *)timer_p);
         presto_semaphore_wait(&copier_pres);
         busy_work(0x01,10);
         presto_semaphore_release(&copier_pres);
      }
   }
}

////////////////////////////////////////////////////////////////////////////////


