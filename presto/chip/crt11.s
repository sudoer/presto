USEHEAP = 0


   .text
   .globl _start


_start:
   lds   #__stack_end-1
   jsr   premain


; clear BSS
   clra
   ldx   #__bss_start
init_loop:
   cpx   #__bss_end
   beq   init_done
   staa  0,x
   inx
   bra   init_loop
init_done:


; copy initialized idata to data
; idata in ppage and data in dpage
   ldx   #__idata_start
   ldy   #__data_start
copy_loop:
   cpx   #__idata_end
   beq copy_done
   ldab 0,x
   stab 0,y
   inx
   iny
   bra copy_loop
copy_done:


; set up heap space
.if USEHEAP
   ldd   #heap_size
   beq   heap_done
   addd #__bss_end
   pshb
   psha
   ldd   #__bss_end
   jsr   __NewHeap
   pulx
.endif
heap_done:


; call user main routine
   jsr main

_postmain:
   bra   _postmain


;    ; __idata_start MUST follow text area for it to be in ROM
;   .area idata
;__idata_start::
;   .data
;__data_start::
;   .area bss
;__bss_start::
