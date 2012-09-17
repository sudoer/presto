

__SREG__ = 0x3f
__SP_H__ = 0x3e
__SP_L__ = 0x3d


.text
.globl _start
.globl _endless


_start:

   ; load initial stack pointer
   ldi    r16,lo8(__stack_end-1)
   ldi    r17,hi8(__stack_end-1)
   out    __SP_H__,r17
   out    __SP_L__,r16

   ; call cpu-specific initialization
   ; rcall  premain


   ; call user main routine
   rcall  main


_endless:
   rjmp   _endless


