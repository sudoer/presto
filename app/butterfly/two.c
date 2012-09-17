
#include "presto.h"
#include "types.h"
#include "error.h"
#include "messages.h"
#include "lcd_driver.h"

////////////////////////////////////////////////////////////////////////////////

#define FLAG_2_LOOP        0x04
#define FLAG_2_MAIL        0x02
#define FLAG_2_TIMER       0x01

////////////////////////////////////////////////////////////////////////////////

void two(void) {

   static PRESTO_MAILBOX_T two_mbox;
   presto_mailbox_init(&two_mbox,FLAG_2_MAIL);

   presto_wait_for_idle();

   while (1) {

      PRESTO_ENVELOPE_T * recv_p;
      recv_p=presto_mail_wait(&two_mbox);
      switch(presto_envelope_message(recv_p)) {
         case MSG_ONEtoTWO_PING: {
            PRESTO_ENVELOPE_T * send_p;
            volatile unsigned char x=0;
            while(++x) { /* delay */ }
            send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
            presto_mail_send_to_task(one_tid,send_p,MSG_TWOtoONE_PONG,NULL);
         } break;
      }
      presto_memory_free((BYTE *)recv_p);
   }
}

////////////////////////////////////////////////////////////////////////////////


