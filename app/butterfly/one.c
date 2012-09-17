
#include "presto.h"
#include "types.h"
#include "error.h"
#include "messages.h"
#include "lcd_driver.h"

////////////////////////////////////////////////////////////////////////////////

#define FLAG_1_MAIL  0x01
#define FLAG_1_TIME0 0x02
#define FLAG_1_TIME1 0x04
#define FLAG_1_TIME2 0x08
#define FLAG_1_TIME3 0x10
#define FLAG_1_TIME4 0x20
#define FLAG_1_TIME5 0x40
#define FLAG_1_TIME  0x7E

static PRESTO_MAILBOX_T one_mbox;
static PRESTO_TIMER_T timer0;
static PRESTO_TIMER_T timer1;
static PRESTO_TIMER_T timer2;
static PRESTO_TIMER_T timer3;
static PRESTO_TIMER_T timer4;
static PRESTO_TIMER_T timer5;

////////////////////////////////////////////////////////////////////////////////

void one(void) {

   char a0='A';
   char a1='A';
   char a2='A';
   char a3='A';
   char a4='A';
   char a5='A';

   presto_mailbox_init(&one_mbox,FLAG_1_MAIL);
   presto_wait_for_idle();

   PRESTO_ENVELOPE_T * send_p;

   presto_timer_start(&timer0,0,1000,FLAG_1_TIME0);
   presto_timer_start(&timer1,0, 425,FLAG_1_TIME1);
   presto_timer_start(&timer2,0,1250,FLAG_1_TIME2);
   presto_timer_start(&timer3,0,  50,FLAG_1_TIME3);
   presto_timer_start(&timer4,0, 800,FLAG_1_TIME4);
   presto_timer_start(&timer5,0, 660,FLAG_1_TIME5);

   while (1) {
      PRESTO_TRIGGER_T t;
      t=presto_wait(FLAG_1_MAIL|FLAG_1_TIME);

      if (t&FLAG_1_TIME0) {
         if(a0++ =='Z') a0='A';
         lcd_text_digit(0,a0);
      }
      if (t&FLAG_1_TIME1) {
         if(a1++ =='Z') a1='A';
         lcd_text_digit(1,a1);
      }
      if (t&FLAG_1_TIME2) {
         if(a2++ =='Z') a2='A';
         lcd_text_digit(2,a2);
      }
      if (t&FLAG_1_TIME3) {
         if(a3++ =='Z') a3='A';
         lcd_text_digit(3,a3);
      }
      if (t&FLAG_1_TIME4) {
         if(a4++ =='Z') a4='A';
         lcd_text_digit(4,a4);
      }
      if (t&FLAG_1_TIME5) {
         if(a5++ =='Z') a5='A';
         lcd_text_digit(5,a5);
      }

/*
      send_p=(PRESTO_ENVELOPE_T *)presto_memory_allocate(sizeof(PRESTO_ENVELOPE_T));
      presto_mail_send_to_task(two_tid,send_p,MSG_ONEtoTWO_PING,NULL);
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
*/
      if (t&FLAG_1_MAIL) {
         PRESTO_ENVELOPE_T * recv_p;
         recv_p=presto_mail_get(&one_mbox);
         presto_memory_free((BYTE *)recv_p);
      }


   }
}

////////////////////////////////////////////////////////////////////////////////


