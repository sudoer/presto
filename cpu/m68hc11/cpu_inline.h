
#ifndef _CPU_INLINE_H_
#define _CPU_INLINE_H_

////////////////////////////////////////////////////////////////////////////////

#include "hc11regs.h"
#include "handyboard.h"
#include "intvect.h"
#include "types.h"

////////////////////////////////////////////////////////////////////////////////

inline BYTE * cpu_inline_setup_stack(BYTE * sp, void (*func)()) {
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

inline void cpu_inline_initialize_software_interrupt(void (*func)()) {
   set_interrupt(INTR_SWI, func);
}

////////////////////////////////////////////////////////////////////////////////

// inline void cpu_inline_launch_first_task(BYTE * first_sp) {
#define cpu_inline_launch_first_task(first_sp)                     \
                                                                   \
   /* load the new stack pointer                               */  \
   asm volatile ("lds %0" : : "m"(first_sp) );                     \
                                                                   \
   /* pop the stuff that the compiler normally pops for you    */  \
   asm volatile ("pulx");  /* _.xy                             */  \
   asm volatile ("pulx");  /* _.z                              */  \
   asm volatile ("pulx");  /* _.tmp                            */  \
                                                                   \
   /* let the CPU pop the registers and PC, and start running! */  \
   asm volatile ("rti");


////////////////////////////////////////////////////////////////////////////////

inline void cpu_inline_software_interrupt() {
   asm volatile ("swi");
}

////////////////////////////////////////////////////////////////////////////////

inline void cpu_inline_swi_start() {
}

////////////////////////////////////////////////////////////////////////////////

inline void cpu_inline_indicate_task_switch() {
   TOGGLE_SPEAKER();
}

////////////////////////////////////////////////////////////////////////////////

// inline void cpu_inline_swap_stack_pointers(BYTE ** old_stack_ptr_p, BYTE * new_sp) {
#define cpu_inline_swap_stack_pointers(old_stack_ptr_p, new_sp) \
   asm volatile ("sts %0" : "=m"(*old_stack_ptr_p) );           \
   asm volatile ("lds %0" : : "m"(new_sp) );


////////////////////////////////////////////////////////////////////////////////

inline void cpu_inline_swi_end() {
}

////////////////////////////////////////////////////////////////////////////////

inline void cpu_inline_idle_work() {
   // Wait for an interrupt.  The WAI instruction places the CPU in
   // a low power "wait" mode.  Plus, it pre-stacks the registers for
   // a slightly faster interrupt response time.
   asm volatile ("wai");
}

////////////////////////////////////////////////////////////////////////////////

#endif

