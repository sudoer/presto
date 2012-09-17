////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "presto.h"
#include "cpu/error.h"
#include "cpu/locks.h"
#include "configure.h"
#include "kernel/kernel.h"
#include "kernel/mail.h"


////////////////////////////////////////////////////////////////////////////////
//   C O N S T A N T S
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//   D A T A   T Y P E S
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N   P R O T O T Y P E S
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   G L O B A L   D A T A
////////////////////////////////////////////////////////////////////////////////


static KERNEL_MAILBOX_T * default_mailbox[PRESTO_KERNEL_MAXUSERTASKS];


////////////////////////////////////////////////////////////////////////////////
//   E X T E R N A L   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


void presto_mailbox_init(KERNEL_MAILBOX_T * box_p, KERNEL_TRIGGER_T trigger) {
   KERNEL_TASKID_T tid;
   // if this is the first mailbox for this task, make it the default
   tid=kernel_current_tcb_p->task_id;
   if (default_mailbox[tid]==NULL) {
      default_mailbox[tid]=box_p;
   }
   // initialize data members
   box_p->message_count=0;
   box_p->mailbox_head=NULL;
   box_p->mailbox_tail=NULL;
   box_p->owner_tcb_p=kernel_current_tcb_p;
   box_p->trigger=trigger;
}


////////////////////////////////////////////////////////////////////////////////


void presto_mailbox_default(KERNEL_MAILBOX_T * box_p) {
   default_mailbox[kernel_current_tcb_p->task_id]=box_p;
}


////////////////////////////////////////////////////////////////////////////////


KERNEL_MAILMSG_T presto_envelope_message(KERNEL_ENVELOPE_T * env_p) {
   return env_p->userdata.message;
}


////////////////////////////////////////////////////////////////////////////////


KERNEL_MAILPTR_T presto_envelope_payload(KERNEL_ENVELOPE_T * env_p) {
   return env_p->userdata.payload;
}


////////////////////////////////////////////////////////////////////////////////


KERNEL_TASKID_T presto_envelope_sender(KERNEL_ENVELOPE_T * env_p) {
   return env_p->from_tid;
}


////////////////////////////////////////////////////////////////////////////////


BOOLEAN presto_mail_send_to_task(KERNEL_TASKID_T tid, KERNEL_ENVELOPE_T * env_p,
                                 KERNEL_MAILMSG_T message, KERNEL_MAILPTR_T payload) {
   KERNEL_MAILBOX_T * box_p;
   box_p=default_mailbox[tid];
   if (box_p==NULL) return FALSE;
   presto_mail_send_to_box(box_p, env_p, message, payload);
   return TRUE;
}

////////////////////////////////////////////////////////////////////////////////


void presto_mail_send_to_box(KERNEL_MAILBOX_T * box_p, KERNEL_ENVELOPE_T * env_p,
                      KERNEL_MAILMSG_T message, KERNEL_MAILPTR_T payload) {

   KERNEL_TCB_T * owner_tcb_p;
   CPU_LOCK_T lock;

   // check to see that the recipient is a live mailbox
   if (box_p==NULL) {
      error_fatal(ERROR_KERNEL_MAILSEND_TONULLBOX);
   }
   owner_tcb_p=box_p->owner_tcb_p;

   // no interrupts
   cpu_lock_save(lock);

   // fill in the blanks
   env_p->userdata.message=message;
   env_p->userdata.payload=payload;
   env_p->from_tid=kernel_current_tcb_p->task_id;
   env_p->to_box_p=box_p;

   // interrupts OK
   cpu_unlock_restore(lock);

   // no interrupts
   cpu_lock_save(lock);

   // move the message to the tail of the task's mail list
   if (box_p->mailbox_head==NULL) {
      // we are the only message in the list
      box_p->mailbox_head=env_p;
      box_p->mailbox_tail=env_p;
      box_p->message_count=1;
   } else {
      // we are one of many, add to the tail of the list
      box_p->mailbox_tail->next=env_p;
      box_p->mailbox_tail=env_p;
      box_p->message_count++;
   }

   // no matter what, we are the last in the task's message list
   env_p->next=NULL;

   // make mailbox owner ready
   kernel_trigger_set(box_p->owner_tcb_p, box_p->trigger);

   // interrupts OK
   cpu_unlock_restore(lock);

   // receiver becomes ready...
   // time to re-evaluate highest ready task
   asm("swi");
}


////////////////////////////////////////////////////////////////////////////////


KERNEL_ENVELOPE_T * presto_mail_wait(KERNEL_MAILBOX_T * box_p) {
   // First, wait for mail to arrive.
   presto_wait(box_p->trigger);

   // sanity check
   if (box_p->mailbox_head==NULL) {
      error_fatal(ERROR_KERNEL_MAILWAIT_NOMAIL);
   }

   // We have mail, so return it.
   return presto_mail_get(box_p);
}


////////////////////////////////////////////////////////////////////////////////


KERNEL_ENVELOPE_T * presto_mail_get(KERNEL_MAILBOX_T * box_p) {
   KERNEL_ENVELOPE_T * env_p;
   CPU_LOCK_T lock;

   // We will return immediately if there are no messages in our queue
   if (box_p->mailbox_head==NULL) return NULL;

   // we're about to mess with the mail list... interrupts off
   cpu_lock_save(lock);

   // we're going to use this a lot, so dereference now
   env_p=box_p->mailbox_head;

   // get one message from the task's mail queue
   if (env_p==NULL) {
      // there are no messages in the box's mail list
      error_fatal(ERROR_KERNEL_MAILGET_NOMESSAGES);
   }

   #ifdef SANITYCHECK_MISDELIVEREDMAIL
      // TODO - this is no longer paranoia... this is security
      // are we being paranoid?
      if ((env_p->to_box_p->owner_tcb_p)!=kernel_current_tcb_p) {
         error_fatal(ERROR_KERNEL_MAILGET_NOTFORME);
      }
   #endif

   // there is at least one message, get one
   if (env_p==box_p->mailbox_tail) {
      // there is only one item in the list, take it
      box_p->mailbox_head=NULL;
      box_p->mailbox_tail=NULL;
      box_p->message_count=0;
   } else {
      // there are many messages, take first
      box_p->mailbox_head=env_p->next;
      box_p->message_count--;
      // make mailbox owner ready (again?)
      kernel_trigger_set(box_p->owner_tcb_p, box_p->trigger);
   }

   // do not point to the other envelopes any more
   env_p->next=NULL;

   // done messing with mail lists... interrupts back on
   cpu_unlock_restore(lock);

   return env_p;
}


////////////////////////////////////////////////////////////////////////////////
//   K E R N E L - O N L Y   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////


void kernel_mail_init(void) {
   int t;
   for (t=0;t<PRESTO_KERNEL_MAXUSERTASKS;t++) {
      default_mailbox[t]=NULL;
   }
}


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N S
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////



