
#include "hc11regs.h"
#include "system.h"
#include "presto.h"
#include "intvect.h"
#include "error.h"
#include "clock.h"
#include "locks.h"

////////////////////////////////////////////////////////////////////////////////

// debugging

#define STATIC    /* static */
#define CHECK_STACK_CLOBBERING
#define PARANOID

////////////////////////////////////////////////////////////////////////////////

#define MAX_USER_TASKS       12
#define MAX_TASKS            MAX_USER_TASKS+1
#define MAX_MESSAGES         40

////////////////////////////////////////////////////////////////////////////////

#define CYCLES_PER_MS       2000        // based on hardware
#define CYCLES_PER_CLOCK    2           // based on hardware
#define CLOCK_PRESCALE      16          // set in TMSK2 register
#define MS_PER_TICK         20          // how often do you want?
#define CLOCKS_PER_MS       CYCLES_PER_MS*CYCLES_PER_CLOCK/CLOCK_PRESCALE
#define CLOCKS_PER_TICK     CLOCKS_PER_MS*MS_PER_TICK
#define IDLE_PRIORITY       0
#define IDLE_STACK_SIZE     50

////////////////////////////////////////////////////////////////////////////////

#define DISABLE_CCR_INTERRUPT_BIT     asm("oraa #0x10");
#define ENABLE_CCR_INTERRUPT_BIT      asm("anda #~0x10");

////////////////////////////////////////////////////////////////////////////////

#define CHECK_STACK_CLOBBERING
#define PARANOID

////////////////////////////////////////////////////////////////////////////////

typedef struct PRESTO_TCB_S {
   PRESTO_TID_T task_id;
   BYTE * stack_ptr;
   BYTE * stack_top;
   BYTE * stack_bottom;
   BYTE priority;
   BOOLEAN in_use;
   //PRESTO_TASK_STATE_T state;
   PRESTO_FLAG_T wait_mask;
   PRESTO_FLAG_T flags;
   struct PRESTO_TCB_S * next;
} PRESTO_TCB_T;

////////////////////////////////////////////////////////////////////////////////

typedef struct PRESTO_MESSAGE_S {
   PRESTO_MSGID_T serial_number;
   PRESTO_TID_T from_tid;
   union PRESTO_MAIL_U payload;
   struct PRESTO_MAILBOX_S * to_box_p;
   struct PRESTO_MESSAGE_S * next;
} PRESTO_MESSAGE_T;

////////////////////////////////////////////////////////////////////////////////

/*
typedef enum {
   STATE_READY,
   STATE_BLOCKED,
   STATE_INACTIVE
} PRESTO_TASK_STATE_T;
*/

////////////////////////////////////////////////////////////////////////////////

// STATIC GLOBAL VARIABLES

// task control blocks
STATIC PRESTO_TCB_T * current_tcb_p=NULL;
STATIC PRESTO_TCB_T * tcb_head_p=NULL;
STATIC PRESTO_TCB_T * free_tcb_p=NULL;
STATIC PRESTO_TCB_T tcb_list[MAX_TASKS];

// clock
STATIC PRESTO_TIME_T presto_master_clock;

// idle task stuff
STATIC BYTE idle_stack[IDLE_STACK_SIZE];
STATIC PRESTO_TCB_T * idle_tcb_p;
STATIC BYTE idle_tid;

// mail stuff
STATIC PRESTO_MESSAGE_T * free_mail_p=NULL;
STATIC PRESTO_MESSAGE_T * po_mail_p=NULL;
STATIC PRESTO_MESSAGE_T mail_list[MAX_MESSAGES];

// timer stuff
STATIC PRESTO_TIMER_T * timer_list=NULL;

// miscellaneous
STATIC BYTE presto_initialized=0;

// These are used to pass arguments to inline assembly routines.
// Do not put these on the stack (BOOM).
STATIC PRESTO_TCB_T * old_tcb_p;
STATIC BYTE * global_new_sp=NULL;
STATIC BYTE * global_old_sp=NULL;
STATIC BYTE ** old_task_stack_pointer_p;


