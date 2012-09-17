
#include "presto.h"
#include "types.h"
#include "locks.h"

#include "avr_regs.h"
#include <avr/io.h>


#define STACK_SIZE  80

PRESTO_TASKID_T hi_tid;
PRESTO_TASKID_T md_tid;
PRESTO_TASKID_T lo_tid;
static BYTE hi_stack[STACK_SIZE];
static BYTE md_stack[STACK_SIZE];
static BYTE lo_stack[STACK_SIZE];

#define LED0_ON   0x01
#define LED0_OFF  0x02
#define LED1_ON   0x04
#define LED1_OFF  0x08


////////////////////////////////////////////////////////////////////////////////

void hi(void) {
   PRESTO_TRIGGER_T t;
   while(1) {
      t=presto_wait(LED1_ON|LED1_OFF|LED0_ON|LED0_OFF);
      if(t&LED0_ON)  { cbi(PORTB,0); }
      if(t&LED0_OFF) { sbi(PORTB,0); }
      if(t&LED1_ON)  { cbi(PORTB,1); }
      if(t&LED1_OFF) { sbi(PORTB,1); }
   }
}

////////////////////////////////////////////////////////////////////////////////

static PRESTO_TIMER_T mdt;

void md(void) {
   while(1) {
      presto_trigger_send(hi_tid,LED0_ON);
      presto_timer_start(&mdt,1000,0,0x10);
      presto_wait(0x10);
      presto_trigger_send(hi_tid,LED0_OFF);
      presto_timer_start(&mdt,1000,0,0x10);
      presto_wait(0x10);
   }
}

////////////////////////////////////////////////////////////////////////////////

static PRESTO_TIMER_T lot;

void lo(void) {
   presto_timer_start(&lot,125,250,0x10);
   presto_wait(0x10);
   while(1) {
      presto_trigger_send(hi_tid,LED1_ON);
      presto_wait(0x10);
      presto_trigger_send(hi_tid,LED1_OFF);
      presto_wait(0x10);
   }
}

////////////////////////////////////////////////////////////////////////////////

int main(void) {
   // start multi-tasking
   presto_init();
   hi_tid=presto_task_create(hi,  hi_stack,  STACK_SIZE, 20);
   md_tid=presto_task_create(md,  md_stack,  STACK_SIZE, 15);
   lo_tid=presto_task_create(lo,  lo_stack,  STACK_SIZE, 10);
   presto_scheduler_start();
   return 0;
}

////////////////////////////////////////////////////////////////////////////////


