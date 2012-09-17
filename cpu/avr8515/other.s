SREG   =0x3f
SPH    =0x3e
SPL    =0x3d
GIMSK  =0x3b
GIFR   =0x3a
TIMSK  =0x39
TIFR   =0x38
MCUCR  =0x35
TCCR0  =0x33
TCNT0  =0x32
TCCR1A =0x2f
TCCR1B =0x2e
TCNT1H =0x2d
TCNT1L =0x2c
OCR1AH =0x2b
OCR1AL =0x2a
OCR1BH =0x29
OCR1BL =0x28
ICR1H  =0x25
ICR1L  =0x24
WDTCR  =0x21
EEARH  =0x1f
EEARL  =0x1e
EEDR   =0x1d
EECR   =0x1c
PORTA  =0x1b
DDRA   =0x1a
PINA   =0x19
PORTB  =0x18
DDRB   =0x17
PINB   =0x16
PORTC  =0x15
DDRC   =0x14
PINC   =0x13
PORTD  =0x12
DDRD   =0x11
PIND   =0x10
SPDR   =0x0f
SPSR   =0x0e
SPCR   =0x0d
UDR    =0x0c
USR    =0x0b
UCR    =0x0a
UBRR   =0x09
ACSR   =0x08


.text

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.global _endless
_endless:
   rjmp _endless


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


.global go
go:

        ; configure PORTB as output pins
        ser     r16                    ; PORTB = all outputs
        out     DDRB,r16


        ; configure PORTD as input pins
        clr     r19                    ; PORTD = all inputs
        out     DDRD,r19

;**** Test input/output

loop:
        ; read r19
        in      r19,PIND
        out     PORTB,r19              ; output data to PORTD

;**** No wait a while to make LED changes visible.

DLY:
        ; count to 64k
        dec     r17
        brne    DLY
        dec     r18
        brne    DLY

        rjmp    loop                    ; repeat loop endlessly



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