////////////////////////////////////////////////////////////////////////////////
//   S T A T I C   F U N C T I O N   P R O T O T Y P E S
////////////////////////////////////////////////////////////////////////////////

// system timer
STATIC void systimer_Start(void);
STATIC void systimer_Restart(void);
STATIC void systimer_ISR(void) __attribute__((interrupt));

// scheduling
STATIC PRESTO_TCB_T * scheduler_FindNextReadyTask(void);
STATIC void idle_task(void);

// context switching
STATIC void context_switch_isr(void) __attribute__((interrupt));

// message handling
STATIC PRESTO_MSGID_T msg_Create(PRESTO_TID_T to, unsigned short delay, unsigned short period, PRESTO_MAIL_T payload);
STATIC void msg_InsertIntoPostOffice(PRESTO_MESSAGE_T * new_mail_p);
STATIC void msg_InsertIntoMailbox(PRESTO_MESSAGE_T * msg_p, PRESTO_TCB_T * to_tcb_p);
STATIC BYTE msg_DeliverToMailboxes(void);

// utilities
STATIC PRESTO_TCB_T * tid_to_tcbptr(BYTE tid);
void set_flag(PRESTO_TCB_T * tcb_p, PRESTO_FLAG_T flag);
void clear_flag(PRESTO_FLAG_T flag);

////////////////////////////////////////////////////////////////////////////////
//   I N I T I A L I Z A T I O N
////////////////////////////////////////////////////////////////////////////////

void presto_init(void) {
   BYTE count;
   BYTE flags;

   if(presto_initialized) return;

   // initialize master clock
   clock_reset(&presto_master_clock);

   // initialize TCB list
   for(count=0;count<MAX_TASKS;count++) {
      tcb_list[count].next=&tcb_list[count+1];
      tcb_list[count].task_id=count;
      tcb_list[count].in_use=FALSE;
   }
   tcb_list[MAX_TASKS-1].next=NULL;
   free_tcb_p=&tcb_list[0];

   // initialize mail list
   for(count=0;count<MAX_MESSAGES;count++) {
      mail_list[count].next=&mail_list[count+1];  // goes past end of array - OK
      mail_list[count].serial_number=count;
   }
   mail_list[MAX_MESSAGES-1].next=NULL;
   free_mail_p=&mail_list[0];

   // must be done before creating idle task
   presto_initialized=1;

   // initialize idle task
   idle_tid=presto_create_task(idle_task,idle_stack,IDLE_STACK_SIZE,IDLE_PRIORITY);
   idle_tcb_p=tid_to_tcbptr(idle_tid);
}

////////////////////////////////////////////////////////////////////////////////
//   T A S K   M A N A G E M E N T
////////////////////////////////////////////////////////////////////////////////

