
#include "presto.h"
#include "types.h"
#include "error.h"
#include "messages.h"
#include "lcd_driver.h"

////////////////////////////////////////////////////////////////////////////////

#define FLAG_1_MAIL 0x01
#define FLAG_1_TIME 0x02

static PRESTO_MAILBOX_T one_mbox;
//PRESTO_TIMER_T timer;

////////////////////////////////////////////////////////////////////////////////

void one(void) {

   BYTE x=0x80;
   //BYTE y=0x00;
   BYTE digit;
   //PRESTO_TIMER_T timer_p;

   presto_mailbox_init(&one_mbox,FLAG_1_MAIL);
   presto_wait_for_idle();

   PRESTO_ENVELOPE_T * send_p;
   //PRESTO_TIMER_T * timer_p;

   lcd_text_digit(0,'-');
   lcd_text_digit(1,'-');
   lcd_text_digit(2,'A');
   lcd_text_digit(3,'L');
   lcd_text_digit(4,'A');
   lcd_text_digit(5,'N');

   //timer_p=(PRESTO_TIMER_T *)presto_memory_allocate(sizeof(PRESTO_TIMER_T));
   //presto_timer_start(&timer,2000,0,FLAG_1_TIME);
   //presto_trigger_send(one_tid,FLAG_1_TIME);

   send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
   presto_mail_send_to_task(two_tid,send_p,MSG_ONEtoTWO_PING,NULL);

   while (1) {
      PRESTO_TRIGGER_T t;
      t=presto_wait(FLAG_1_MAIL|FLAG_1_TIME);

      if (t&FLAG_1_TIME) {
         //presto_memory_free((BYTE *)timer_p);
         x++;
         digit=(x&0xF0)>>4;
         lcd_text_digit(0,(digit<10)?(digit+'0'):(digit-10+'A'));
         digit=(x&0x0F);
         lcd_text_digit(1,(digit<10)?(digit+'0'):(digit-10+'A'));
         //presto_timer_start(&timer,1000,0,FLAG_1_TIME);
      }

      if (t&FLAG_1_MAIL) {
         PRESTO_ENVELOPE_T * recv_p;
         recv_p=presto_mail_get(&one_mbox);
         switch(presto_envelope_message(recv_p)) {
            case MSG_TWOtoONE_PONG:
               send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
               presto_mail_send_to_task(two_tid,send_p,MSG_ONEtoTWO_PING,NULL);
               break;
         }
         presto_memory_free((BYTE *)recv_p);
         presto_trigger_send(one_tid,FLAG_1_TIME);
      }

   }
}

////////////////////////////////////////////////////////////////////////////////


