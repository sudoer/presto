
#include "presto.h"
#include "types.h"
#include "error.h"
#include "messages.h"
#include "lcd_driver.h"

////////////////////////////////////////////////////////////////////////////////

#define FLAG_1_MAIL 0x01
#define FLAG_1_TIME 0x02

static PRESTO_MAILBOX_T one_mbox;
static PRESTO_TIMER_T timer1;

////////////////////////////////////////////////////////////////////////////////

void one(void) {

   BYTE x=0x80;
   BYTE digit;
   BYTE count=0;
   BOOLEAN gotit=TRUE;

   presto_mailbox_init(&one_mbox,FLAG_1_MAIL);
   presto_wait_for_idle();

   PRESTO_ENVELOPE_T * send_p;

   presto_timer_start(&timer1,0,1000,FLAG_1_TIME);

   while (1) {
      PRESTO_TRIGGER_T t;
      t=presto_wait(FLAG_1_MAIL|FLAG_1_TIME);

      if (t&FLAG_1_TIME) {
         if (gotit) {
            x++;
            digit=(x&0xF0)>>4;
            lcd_text_digit(0,(digit<10)?(digit+'0'):(digit-10+'A'));
            digit=(x&0x0F);
            lcd_text_digit(1,(digit<10)?(digit+'0'):(digit-10+'A'));
            if(++count==8) {
               send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
               presto_mail_send_to_task(two_tid,send_p,MSG_ONEtoTWO_PING,NULL);
               gotit=FALSE;
            }
         } else {
            count=0;
         }
      }

      if (t&FLAG_1_MAIL) {
         PRESTO_ENVELOPE_T * recv_p;
         recv_p=presto_mail_get(&one_mbox);
         switch(presto_envelope_message(recv_p)) {
            case MSG_TWOtoONE_PONG:
               gotit=TRUE;
               break;
         }
         presto_memory_free((BYTE *)recv_p);
      }

   }
}

////////////////////////////////////////////////////////////////////////////////


