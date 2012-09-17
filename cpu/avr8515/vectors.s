
.text
;.section .vectors,"x"

   rjmp reset_vector          ; RESET
   rjmp context_switch_isr    ; INT0
   rjmp bad_intr_1            ; INT1
   rjmp bad_intr_2            ; TIMER1_CAPT
   rjmp timer_isr             ; TIMER1_COMPA
   rjmp bad_intr_3            ; TIMER1_COMPB
   rjmp bad_intr_4            ; TIMER1_OVF
   rjmp bad_intr_5            ; TIMER0_OVF
   rjmp bad_intr_6            ; SPI,STC
   rjmp bad_intr_7            ; UART_RX
   rjmp bad_intr_8            ; UART_UDRE
   rjmp bad_intr_9            ; UART_TX
   rjmp bad_intr_10           ; ANA_COMP