PRESTO_TID_T presto_create_task( void (*func)(void), BYTE * stack, short stack_size, BYTE priority ) {

   PRESTO_TCB_T * new_tcb_p;
   BYTE * sp;
   MISCWORD xlate;    // to split a word into two bytes
   WORD lock;

   if(!presto_initialized) {
      presto_fatal_error(ERROR_CREATE_BEFORE_INIT);
   }

   if(free_tcb_p==NULL) {
      // There are no more TCB's left.
      presto_fatal_error(ERROR_NO_MORE_TCB);
      return -1;
   }

   // we're about to mess with tasks, TCB's... interrupts off
   presto_lock_save(lock);

   // allocate TCB for new task
   new_tcb_p=free_tcb_p;
   free_tcb_p=free_tcb_p->next;

   // initialize TCB elements
   // new_tcb_p->task_id is already assigned
   new_tcb_p->stack_top=stack+stack_size-1;
   new_tcb_p->stack_bottom=stack;
   new_tcb_p->priority=priority;
   new_tcb_p->wait_mask=0;
   new_tcb_p->flags=0;
   new_tcb_p->in_use=TRUE;

   /*
   new_tcb_p->mailbox_head=NULL;
   new_tcb_p->mailbox_tail=NULL;
   */

   // SET UP NEW STACK

   sp=new_tcb_p->stack_top;
   xlate.w=(WORD)func;
   *sp--=xlate.b.l;    // function pointer(L)
   *sp--=xlate.b.h;    // function pointer(H)
   *sp--=0x66;         // Y(L) register
   *sp--=0x55;         // Y(H) register
   *sp--=0x44;         // X(L) register
   *sp--=0x33;         // X(H) register
   *sp--=0x11;         // A register
   *sp--=0x22;         // B register
   *sp--=0x00;         // condition codes (I bit cleared)
   *sp--=0xBB;         // _.tmp 0000(L)
   *sp--=0xAA;         // _.tmp 0000(H)
   *sp--=0xDD;         // _.z   0002(L)
   *sp--=0xCC;         // _.z   0002(H)
   *sp--=0xFF;         // _.xy  0004(L)
   *sp--=0xEE;         // _.xy  0004(H)

   new_tcb_p->stack_ptr=sp;

   // INSERT NEW TCB INTO LIST IN PRIORITY ORDER

   if(tcb_head_p==NULL) {
      // we are the first TCB in the list
      tcb_head_p=new_tcb_p;
      new_tcb_p->next=NULL;
   } else if((new_tcb_p->priority)>(tcb_head_p->priority)) {
      // advance to the head of the class!
      new_tcb_p->next=tcb_head_p;
      tcb_head_p=new_tcb_p;
   } else {
      PRESTO_TCB_T * ptr=tcb_head_p;
      while(ptr->next!=NULL) {
         if((new_tcb_p->priority)>(ptr->next->priority)) break;
         ptr=ptr->next;
      }

      // ptr->next is either NULL or lower priority than us
      // either way, we want to get inserted between ptr and ptr->next
      new_tcb_p->next=ptr->next;
      ptr->next=new_tcb_p;
   }

   // we're done messing with the task list... interrupts back on
   presto_unlock_restore(lock);

   return new_tcb_p->task_id;
}

////////////////////////////////////////////////////////////////////////////////
//   S T A R T   M U L T I - T A S K I N G
////////////////////////////////////////////////////////////////////////////////

void presto_start_scheduler(void) {

   // we're about to switch to our first task... interrupts off
   presto_lock();

   //set_interrupt(INTR_TOC2, systimer_ISR_wrapper);
   set_interrupt(INTR_TOC2, systimer_ISR);
   set_interrupt(INTR_SWI, context_switch_isr);

   // start timer interrupts for pre-emption
   systimer_Start();

   // pick next task to run
   // first task in list is highest priority and is ready
   current_tcb_p=tcb_head_p;
   if(current_tcb_p==NULL) {
      presto_fatal_error(ERROR_TCB_HEAD_IS_NULL);
   }

   // SET UP A NEW STACK AND START EXECUTION USING IT

   // these parameters will be used in inline assembly...
   // must be put in global space, not on stack
   global_new_sp=current_tcb_p->stack_ptr;

   asm("lds global_new_sp");
   asm("pulx");  // _.xy
   asm("pulx");  // _.z
   asm("pulx");  // _.tmp
   asm("rti");

   // we never get here
   presto_fatal_error(ERROR_START_AFTERRTI);
}

////////////////////////////////////////////////////////////////////////////////
//   F L A G S   A N D   W A I T I N G
////////////////////////////////////////////////////////////////////////////////

PRESTO_FLAG_T presto_wait(PRESTO_FLAG_T wait_for) {

   PRESTO_FLAG_T matched_flags;

   // Save wait_for for later.
   current_tcb_p->wait_mask=wait_for;

   matched_flags=(wait_for & current_tcb_p->flags);

   // If we do not need to wait, then keep going.
   if(matched_flags==0) {
      // Current task must wait.
      // We should re-evaluate priorities and swap tasks.
      asm("swi");
   }

   // When we wake up, we'll be ready to run again.
   // Interrupts will be enabled.

   // Save wait_for for later.
   current_tcb_p->wait_mask=0;

   return matched_flags;
}

////////////////////////////////////////////////////////////////////////////////

