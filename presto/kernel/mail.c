////////////////////////////////////////////////////////////////////////////////
//   C O M M E N T A R Y
////////////////////////////////////////////////////////////////////////////////

// This module implements a simple and fast mechanism for passing messages
// between tasks.  The metaphor used is that of mailboxes and envelopes.

// A task can own one or more mailboxes.  Each mailbox has associated with
// it, a "trigger" (or "ready bit").  When mail arrives in the mailbox, the
// trigger is set.  If the task is waiting on that trigger, then the task
// will become ready.

// There is a little bit of overhead associated with keeping track of mail
// messages.  This is primarily the "next" pointers in a linked list of
// messages.  Rather than keep an arbitrary-sized list of mail messages
// for each mailbox (which is limiting and inefficient), we require the
// use of "envelopes".  Envelopes contain internal accounting data that
// is needed to keep mail messages in lists, but they also contain some
// useful information, like the sender of the message.

// The user can decide whether to use static envelopes (stored on the stack
// or globally) or dynamically allocated envelopes (from the heap).  It is
// a good practice to allocate envelopes just before they are sent, and
// then let the receiver free the envelope memory after he has read the
// message.  Coincidentally, this practice mirrors what we do in real life
// (sender buys, receiver throws away).

// A task may have one mailbox, or it may have many mailboxes, or it may
// have no mailboxes.  There are cases where it is useful to have more than
// one mailbox.  For example, a serial port driver may have one mailbox for
// traffic to pass along the line and a separate mailbox for flow control
// messages (you would not want a flow control message to get stuck behind
// lots of data -- you would want that message to be received immediately).

// In most cases, however, one mailbox per task is sufficient.  To make
// addressing easier, the first mailbox that a task initializes is it's
// "primary" mailbox.  If someone sends a message to a task, then it will be
// delivered to that task's primary mailbox.  The "primary" designation can
// later be assigned to a different mailbox.

// Messages meant to be delivered to a secondary mailbox must be delivered
// directly to that BOX, and not to the task.  There are two "send" functions
// which cover these two options.

// So what kind of messages can we send?  Most of the time, we are sending
// simple instructions from one task to another.  "I am alive", or "please
// scan the keyboard".  Other times, we need to send a lot of data.  Presto
// sends two things in each mail message: an integer and a pointer.  For
// simple messages, the integer will suffice (you can leave the pointer
// NULL).  For more complex scenarios, the pointer can give the location of
// a structure that has the needed information.  This pointer can point to
// static (global) memory or to dynamic memory -- it is up to the user to
// define a protocol for ownership of memory, and who frees what.  Again,
// a good practice is to let the sender allocate and the receiver free.


////////////////////////////////////////////////////////////////////////////////
//   D E P E N D E N C I E S
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "presto.h"
#include "error.h"
#include "cpu_locks.h"
#include "configure.h"
#include "kernel/kernel.h"
#include "kernel/mail.h"

#ifdef FEATURE_KERNEL_MAIL

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
   tid=kernel_current_task();
   if (default_mailbox[tid]==NULL) {
      default_mailbox[tid]=box_p;
   }
   // initialize data members
   box_p->message_count=0;
   box_p->mailbox_head=NULL;
   box_p->mailbox_tail=NULL;
   box_p->owner_tid=tid;
   box_p->trigger=trigger;
}


////////////////////////////////////////////////////////////////////////////////


void presto_mailbox_default(KERNEL_MAILBOX_T * box_p) {
   default_mailbox[kernel_current_task()]=box_p;
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

   CPU_LOCK_T lock;

   // check to see that the recipient is a live mailbox
   if (box_p==NULL) {
      error_fatal(ERROR_MAIL_SENDTONULLBOX);
   }

   // check to see that the envelope is OK
   if (env_p==NULL) {
      error_fatal(ERROR_MAIL_SENDNULLENVELOPE);
   }

   // fill in the blanks in the envelope
   env_p->userdata.message=message;
   env_p->userdata.payload=payload;
   env_p->from_tid=kernel_current_task();
   env_p->to_box_p=box_p;

   // messing with the mail lists, no interrupts
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

   // done with mail lists, interrupts OK
   cpu_unlock_restore(lock);

   // make mailbox owner ready
   kernel_trigger_set_noswitch(box_p->owner_tid, box_p->trigger);

   // receiver becomes ready...
   // time to re-evaluate highest ready task
   kernel_context_switch();
}


////////////////////////////////////////////////////////////////////////////////


KERNEL_ENVELOPE_T * presto_mail_wait(KERNEL_MAILBOX_T * box_p) {
   // First, wait for mail to arrive.
   presto_wait(box_p->trigger);

   // We have mail(*), so return it.

   // * If we allow mail stealing (letting a task read from mailboxes that
   // he does now own), the owner could be alerted to an incoming mail, but
   // by the time he checks the box, it could be empty.  In this case, we
   // return a NULL here.
   return presto_mail_get(box_p);
}


////////////////////////////////////////////////////////////////////////////////


KERNEL_ENVELOPE_T * presto_mail_get(KERNEL_MAILBOX_T * box_p) {
   KERNEL_ENVELOPE_T * env_p;
   CPU_LOCK_T lock;

   // we're going to use this a lot, so dereference now
   env_p=box_p->mailbox_head;

   // We will return immediately if there are no messages in our queue
   if (env_p==NULL) return NULL;

   #ifdef FEATURE_MAIL_NOSTEALING
      // Someone can easily read mail from someone else's box.
      // Check ownership, and give anyone else nothing.
      if ((env_p->to_box_p->owner_tid)!=kernel_current_task()) return NULL;
   #endif // FEATURE_MAIL_NOSTEALING

   // we're about to mess with the mail list... interrupts off
   cpu_lock_save(lock);

   // there is at least one message, get one
   if (env_p==box_p->mailbox_tail) {
      // there is only one item in the list, take it
      box_p->mailbox_head=NULL;
      box_p->mailbox_tail=NULL;
      box_p->message_count=0;
      // Should we clear the owner's flag here?  I don't think so.
   } else {
      // there are many messages, take first
      box_p->mailbox_head=env_p->next;
      box_p->message_count--;
      // make mailbox owner ready (again?)
      kernel_trigger_set_noswitch(box_p->owner_tid, box_p->trigger);
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

#endif // FEATURE_KERNEL_MAIL

