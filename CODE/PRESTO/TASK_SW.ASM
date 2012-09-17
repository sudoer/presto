
.data
   _presto_asm_save_sp:
   .blkw 1
.area idata
   .word 0    ; _presto_asm_save_sp
.area text

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

CCR_INTERRUPT=0x10
ISR_LED_PORT=0x1008       ; PORTD
ISR_LED_MASK=0x10         ; PIN4

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

_presto_system_isr::
; uses global presto_asm_old_sp_p
; uses global presto_asm_new_sp
; uses global presto_asm_swap

   ; turn off interrupts
   sei
   ; LED on
;  ldaa  ISR_LED_PORT
;  anda  ~#ISR_LED_MASK
;  staa  ISR_LED_PORT
   ; increment clock, check if we need to pre-empt
   jsr   _presto_service_timer_interrupt
   ; check to see if we need to switch tasks
   tst   _presto_asm_swap
   beq   psi_rti
   ; toggle the speaker
;  ldaa  $1000
;  eora  #$08
;  staa  $1000
  ; swap the stack pointers
   ldx   _presto_asm_old_sp_p
   sts   0,x
   lds   _presto_asm_new_sp
psi_rti:
   ; clear interrupt mask bit (enable ints) in the CC register on the stack
   pula
   anda  ~#CCR_INTERRUPT
   psha
   ; LED off
;  ldaa  ISR_LED_PORT
;  oraa  ISR_LED_MASK
;  staa  ISR_LED_PORT
   ; "run" new task
   rti

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

_presto_switch_tasks::
; uses global presto_asm_old_sp_p
; uses global presto_asm_new_sp

   ; LED on
;  ldaa  ISR_LED_PORT
;  anda  ~#ISR_LED_MASK
;  staa  ISR_LED_PORT
   ; save the registers (in the same order that an interrupt does)
   pshy
   pshx
   psha
   pshb
   tpa
   anda  ~#CCR_INTERRUPT     ; enable interrupts in pushed CC register
   psha
   ; swap the stack pointers
   ldx   _presto_asm_old_sp_p
   sts   0,x
   ; fall through to the "half-function" below
_presto_start_task_switching::
   lds   _presto_asm_new_sp
   ; restore the registers
   pula
   oraa  #CCR_INTERRUPT      ; do not enable interrupts in the CC register... yet
   tap
   pulb
   pula
   pulx
   puly
   ; NOW we can re-enable interrupts, because we are done with the stack
   cli
   ; LED off
;  ldaa  ISR_LED_PORT
;  oraa  ISR_LED_MASK
;  staa  ISR_LED_PORT
   ; "run" new task
   rts

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

_presto_setup_new_task::
; uses global presto_asm_new_sp

   ; store our own SP so we can work on the new task
   sts _presto_asm_save_sp
   ; load empty SP from task so we can initialize it
   lds _presto_asm_new_sp
   ; make presto_kill_self as the "return pc" of a new task,
   ; so if it ever returns, it will reclaim the TCB, etc
   ldd   #_presto_kill_self
   pshb
   psha
   ; push the actual function call on the stack
   ldd   _presto_asm_new_fn
   pshb
   psha
   ; push any old stinkin' registers onto the stack
   ; they'll be pulled off when we start running
   ; we push in interrupt-stack order
   pshy
   pshx
   psha
   pshb
   tpa
   anda  ~#CCR_INTERRUPT     ; enable interrupts in pushed CC
   psha
   ; save task SP in TCB
   sts   _presto_asm_new_sp
   ; re-load our own SP so we can return
   lds   _presto_asm_save_sp
   rts

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


