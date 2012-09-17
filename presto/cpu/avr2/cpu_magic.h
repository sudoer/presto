
#ifndef _CPU_MAGIC_H_
#define _CPU_MAGIC_H_

////////////////////////////////////////////////////////////////////////////////

#include <avr/io.h>
#include "registers.h"
#include "types.h"
#include "avr_board.h"

////////////////////////////////////////////////////////////////////////////////
//   I N I T I A L I Z E
////////////////////////////////////////////////////////////////////////////////

static inline BYTE * CPU_MAGIC_SETUP_STACK(BYTE * stack_ptr, void (*func)(void)) {
   MISCWORD xlate;     // to split a word into two bytes
   xlate.w=(WORD)func;
   *stack_ptr--=xlate.b.l;    // function pointer(L)
   *stack_ptr--=xlate.b.h;    // function pointer(H)
   *stack_ptr--=0x00;         // __zero_reg__
   *stack_ptr--=0x00;         // __tmp_reg__
   *stack_ptr--=0x00;         // __SREG__
   stack_ptr-=30;             // r2 - r31 can be any value
   return stack_ptr;
}

////////////////////////////////////////////////////////////////////////////////

static inline void CPU_MAGIC_INITIALIZE_SOFTWARE_INTERRUPT(void (*func)(void)) {
   NOT_USED(func);
   cbi(GIMSK,B_INT0);                      // disable INT0
   outb(MCUCR,inb(MCUCR)|M_ISC01|M_ISC00); // INT0 on rising edge
   cbi(PORTD,B_PD2);                       // drive INT0 (PORTD.2) low
   sbi(DDRD,B_DDD2);                       // INT0 (PORTD.2) is output
   sbi(GIMSK,B_INT0);                      // enable INT0
}

////////////////////////////////////////////////////////////////////////////////
//   S T A C K   P O I N T E R S
////////////////////////////////////////////////////////////////////////////////

#define CPU_MAGIC_LOAD_STACK_PTR(task_sp)  \
   outw(SPL,(unsigned short)(task_sp));

////////////////////////////////////////////////////////////////////////////////

#define CPU_MAGIC_SWAP_STACK_POINTERS(old_stack_ptr_p,new_sp)  \
   *old_stack_ptr_p=(BYTE *)inw(SPL);                             \
   outw(SPL,(unsigned short)(new_sp));

////////////////////////////////////////////////////////////////////////////////
//   T R I G G E R I N G   S W I
////////////////////////////////////////////////////////////////////////////////

// AVR pushes PC (that's all!)
// compiler pushes several registers and SREG

static inline void CPU_MAGIC_SOFTWARE_INTERRUPT(void) {
   cbi(PORTD,2);          // drive INT0 low
   sbi(PORTD,2);          // drive INT0 high
   asm volatile ("sei");  // enable interrupts
}

////////////////////////////////////////////////////////////////////////////////
//   S W I   E N T R Y / E X I T
////////////////////////////////////////////////////////////////////////////////

#define CPU_MAGIC_DECLARE_SWI(x) void x (void) __attribute__((naked));

////////////////////////////////////////////////////////////////////////////////

//static inline void CPU_MAGIC_START_OF_SWI(void) {
#define CPU_MAGIC_START_OF_SWI()            \
   asm volatile ("push __zero_reg__");         \
   asm volatile ("push __tmp_reg__");          \
   asm volatile ("in __tmp_reg__,__SREG__");   \
   asm volatile ("push __tmp_reg__");          \
   asm volatile ("clr __zero_reg__");          \
   asm volatile ("push r2");                   \
   asm volatile ("push r3");                   \
   asm volatile ("push r4");                   \
   asm volatile ("push r5");                   \
   asm volatile ("push r6");                   \
   asm volatile ("push r7");                   \
   asm volatile ("push r8");                   \
   asm volatile ("push r9");                   \
   asm volatile ("push r10");                  \
   asm volatile ("push r11");                  \
   asm volatile ("push r12");                  \
   asm volatile ("push r13");                  \
   asm volatile ("push r14");                  \
   asm volatile ("push r15");                  \
   asm volatile ("push r16");                  \
   asm volatile ("push r17");                  \
   asm volatile ("push r18");                  \
   asm volatile ("push r19");                  \
   asm volatile ("push r20");                  \
   asm volatile ("push r21");                  \
   asm volatile ("push r22");                  \
   asm volatile ("push r23");                  \
   asm volatile ("push r24");                  \
   asm volatile ("push r25");                  \
   asm volatile ("push r26");                  \
   asm volatile ("push r27");                  \
   asm volatile ("push r28");                  \
   asm volatile ("push r29");                  \
   asm volatile ("push r30");                  \
   asm volatile ("push r31");                  \
   cbi(PORTD,2);       /* drive INT0 low */    \
   sbi(GIMSK,B_INT0);  /* re-enable INT0 */

////////////////////////////////////////////////////////////////////////////////

//static inline void CPU_MAGIC_END_OF_SWI(void) {
#define CPU_MAGIC_END_OF_SWI()                   \
   asm volatile ("pop r31");                        \
   asm volatile ("pop r30");                        \
   asm volatile ("pop r29");                        \
   asm volatile ("pop r28");                        \
   asm volatile ("pop r27");                        \
   asm volatile ("pop r26");                        \
   asm volatile ("pop r25");                        \
   asm volatile ("pop r24");                        \
   asm volatile ("pop r23");                        \
   asm volatile ("pop r22");                        \
   asm volatile ("pop r21");                        \
   asm volatile ("pop r20");                        \
   asm volatile ("pop r19");                        \
   asm volatile ("pop r18");                        \
   asm volatile ("pop r17");                        \
   asm volatile ("pop r16");                        \
   asm volatile ("pop r15");                        \
   asm volatile ("pop r14");                        \
   asm volatile ("pop r13");                        \
   asm volatile ("pop r12");                        \
   asm volatile ("pop r11");                        \
   asm volatile ("pop r10");                        \
   asm volatile ("pop r9");                         \
   asm volatile ("pop r8");                         \
   asm volatile ("pop r7");                         \
   asm volatile ("pop r6");                         \
   asm volatile ("pop r5");                         \
   asm volatile ("pop r4");                         \
   asm volatile ("pop r3");                         \
   asm volatile ("pop r2");                         \
   asm volatile ("pop __tmp_reg__");                \
   asm volatile ("out __SREG__,__tmp_reg__");       \
   asm volatile ("pop __tmp_reg__");                \
   asm volatile ("pop __zero_reg__");               \
   /* let the CPU pop the PC and start running! */  \
   asm volatile ("reti");

////////////////////////////////////////////////////////////////////////////////

#define CPU_MAGIC_RUN_FIRST_TASK() CPU_MAGIC_END_OF_SWI();

////////////////////////////////////////////////////////////////////////////////
//   I D L E   W O R K
////////////////////////////////////////////////////////////////////////////////

static inline void CPU_MAGIC_IDLE_WORK(void) {
   cbi(PORTB,LED_IDLE);  // LED on
}

////////////////////////////////////////////////////////////////////////////////

#endif // _CPU_MAGIC_H_

