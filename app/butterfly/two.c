
#include "presto.h"
#include "types.h"
#include "error.h"
#include "messages.h"
#include "lcd_driver.h"

////////////////////////////////////////////////////////////////////////////////

#define FLAG_2_MAIL 0x01
#define FLAG_2_TIME 0x02

static PRESTO_MAILBOX_T two_mbox;
static PRESTO_TIMER_T timer2;

////////////////////////////////////////////////////////////////////////////////

void two(void) {

   BYTE x=0x80;
   BYTE digit;
   BYTE count=0;
   BOOLEAN gotit=FALSE;

   presto_mailbox_init(&two_mbox,FLAG_2_MAIL);
   presto_wait_for_idle();

   PRESTO_ENVELOPE_T * send_p;

   presto_timer_start(&timer2,0,500,FLAG_2_TIME);

   while (1) {
      PRESTO_TRIGGER_T t;
      t=presto_wait(FLAG_2_MAIL|FLAG_2_TIME);

      if (t&FLAG_2_TIME) {
         if (gotit) {
            x++;
            digit=(x&0xF0)>>4;
            lcd_text_digit(3,(digit<10)?(digit+'0'):(digit-10+'A'));
            digit=(x&0x0F);
            lcd_text_digit(4,(digit<10)?(digit+'0'):(digit-10+'A'));
            if(++count==8) {
               send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
               presto_mail_send_to_task(one_tid,send_p,MSG_TWOtoONE_PONG,NULL);
               gotit=FALSE;
            }
         } else {
            count=0;
         }
      }

      if (t&FLAG_2_MAIL) {
         PRESTO_ENVELOPE_T * recv_p;
         recv_p=presto_mail_get(&two_mbox);
         switch(presto_envelope_message(recv_p)) {
            case MSG_ONEtoTWO_PING:
               gotit=TRUE;
               break;
         }
         presto_memory_free((BYTE *)recv_p);
      }

   }
}

////////////////////////////////////////////////////////////////////////////////