void set_flag(PRESTO_TCB_T * tcb_p, PRESTO_FLAG_T flag) {
   tcb_p->flags |= flag;
}

////////////////////////////////////////////////////////////////////////////////

void clear_flag(PRESTO_FLAG_T flag) {
   current_tcb_p->flags &= ~flag;
}

////////////////////////////////////////////////////////////////////////////////
//   S C H E D U L I N G
////////////////////////////////////////////////////////////////////////////////

STATIC PRESTO_TCB_T * scheduler_FindNextReadyTask(void) {
   // pick highest priority ready task to run
   PRESTO_TCB_T * ptr=tcb_head_p;
   while(ptr!=NULL) {
      if(ptr->wait_mask==0) return ptr;
      if(ptr->wait_mask & ptr->flags) return ptr;
      ptr=ptr->next;
   }
   // should never get here
   presto_fatal_error(ERROR_NEXTTCB_NOTFOUND);
   return NULL;
}

////////////////////////////////////////////////////////////////////////////////
//   M A I L   M E S S A G E S
////////////////////////////////////////////////////////////////////////////////

void presto_mailbox_init(PRESTO_MAILBOX_T * box_p, PRESTO_FLAG_T flag) {
   box_p->message_count=0;
   box_p->mailbox_head=NULL;
   box_p->mailbox_tail=NULL;
   box_p->owner_tcb_p=current_tcb_p;
   box_p->trigger_flag=flag;
}

////////////////////////////////////////////////////////////////////////////////

