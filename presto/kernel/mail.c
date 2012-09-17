////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "presto.h"
#include "error.h"
#include "chip/locks.h"
#include "kernel/kernel.h"
#include "kernel/mail.h"


////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////

// debug
#define STATIC //static


////////////////////////////////////////////////////////////////////////////////
//   D A T A   T Y P E S
////////////////////////////////////////////////////////////////////////////////

typedef struct KERNEL_MESSAGE_S {
   KERNEL_MSGID_T serial_number;
   KERNEL_TID_T from_tid;
   union KERNEL_MAIL_U payload;
   struct KERNEL_MAILBOX_S * to_box_p;
   struct KERNEL_MESSAGE_S * next;
} PRESTO_MESSAGE_T;


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N   P R O T O T Y P E S
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//   K E R N E L - O N L Y   D A T A
///////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   G L O B A L   D A T A
////////////////////////////////////////////////////////////////////////////////

STATIC PRESTO_MESSAGE_T * free_mail_p=NULL;
STATIC PRESTO_MESSAGE_T mail_list[MAX_MESSAGES];


////////////////////////////////////////////////////////////////////////////////
//   K E R N E L - O N L Y   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void kernel_mail_init(void) {
   int count;
   // initialize mail list
   for(count=0;count<MAX_MESSAGES;count++) {
      mail_list[count].next=&mail_list[count+1];  // goes past end of array - OK
      mail_list[count].serial_number=count;
   }
   mail_list[MAX_MESSAGES-1].next=NULL;
   free_mail_p=&mail_list[0];
}

////////////////////////////////////////////////////////////////////////////////
//   E X T E R N A L   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

void presto_mail_init(PRESTO_MAILBOX_T * box_p, KERNEL_FLAG_T flag) {
   box_p->message_count=0;
   box_p->mailbox_head=NULL;
   box_p->mailbox_tail=NULL;
   box_p->owner_tcb_p=current_tcb_p;
   box_p->trigger_flag=flag;
}

////////////////////////////////////////////////////////////////////////////////

KERNEL_MSGID_T presto_mail_send(PRESTO_MAILBOX_T * box_p, PRESTO_MAIL_T payload) {

   PRESTO_MESSAGE_T * new_mail_p;
   KERNEL_TCB_T * owner_tcb_p;
   WORD lock;

   // check to see if there's room
   if(free_mail_p==NULL) {
      presto_fatal_error(ERROR_KERNEL_MAILSEND_NOFREE);
   }
   // check to see that the recipient is a live mailbox
   if(box_p==NULL) {
      presto_fatal_error(ERROR_KERNEL_MAILSEND_TONULLBOX);
   }
   owner_tcb_p=box_p->owner_tcb_p;
   if((owner_tcb_p==NULL)||(owner_tcb_p->in_use!=TRUE)) {
      presto_fatal_error(ERROR_KERNEL_MAILSEND_TONOBODY);
   }

   // no interrupts
   presto_lock_save(lock);

   // allocate space for a new message
   new_mail_p=free_mail_p;
   free_mail_p=free_mail_p->next;

   // fill in the blanks
   // we never cover up new_mail_p->serial_number
   new_mail_p->from_tid=current_tcb_p->task_id;
   new_mail_p->to_box_p=box_p;
   //new_mail_p->delivery_time=kernel_clock;
   //clock_add_ms(&new_mail_p->delivery_time,delay);
   //new_mail_p->period=period;
   new_mail_p->payload=payload;

   // interrupts OK
   presto_unlock_restore(lock);

   // no interrupts
   presto_lock_save(lock);

   // move the message to the tail of the task's mail list
   if(box_p->mailbox_head==NULL) {
      // we are the only message in the list
      box_p->mailbox_head=new_mail_p;
      box_p->mailbox_tail=new_mail_p;
      box_p->message_count=1;
   } else {
      // we are one of many, add to the tail of the list
      box_p->mailbox_tail->next=new_mail_p;
      box_p->mailbox_tail=new_mail_p;
      box_p->message_count++;
   }

   // no matter what, we are the last in the task's message list
   new_mail_p->next=NULL;

   // make mailbox owner ready
   kernel_flag_set(box_p->owner_tcb_p, box_p->trigger_flag);

   // interrupts OK
   presto_unlock_restore(lock);

   // receiver becomes ready...
   // time to re-evaluate highest ready task
   asm("swi");

   return new_mail_p->serial_number;
}

////////////////////////////////////////////////////////////////////////////////

BOOLEAN presto_mail_waiting(PRESTO_MAILBOX_T * box_p) {
   return (box_p->mailbox_head==NULL)?FALSE:TRUE;
}

////////////////////////////////////////////////////////////////////////////////

void presto_mail_wait(PRESTO_MAILBOX_T * box_p, PRESTO_MAIL_T * payload_p) {
   // First, wait for mail to arrive.
   presto_wait(box_p->trigger_flag);

   // sanity check
   if(box_p->mailbox_head==NULL) {
      presto_fatal_error(ERROR_KERNEL_MAILWAIT_NOMAIL);
   }

   // We have mail, so return it.
   presto_mail_get(box_p, payload_p);
}

////////////////////////////////////////////////////////////////////////////////

BOOLEAN presto_mail_get(PRESTO_MAILBOX_T * box_p, PRESTO_MAIL_T * payload_p) {
   PRESTO_MESSAGE_T * msg_p;
   WORD lock;

   // We will return immediately if there are no messages in our queue
   if(box_p->mailbox_head==NULL) return FALSE;

   // we're about to mess with the mail list... interrupts off
   presto_lock_save(lock);

   // we're going to use this a lot, so dereference now
   msg_p=box_p->mailbox_head;

   // get one message from the task's mail queue
   if(msg_p==NULL) {
      // there are no messages in the box's mail list
      presto_fatal_error(ERROR_KERNEL_MAILGET_NOMESSAGES);
   }

   #ifdef PARANOID
      // TODO - this is no longer paranoia... this is security
      // are we being paranoid?
      if((msg_p->to_box_p->owner_tcb_p)!=current_tcb_p) {
         presto_fatal_error(ERROR_KERNEL_MAILGET_NOTFORME);
      }
   #endif

   // there is at least one message, get one
   if (msg_p==box_p->mailbox_tail) {
      // there is only one item in the list, take it
      box_p->mailbox_head=NULL;
      box_p->mailbox_tail=NULL;
      box_p->message_count=0;
   } else {
      // there are many messages, take first
      box_p->mailbox_head=msg_p->next;
      box_p->message_count--;
   }

   // read the contents of the message before we can get interrupted
   if(payload_p!=NULL) *payload_p=msg_p->payload;

   // return the message to the free list
   msg_p->next=free_mail_p;
   free_mail_p=msg_p;

   // done messing with mail lists... interrupts back on
   presto_unlock_restore(lock);

   return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////

