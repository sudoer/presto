
#ifndef _KERNEL_MAGIC_H_
#define _KERNEL_MAGIC_H_

////////////////////////////////////////////////////////////////////////////////

#include "hc11_regs.h"
#include "handyboard.h"
#include "vectors.h"
#include "types.h"

////////////////////////////////////////////////////////////////////////////////
//   I N I T I A L I Z E
////////////////////////////////////////////////////////////////////////////////

static inline BYTE * KERNEL_MAGIC_SETUP_STACK(BYTE * sp, void (*func)()) {
   MISCWORD xlate;
   xlate.w=(WORD)func;   // split a word into two bytes
   *sp--=xlate.b.l;      // function pointer(L)
   *sp--=xlate.b.h;      // function pointer(H)
   *sp--=0x00;           // Y(L) register
   *sp--=0x00;           // Y(H) register
   *sp--=0x00;           // X(L) register
   *sp--=0x00;           // X(H) register
   *sp--=0x00;           // A register
   *sp--=0x00;           // B register
   *sp--=0x00;           // condition codes (I bit cleared)
   *sp--=0x00;           // _.tmp 0000(L)
   *sp--=0x00;           // _.tmp 0000(H)
   *sp--=0x00;           // _.z   0002(L)
   *sp--=0x00;           // _.z   0002(H)
   *sp--=0x00;           // _.xy  0004(L)
   *sp--=0x00;           // _.xy  0004(H)
   return sp;
}

////////////////////////////////////////////////////////////////////////////////

static inline void KERNEL_MAGIC_INITIALIZE_SOFTWARE_INTERRUPT(void (*func)()) {
   set_interrupt(INTR_SWI, func);
}

////////////////////////////////////////////////////////////////////////////////
//   S T A C K   P O I N T E R S
////////////////////////////////////////////////////////////////////////////////

#define KERNEL_MAGIC_LOAD_STACK_PTR(task_sp)    \
   asm volatile ("lds %0" : : "m"(task_sp) );

////////////////////////////////////////////////////////////////////////////////

#define KERNEL_MAGIC_SWAP_STACK_POINTERS(old_stack_ptr_p,new_sp) \
   asm volatile ("sts %0" : "=m"(*old_stack_ptr_p) );            \
   asm volatile ("lds %0" : : "m"(new_sp) );

////////////////////////////////////////////////////////////////////////////////
//   T R I G G E R I N G   S W I
////////////////////////////////////////////////////////////////////////////////

static inline void KERNEL_MAGIC_SOFTWARE_INTERRUPT() {
   asm volatile ("swi");
}

////////////////////////////////////////////////////////////////////////////////
//   S W I   E N T R Y / E X I T
////////////////////////////////////////////////////////////////////////////////

#define KERNEL_MAGIC_DECLARE_SWI(x) void x (void) __attribute__((interrupt));

////////////////////////////////////////////////////////////////////////////////

static inline void KERNEL_MAGIC_START_OF_SWI() {
/*
   asm volatile("ldx *_.tmp");
   asm volatile("pshx");
   asm volatile("ldx *_.z");
   asm volatile("pshx");
   asm volatile("ldx *_.xy");
   asm volatile("pshx");
*/
}

////////////////////////////////////////////////////////////////////////////////

static inline void KERNEL_MAGIC_END_OF_SWI() {
/*
   asm volatile("pulx");
   asm volatile("stx *_.xy");
   asm volatile("pulx");
   asm volatile("stx *_.z");
   asm volatile("pulx");
   asm volatile("stx *_.tmp");
   asm volatile("rti");
*/
}

////////////////////////////////////////////////////////////////////////////////

static inline void KERNEL_MAGIC_RUN_FIRST_TASK() {
   asm volatile("pulx");
   asm volatile("pulx");
   asm volatile("pulx");
   asm volatile("rti");
}

////////////////////////////////////////////////////////////////////////////////
//   I D L E   W O R K
////////////////////////////////////////////////////////////////////////////////

static inline void KERNEL_MAGIC_INDICATE_IDLE_WORK() {
   // Wait for an interrupt.  The WAI instruction places the CPU in
   // a low power "wait" mode.  Plus, it pre-stacks the registers for
   // a slightly faster interrupt response time.
   asm volatile ("wai");
}

////////////////////////////////////////////////////////////////////////////////
//   I N D I C A T I O N S / F E E D B A C K
////////////////////////////////////////////////////////////////////////////////

#define KERNEL_MAGIC_INDICATE_SWI_START()  ;
#define KERNEL_MAGIC_INDICATE_SWI_END()    ;

////////////////////////////////////////////////////////////////////////////////

#define KERNEL_MAGIC_INDICATE_TASK_SWITCH()  TOGGLE_SPEAKER();

////////////////////////////////////////////////////////////////////////////////

#define KERNEL_MAGIC_INDICATE_TICK_START()  ;
#define KERNEL_MAGIC_INDICATE_TICK_END()    ;

////////////////////////////////////////////////////////////////////////////////

#endif
























