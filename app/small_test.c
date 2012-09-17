
#include "presto.h"
#include "types.h"
#include "locks.h"

#include "avr_regs.h"
#include <avr/io.h>


#define STACK_SIZE  60

PRESTO_TASKID_T led_tid;
PRESTO_TASKID_T hgh_tid;
PRESTO_TASKID_T med_tid;
PRESTO_TASKID_T low_tid;
static BYTE led_stack[STACK_SIZE];
static BYTE hgh_stack[STACK_SIZE];
static BYTE med_stack[STACK_SIZE];
static BYTE low_stack[STACK_SIZE];

#define LED0_ON   0x01
#define LED0_OFF  0x02
#define LED1_ON   0x04
#define LED1_OFF  0x08
#define LED2_ON   0x10
#define LED2_OFF  0x20


////////////////////////////////////////////////////////////////////////////////

void led(void) {
   PRESTO_TRIGGER_T t;
   while(1) {
      t=presto_wait(0xFF);
      if(t&LED0_ON)  { cbi(PORTB,0); }
      if(t&LED0_OFF) { sbi(PORTB,0); }
      if(t&LED1_ON)  { cbi(PORTB,1); }
      if(t&LED1_OFF) { sbi(PORTB,1); }
      if(t&LED2_ON)  { cbi(PORTB,2); }
      if(t&LED2_OFF) { sbi(PORTB,2); }
   }
}

////////////////////////////////////////////////////////////////////////////////

static PRESTO_TIMER_T hit;

void hgh(void) {
   presto_timer_start(&hit,100,100,0x80);
   while(1) {
      presto_trigger_send(led_tid,LED0_ON);
      presto_wait(0x80);
      presto_trigger_send(led_tid,LED0_OFF);
      presto_wait(0x80);
   }
}

////////////////////////////////////////////////////////////////////////////////

static PRESTO_TIMER_T mdt;

void med(void) {
   while(1) {
      presto_trigger_send(led_tid,LED1_ON);
      presto_timer_start(&mdt,500,0,0x80);
      presto_wait(0x80);
      presto_trigger_send(led_tid,LED1_OFF);
      presto_timer_start(&mdt,225,0,0x80);
      presto_wait(0x80);
   }
}

////////////////////////////////////////////////////////////////////////////////

static PRESTO_TIMER_T lot;

void low(void) {
   presto_timer_start(&lot,125,250,0x80);
   presto_wait(0x80);
   while(1) {
      presto_trigger_send(led_tid,LED2_ON);
      presto_wait(0x80);
      presto_trigger_send(led_tid,LED2_OFF);
      presto_wait(0x80);
   }
}

////////////////////////////////////////////////////////////////////////////////

int main(void) {
   // start multi-tasking
   presto_init();
   led_tid=presto_task_create(led,  led_stack,  STACK_SIZE, 25);
   hgh_tid=presto_task_create(hgh,  hgh_stack,  STACK_SIZE, 20);
   med_tid=presto_task_create(med,  med_stack,  STACK_SIZE, 15);
   low_tid=presto_task_create(low,  low_stack,  STACK_SIZE, 10);
   presto_scheduler_start();
   return 0;
}

////////////////////////////////////////////////////////////////////////////////