PRESTO_MSGID_T presto_mail_send(PRESTO_MAILBOX_T * box_p, PRESTO_MAIL_T payload) {

   PRESTO_MESSAGE_T * new_mail_p;
   PRESTO_TCB_T * owner_tcb_p;
   WORD lock;

   // check to see if there's room
   if(free_mail_p==NULL) {
      presto_fatal_error(ERROR_MAIL_NOFREE);
   }
   // check to see that the recipient is a live mailbox
   if(box_p==NULL) {
      presto_fatal_error(ERROR_MAIL_TONULLBOX);
   }
   owner_tcb_p=box_p->owner_tcb_p;
   if((owner_tcb_p==NULL)||(owner_tcb_p->in_use!=TRUE)) {
      presto_fatal_error(ERROR_MAIL_TONOBODY);
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
   //new_mail_p->delivery_time=presto_master_clock;
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
   set_flag(box_p->owner_tcb_p, box_p->trigger_flag);

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
      presto_fatal_error(ERROR_MAILWAIT_NOMAIL);
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
      presto_fatal_error(ERROR_MAILGET_NOMESSAGES);
   }

   #ifdef PARANOID
      // TODO - this is no longer paranoia... this is security
      // are we being paranoid?
      if((msg_p->to_box_p->owner_tcb_p)!=current_tcb_p) {
         presto_fatal_error(ERROR_MAILGET_NOTFORME);
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
//   T I M E R S
////////////////////////////////////////////////////////////////////////////////

void presto_timer(PRESTO_TIMER_T * timer_p, PRESTO_INTERVAL_T delay, PRESTO_INTERVAL_T period, PRESTO_FLAG_T flag) {

   WORD lock;

   if(delay==0) {
      set_flag(timer_p->owner_tcb_p, flag);
      return;
   }

   // set members of the timer structure
   timer_p->delivery_time=presto_master_clock;
   clock_add_ms(&timer_p->delivery_time,delay);
   timer_p->timer_period=period;
   timer_p->owner_tcb_p=current_tcb_p;
   timer_p->trigger_flag=flag;

   // insert into the timer list

   presto_lock_save(lock);
   if(timer_list==NULL) {
      // we are the first timer in the list
      timer_list=timer_p;
      timer_p->next=NULL;
   } else if(clock_compare(&timer_list->delivery_time,&timer_p->delivery_time)>0) {
      // advance to the head of the class!
      timer_p->next=timer_list;
      timer_list=timer_p;
   } else {
      // we are one of many timers in the list
      PRESTO_TIMER_T * temp_p=timer_list;
      while(temp_p->next!=NULL) {
         if(clock_compare(&temp_p->next->delivery_time,&timer_p->delivery_time)>0) break;
         temp_p=temp_p->next;
      }
      // temp_p->next is either NULL or later delivery time than us
      // either way, we want to get inserted between temp_p and temp_p->next
      timer_p->next=temp_p->next;
      temp_p->next=timer_p;
   }
   presto_unlock_restore(lock);

}

////////////////////////////////////////////////////////////////////////////////

STATIC BYTE timer_CheckForExpiration(void) {
   BYTE count=0;
   PRESTO_TIMER_T * timer_p;
   WORD lock;

   presto_lock_save(lock);
   while((timer_list!=NULL)&&(clock_compare(&timer_list->delivery_time,&presto_master_clock)<=0)) {

      // remove timer from master list
      timer_p=timer_list;                      // we know that timer_list!=NULL
      timer_list=timer_list->next;

      set_flag(timer_p->owner_tcb_p, timer_p->trigger_flag);

      if(timer_p->timer_period>0) {
         // This timer is a repeating timer.
         // Keep the structure, but update the delivery time.
         clock_add_ms(&timer_p->delivery_time,timer_p->timer_period);

         // TODO - re-add the timer to the linked list
      }

      // indicate that a timer expired
      count++;
   }
   presto_unlock_restore(lock);
   return count;
}

////////////////////////////////////////////////////////////////////////////////
//   I N T E R N A L   M E S S A G E   H A N D L I N G
////////////////////////////////////////////////////////////////////////////////

/*

STATIC PRESTO_MSGID_T msg_Create(PRESTO_TID_T to, unsigned short delay, unsigned short period, PRESTO_MAIL_T payload) {
   PRESTO_MESSAGE_T * new_mail_p;
   PRESTO_TCB_T * to_tcb_p;
   WORD lock;

   // check to see if there's room
   if(free_mail_p==NULL) {
      presto_fatal_error(ERROR_TIMER_NOFREE);
   }

   // check to see that the recipient is a live task
   to_tcb_p=tid_to_tcbptr(to);
   if(to_tcb_p==NULL) {
      presto_fatal_error(ERROR_TIMER_TONOBODY);
   }

   // allocate space for a new message
   presto_lock_save(lock);
   new_mail_p=free_mail_p;
   free_mail_p=free_mail_p->next;

   // fill in the blanks
   // we never cover up new_mail_p->serial_number
   new_mail_p->from_tid=current_tcb_p->task_id;
   new_mail_p->to_tcb_p=to_tcb_p;
   new_mail_p->delivery_time=presto_master_clock;
   clock_add_ms(&new_mail_p->delivery_time,delay);
   new_mail_p->period=period;
   new_mail_p->payload=payload;
   presto_unlock_restore(lock);

   if(delay>0) {
      msg_InsertIntoPostOffice(new_mail_p);
   } else {
      msg_InsertIntoMailbox(new_mail_p,to_tcb_p);
      // receiver becomes ready...
      // time to re-evaluate highest ready task
      asm("swi");
   }

   return new_mail_p->serial_number;
}

*/

////////////////////////////////////////////////////////////////////////////////

/*

STATIC void msg_InsertIntoPostOffice(PRESTO_MESSAGE_T * new_mail_p) {
   WORD lock;
   // insert new message into list in time order
   presto_lock_save(lock);
   if(po_mail_p==NULL) {
      // we are the first message in PO
      po_mail_p=new_mail_p;
      new_mail_p->next=NULL;
   } else if(clock_compare(&po_mail_p->delivery_time,&new_mail_p->delivery_time)>0) {
      // advance to the head of the class!
      new_mail_p->next=po_mail_p;
      po_mail_p=new_mail_p;
   } else {
      // we are one of many messages in the PO
      PRESTO_MESSAGE_T * msg_p=po_mail_p;
      while(msg_p->next!=NULL) {
         if(clock_compare(&msg_p->next->delivery_time,&new_mail_p->delivery_time)>0) break;
         msg_p=msg_p->next;
      }
      // msg_p->next is either NULL or later delivery time than us
      // either way, we want to get inserted between msg_p and msg_p->next
      new_mail_p->next=msg_p->next;
      msg_p->next=new_mail_p;
   }
   presto_unlock_restore(lock);
}

*/

////////////////////////////////////////////////////////////////////////////////

/*

STATIC void msg_InsertIntoMailbox(PRESTO_MESSAGE_T * msg_p, PRESTO_MAILBOX_T * dest_mb_p) {
   WORD lock;
   PRESTO_TCB_T * owner_tcb_p;
   if(dest_mb_p==NULL) {
      presto_fatal_error(ERROR_MAIL_DESTBOXNULL);
   }
   presto_lock_save(lock);
   // move the message to the tail of the task's mail list
   if(dest_mb_p->mailbox_head==NULL) {
      // we are the only message in the list
      dest_mb_p->mailbox_head=msg_p;
      dest_mb_p->mailbox_tail=msg_p;
      message_count=1;
   } else {
      // we are one of many, add to the tail of the list
      dest_mb_p->mailbox_tail->next=msg_p;
      dest_mb_p->mailbox_tail=msg_p;
      message_count++;
   }
   // no matter what, we are the last in the task's message list
   msg_p->next=NULL;
   // make mailbox owner ready
   owner_tcb_p=dest_mb_p->owner_tcb_p;
   if(owner_tcb_p!=NULL) {
      owner_tcb_p.flags|=dest_mb_p->trigger_flag;
   }
   presto_unlock_restore(lock);
}

*/

////////////////////////////////////////////////////////////////////////////////

/*

STATIC BYTE msg_DeliverToMailboxes(void) {
   BYTE count=0;
   PRESTO_MESSAGE_T * msg_p;
   PRESTO_MAILBOX_T * temp_tcb_p;
   WORD lock;
   presto_lock_save(lock);
   while((po_mail_p!=NULL)&&(clock_compare(&po_mail_p->delivery_time,&presto_master_clock)<=0)) {
      // we're going to use this a lot, so de-reference once
      temp_tcb_p=po_mail_p->to_tcb_p;
      if(temp_tcb_p==NULL) {
         presto_fatal_error(ERROR_DELIVER_TCBPNULL);
      }

      // remove message from PO list
      msg_p=po_mail_p;                      // we know that po_mail_p!=NULL
      po_mail_p=po_mail_p->next;

      if(msg_p->period>0) {
         // this message is a repeating timer
         // create a copy of the message and re-insert it into the post office

         PRESTO_MESSAGE_T * duplicate_mail_p;

         // check to see if there's room
         if(free_mail_p==NULL) {
            presto_fatal_error(ERROR_DELIVER_NOFREE);
         }

         // allocate space for a new message
         duplicate_mail_p=free_mail_p;
         free_mail_p=free_mail_p->next;

         // fill in the blanks
         // duplicate_mail_p->serial_number;   already set
         duplicate_mail_p->from_tid=msg_p->from_tid;
         duplicate_mail_p->to_tcb_p=msg_p->to_tcb_p;
         duplicate_mail_p->delivery_time=msg_p->delivery_time;
         clock_add_ms(&duplicate_mail_p->delivery_time,msg_p->period);
         duplicate_mail_p->period=msg_p->period;
         duplicate_mail_p->payload=msg_p->payload;

         msg_InsertIntoPostOffice(duplicate_mail_p);
      }

      msg_InsertIntoMailbox(msg_p, temp_tcb_p);

      // indicate that we moved one mail message
      count++;
   }
   presto_unlock_restore(lock);
   return count;
}

*/

////////////////////////////////////////////////////////////////////////////////
//   S Y S T E M   C L O C K
////////////////////////////////////////////////////////////////////////////////

void presto_get_clock(PRESTO_TIME_T * clk) {
   *clk=presto_master_clock;
}

////////////////////////////////////////////////////////////////////////////////
//   T I M E R   I N T E R R U P T
////////////////////////////////////////////////////////////////////////////////

STATIC void systimer_Start(void) {
   // store (current plus CYCLES_PER_TICK)
   TOC2 = (WORD)(TCNT + CLOCKS_PER_TICK);
   // request output compare interrupt
   TMSK1 |= TMSK1_OC2I;
   // clear the OUTPUT COMPARE flag
   // writing O's makes no change, writing 1's clears the bit
   TFLG1 = TFLG1_OC2F;
   // counter disconnected from output pin logic
   TCTL1 &= ~(TCTL1_OM2|TCTL1_OL2);
}

////////////////////////////////////////////////////////////////////////////////

STATIC void systimer_Restart(void) {
   // store (last plus CLOCKS_PER_TICK)
   TOC2 = (WORD)(TOC2 + CLOCKS_PER_TICK);
   // clear the OUTPUT COMPARE flag
   // writing O's makes no change, writing 1's clears the bit
   TFLG1 = TFLG1_OC2F;
}

////////////////////////////////////////////////////////////////////////////////

STATIC void systimer_ISR(void) {
   WORD lock;
   // take care of clock things
   clock_add_ms(&presto_master_clock,MS_PER_TICK);
   systimer_Restart();
   // check timers
   if(timer_CheckForExpiration()>0) {
      asm("swi");
   }
}

////////////////////////////////////////////////////////////////////////////////
//   C O N T E X T   S W I T C H I N G   ( I N T E R R U P T )
////////////////////////////////////////////////////////////////////////////////

STATIC void context_switch_isr(void) {
   // registers are pushed when SWI is executed
   // pseudo-registers are also pushed (tmp,z,xy)

   #ifdef CHECK_STACK_CLOBBERING
      // check to see if the old task has clobbered its stack
      if(((current_tcb_p->stack_ptr)>(current_tcb_p->stack_top))
      ||((current_tcb_p->stack_ptr)<(current_tcb_p->stack_bottom)))
         presto_fatal_error(ERROR_CONTEXTSWITCH_STACKCLOBBERED);
   #endif // CHECK_STACK_CLOBBERING

   // the inline asm will save old SP in old TCB
   old_task_stack_pointer_p=&(current_tcb_p->stack_ptr);

   // pick next task to run
   old_tcb_p=current_tcb_p;
   current_tcb_p=scheduler_FindNextReadyTask();

   // check to see if the same task won...
   // only manipulate stacks if there is a context SWITCH
   if(current_tcb_p!=old_tcb_p) {

      // there's a new "highest priority ready task"

      #ifdef CHECK_STACK_CLOBBERING
         // check to see if the new task has clobbered its stack
         if(((current_tcb_p->stack_ptr)>(current_tcb_p->stack_top))
         ||((current_tcb_p->stack_ptr)<(current_tcb_p->stack_bottom)))
            presto_fatal_error(ERROR_CONTEXTSWITCH_STACKCLOBBERED);
      #endif // CHECK_STACK_CLOBBERING

      // call asm routine to set up new stack
      // when we return, we'll be another process
      // the asm routine will re-enable interrupts
      global_new_sp=current_tcb_p->stack_ptr;

      // store the old stack pointer in a global place
      asm("sts global_old_sp");
      // put the old stack pointer into the old task's TCB
      *old_task_stack_pointer_p=global_old_sp;
      // load the new stack pointer
      asm("lds global_new_sp");

   }

   // pseudo-registers are pulled (xy,z,tmp)
   // then normal registers are pulled
}

////////////////////////////////////////////////////////////////////////////////
//   I D L E   T A S K
////////////////////////////////////////////////////////////////////////////////

STATIC void idle_task(void) {
   while(1) {
      // do nothing
   }
}

////////////////////////////////////////////////////////////////////////////////
//   U T I L I T I E S
////////////////////////////////////////////////////////////////////////////////

STATIC PRESTO_TCB_T * tid_to_tcbptr(BYTE tid) {
   if(tid>=MAX_TASKS) {
      presto_fatal_error(ERROR_TIDTOTCB_RANGE);
   }
   if(tcb_list[tid].in_use==FALSE) return NULL;
   return &tcb_list[tid];
}

////////////////////////////////////////////////////////////////////////////////
