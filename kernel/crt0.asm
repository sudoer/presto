;-------------------------------------------------------------------------------
; CRT0.ASM
; Basic definitions for the compiled code.
;-------------------------------------------------------------------------------

   .module crt0.s


;-------------------------------------------------------------------------------
; Null-Ptr Destination (hope, that nobody writes more than 2 bytes here)
;-------------------------------------------------------------------------------
   .area   DIRECT (ABS,PAG)
   .org  0
_os_null_ptr::
	.blkb 2


;-------------------------------------------------------------------------------
; Registers area
;   Steven L. Barnicki, 2/1/99
;-------------------------------------------------------------------------------
   .area   REGS (ABS)
	.org  0x1000
_porta::.blkb 1
	.blkb 1         ;reserved
_pioc:: .blkb 1
_portc::.blkb 1
_portb::.blkb 1
_portcl::.blkb 1
	.blkb 1         ;reserved
_ddrc:: .blkb 1
_portd::.blkb 1
_ddrd:: .blkb 1
_porte::.blkb 1
_cforc::.blkb 1
_oc1m:: .blkb 1
_oc1d:: .blkb 1
_tcnt:: .blkw 1
_tic1:: .blkw 1
_tic2:: .blkw 1
_tic3:: .blkw 1
_toc1:: .blkw 1
_toc2:: .blkw 1
_toc3:: .blkw 1
_toc4:: .blkw 1
_ti4::  .blkw 1
_tctl1::.blkb 1
_tctl2::.blkb 1
_tmsk1::.blkb 1
_tflg1::.blkb 1
_tmsk2::.blkb 1
_tflg2::.blkb 1
_pactl::.blkb 1
_pacnt::.blkb 1
_spcr:: .blkb 1
_spsr:: .blkb 1
_spdr:: .blkb 1
_baud:: .blkb 1
_sccr1::.blkb 1
_sccr2::.blkb 1
_scsr:: .blkb 1
_scdr:: .blkb 1
_adctl::.blkb 1
_adr1:: .blkb 1
_adr2:: .blkb 1
_adr3:: .blkb 1
_adr4:: .blkb 1
_bprot::.blkb 1
_eprog::.blkb 1
	.blkb 2	;reserved
_option::.blkb 1
_coprst::.blkb 1
_pprog::.blkb 1
_hprio::.blkb 1
_init:: .blkb 1
_test1::.blkb 1
_config::.blkb 1

;-------------------------------------------------------------------------------

.area   STARTUP      (ABS)
__start::
;         lds  #_stack_begin-1   ; initialize stack pointer
;         jsr  __HC11Setup



loop1:
         ldaa    #255
         staa    4105

         ldaa    #00
         staa    4104

         ldaa    #255
         staa    4104

         bra     loop1



         ldx     #0                 ; clear the NULL pointer
         stx     *_os_null_ptr

         cli
			jsr     _main              ; main()
__exit::
         bra     __exit

;-------------------------------------------------------------------------------

      .area CODE
_inert_isr::
         rti

;-------------------------------------------------------------------------------

.area   BOOTLIST     (ABS)
.org    0xbfd6
.word   _inert_isr           ; 0xffd6  SCI Serial System
.word   _inert_isr           ; 0xffd8  SPI Serial Transfer complete
.word   _inert_isr           ; 0xffda  Pulse Akku Input Edge
.word   _inert_isr           ; 0xffdc  Pulse Akku Overflow
.word   _inert_isr           ; 0xffde  Timer Overflow
.word   _inert_isr           ; 0xffe0
.word   _inert_isr           ; 0xffe2
.word   _inert_isr           ; 0xffe4
.word   _inert_isr           ; 0xffe6
.word   _inert_isr           ; 0xffe8
.word   _inert_isr           ; 0xffea
.word   _inert_isr           ; 0xffec
.word   _inert_isr           ; 0xffee
.word   _inert_isr           ; 0xfff0  Real Time Interrupt
.word   _inert_isr           ; 0xfff2  Maskable Interrupt, Interrupt Request (IRQ)
.word   _inert_isr           ; 0xfff4  Nonmaskable Interrupt Request (XIRQ)
.word   _inert_isr           ; 0xfff6  Software Interrupt (SWI)
.word   _inert_isr           ; 0xfff8  Illegal Opcode
.word   _inert_isr           ; 0xfffa  COP Watchdog Time-Out
.word   _inert_isr           ; 0xfffc  Clock Monitor Fail
.word   _start               ; 0xfffe  Power On Reset (POR) oder RESET Pin

;-------------------------------------------------------------------------------
; _os_set_irq
; C-Call: void os_set_irq(int number, void (*fn)() )

.area  _CODE
_ods_set_irq::
			pshy  ; Save stack frame
			tsy   ; Set current stack frame
			pshx

         ldd   2,y       ; load integer number from stack
         clra            ; truncate to 8-bit number (in 16-bit register)
         lsld            ; multiply by two (each entry in table is 2 bytes)
         addd  #0xbfd0   ; add base address of interrupt table
         xgdx            ; put it in X
         ldd   4,y       ; load address of function from stack
         std   0,x       ; store address in interrupt table

         pulx
			puly  ; Restore stack frame
			rts   ; return from function

;-------------------------------------------------------------------------------

