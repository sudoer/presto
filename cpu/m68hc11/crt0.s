
   .text
   .globl _start


_start:


; load initial stack pointer
   lds   #__stack_end-1

; call cpu-specific initialization
   jsr   premain


; clear BSS
   clra
   ldx   #__bss_start
bss_loop:
   cpx   #__bss_end
   beq   bss_done
   staa  0,x
   inx
   bra   bss_loop
bss_done:


; copy initialized idata to data
   ldx   #__idata_start
   ldy   #__data_start
idata_loop:
   cpx   #__idata_end
   beq idata_done
   ldab 0,x
   stab 0,y
   inx
   iny
   bra idata_loop
idata_done:


; call user main routine
   jsr main


_postmain:
   bra   _postmain


