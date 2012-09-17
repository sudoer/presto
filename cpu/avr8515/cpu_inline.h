
#ifndef _CPU_INLINE_H_
#define _CPU_INLINE_H_

////////////////////////////////////////////////////////////////////////////////

#include <avr/io.h>
#include "avr_regs.h"
#include "types.h"

////////////////////////////////////////////////////////////////////////////////

static inline BYTE * cpu_inline_setup_stack(BYTE * stack_ptr, void (*func)()) {
   MISCWORD xlate;     // to split a word into two bytes
   xlate.w=(WORD)func;
   *stack_ptr--=xlate.b.l;    // function pointer(L)
   *stack_ptr--=xlate.b.h;    // function pointer(H)
   *stack_ptr--=0x00;         // __zero_reg__
   *stack_ptr--=0x00;         // __tmp_reg__
   *stack_ptr--=0x00;         // __SREG__
   *stack_ptr--=0x00;         // r16
   *stack_ptr--=0x00;         // r17
   *stack_ptr--=0x00;         // r18
   *stack_ptr--=0x00;         // r19
   *stack_ptr--=0x00;         // r20
   *stack_ptr--=0x00;         // r21
   *stack_ptr--=0x00;         // r22
   *stack_ptr--=0x00;         // r23
   *stack_ptr--=0x00;         // r24
   *stack_ptr--=0x00;         // r25
   *stack_ptr--=0x00;         // r26 (XL)
   *stack_ptr--=0x00;         // r27 (XH)
/* *stack_ptr--=0x00; */      // r28 (YL)
   *stack_ptr--=0x00;         // r29 (YH)
   *stack_ptr--=0x00;         // r30 (ZL)
   *stack_ptr--=0x00;         // r31 (ZH)
   return stack_ptr;
}

////////////////////////////////////////////////////////////////////////////////

static inline void cpu_inline_initialize_software_interrupt(void (*func)()) {
   // enable INT0 interrupt
   cbi(GIMSK,B_INT0);                      // disable INT0
   outb(MCUCR,inb(MCUCR)|M_ISC01|M_ISC00); // INT0 on rising edge
   cbi(PORTD,B_PD2);                       // drive INT0 (PORTD.2) low
   sbi(DDRD,B_DDD2);                       // INT0 (PORTD.2) is output
   sbi(GIMSK,B_INT0);                      // enable INT0
}

////////////////////////////////////////////////////////////////////////////////

// static inline void cpu_inline_launch_first_task(BYTE * task_sp) {
#define cpu_inline_launch_first_task(task_sp)                     \
   outw(SPL,(unsigned short)(task_sp));                           \
   asm volatile ("pop r31");                                      \
   asm volatile ("pop r30");                                      \
   asm volatile ("pop r29");                                      \
 /*asm volatile ("pop r28");*/                                    \
   asm volatile ("pop r27");                                      \
   asm volatile ("pop r26");                                      \
   asm volatile ("pop r25");                                      \
   asm volatile ("pop r24");                                      \
   asm volatile ("pop r23");                                      \
   asm volatile ("pop r22");                                      \
   asm volatile ("pop r21");                                      \
   asm volatile ("pop r20");                                      \
   asm volatile ("pop r19");                                      \
   asm volatile ("pop r18");                                      \
   asm volatile ("pop r17");                                      \
   asm volatile ("pop r16");                                      \
   asm volatile ("pop __tmp_reg__");                              \
   asm volatile ("out __SREG__,__tmp_reg__");                     \
   asm volatile ("pop __tmp_reg__");                              \
   asm volatile ("pop __zero_reg__");                             \
   asm volatile ("sei");                                          \
   /* let the CPU pop the PC, and start running!              */  \
   asm volatile ("reti");


////////////////////////////////////////////////////////////////////////////////

static inline void cpu_inline_software_interrupt() {
   cbi(PORTD,2);         // drive INT0 low
   sbi(PORTD,2);         // drive INT0 high
   // interrupted here
}

////////////////////////////////////////////////////////////////////////////////

static inline void cpu_inline_swi_start() {

   // Mark registers r16-r31 as "dirty", so the compiler will
   // push/pop them in the interrupt entry/exit code.
   asm volatile ("" : : : "r16","r17","r18","r19","r20","r21","r22","r23","r24","r25","r26","r27","r28","r29","r30","r31");

   cbi(PORTD,2);         // drive INT0 low
   sbi(GIMSK,B_INT0);    // re-enable INT0

   // blink an LED
   sbi(PORTB,6);
}

////////////////////////////////////////////////////////////////////////////////

static inline void cpu_inline_indicate_task_switch() {
   cbi(PORTB,5);
   sbi(PORTB,5);
}

////////////////////////////////////////////////////////////////////////////////

#define cpu_inline_swap_stack_pointers(old_stack_ptr_p,new_sp) \
   *old_stack_ptr_p=(BYTE *)inw(SPL);                          \
   outw(SPL,(unsigned short)(new_sp));


////////////////////////////////////////////////////////////////////////////////

static inline void cpu_inline_swi_end() {

   // The AVR sets the I bit in SREG upon executing a RETI instruction.

   // blink an LED
   cbi(PORTB,6);
}

////////////////////////////////////////////////////////////////////////////////

static inline void cpu_inline_idle_work() {
   cbi(PORTB,4);
   sbi(PORTB,4);
}

////////////////////////////////////////////////////////////////////////////////


#endif

