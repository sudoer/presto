
#ifndef _AVR_REGS_H_
#define _AVR_REGS_H_

////////////////////////////////////////////////////////////////////////////////

// I/O Register Definitions
/*
#define    SREG    *(volatile unsigned char *)(0x3f)
#define    SPH     *(volatile unsigned char *)(0x3e)
#define    SPL     *(volatile unsigned char *)(0x3d)
#define    GIMSK   *(volatile unsigned char *)(0x3b)
#define    GIFR    *(volatile unsigned char *)(0x3a)
#define    TIMSK   *(volatile unsigned char *)(0x39)
#define    TIFR    *(volatile unsigned char *)(0x38)
#define    MCUCR   *(volatile unsigned char *)(0x35)
#define    TCCR0   *(volatile unsigned char *)(0x33)
#define    TCNT0   *(volatile unsigned char *)(0x32)
#define    TCCR1A  *(volatile unsigned char *)(0x2f)
#define    TCCR1B  *(volatile unsigned char *)(0x2e)
#define    TCNT1H  *(volatile unsigned char *)(0x2d)
#define    TCNT1L  *(volatile unsigned char *)(0x2c)
#define    OCR1AH  *(volatile unsigned char *)(0x2b)
#define    OCR1AL  *(volatile unsigned char *)(0x2a)
#define    OCR1BH  *(volatile unsigned char *)(0x29)
#define    OCR1BL  *(volatile unsigned char *)(0x28)
#define    ICR1H   *(volatile unsigned char *)(0x25)
#define    ICR1L   *(volatile unsigned char *)(0x24)
#define    WDTCR   *(volatile unsigned char *)(0x21)
#define    EEARH   *(volatile unsigned char *)(0x1f)
#define    EEARL   *(volatile unsigned char *)(0x1e)
#define    EEDR    *(volatile unsigned char *)(0x1d)
#define    EECR    *(volatile unsigned char *)(0x1c)
#define    PORTA   *(volatile unsigned char *)(0x1b)
#define    DDRA    *(volatile unsigned char *)(0x1a)
#define    PINA    *(volatile unsigned char *)(0x19)
#define    PORTB   *(volatile unsigned char *)(0x18)
#define    DDRB    *(volatile unsigned char *)(0x17)
#define    PINB    *(volatile unsigned char *)(0x16)
#define    PORTC   *(volatile unsigned char *)(0x15)
#define    DDRC    *(volatile unsigned char *)(0x14)
#define    PINC    *(volatile unsigned char *)(0x13)
#define    PORTD   *(volatile unsigned char *)(0x12)
#define    DDRD    *(volatile unsigned char *)(0x11)
#define    PIND    *(volatile unsigned char *)(0x10)
#define    SPDR    *(volatile unsigned char *)(0x0f)
#define    SPSR    *(volatile unsigned char *)(0x0e)
#define    SPCR    *(volatile unsigned char *)(0x0d)
#define    UDR     *(volatile unsigned char *)(0x0c)
#define    USR     *(volatile unsigned char *)(0x0b)
#define    UCR     *(volatile unsigned char *)(0x0a)
#define    UBRR    *(volatile unsigned char *)(0x09)
#define    ACSR    *(volatile unsigned char *)(0x08)
*/

////////////////////////////////////////////////////////////////////////////////

// Bit Definitions
#define    B_INT1          7
#define    B_INT0          6

#define    B_TOIE1         7
#define    B_OCIE1A        6
#define    B_OCIE1B        5
#define    B_TICIE1        3
#define    B_TOIE0         1

#define    B_TOV1          7
#define    B_OCF1A         6
#define    B_OCF1B         5
#define    B_ICF1          3
#define    B_TOV0          1

#define    B_SRE           7
#define    B_SRW           6
#define    B_SE            5
#define    B_SM            4
#define    B_ISC11         3
#define    B_ISC10         2
#define    B_ISC01         1
#define    B_ISC00         0

#define    B_COM1A1        7
#define    B_COM1A0        6
#define    B_COM1B1        5
#define    B_COM1B0        4
#define    B_PWM11         1
#define    B_PWM10         0

#define    B_ICNC1         7
#define    B_ICES1         6
#define    B_CTC1          3
#define    B_CS12          2
#define    B_CS11          1
#define    B_CS10          0

#define    B_WDTOE         4
#define    B_WDE           3
#define    B_WDP2          2
#define    B_WDP1          1
#define    B_WDP0          0

#define    B_EEMWE         2
#define    B_EEWE          1
#define    B_EERE          0

#define    B_PA7           7
#define    B_PA6           6
#define    B_PA5           5
#define    B_PA4           4
#define    B_PA3           3
#define    B_PA2           2
#define    B_PA1           1
#define    B_PA0           0

#define    B_DDA7          7
#define    B_DDA6          6
#define    B_DDA5          5
#define    B_DDA4          4
#define    B_DDA3          3
#define    B_DDA2          2
#define    B_DDA1          1
#define    B_DDA0          0

#define    B_PINA7         7
#define    B_PINA6         6
#define    B_PINA5         5
#define    B_PINA4         4
#define    B_PINA3         3
#define    B_PINA2         2
#define    B_PINA1         1
#define    B_PINA0         0

#define    B_PB7           7
#define    B_PB6           6
#define    B_PB5           5
#define    B_PB4           4
#define    B_PB3           3
#define    B_PB2           2
#define    B_PB1           1
#define    B_PB0           0

#define    B_DDB7          7
#define    B_DDB6          6
#define    B_DDB5          5
#define    B_DDB4          4
#define    B_DDB3          3
#define    B_DDB2          2
#define    B_DDB1          1
#define    B_DDB0          0

#define    B_PINB7         7
#define    B_PINB6         6
#define    B_PINB5         5
#define    B_PINB4         4
#define    B_PINB3         3
#define    B_PINB2         2
#define    B_PINB1         1
#define    B_PINB0         0

#define    B_PC7           7
#define    B_PC6           6
#define    B_PC5           5
#define    B_PC4           4
#define    B_PC3           3
#define    B_PC2           2
#define    B_PC1           1
#define    B_PC0           0

#define    B_DDC7          7
#define    B_DDC6          6
#define    B_DDC5          5
#define    B_DDC4          4
#define    B_DDC3          3
#define    B_DDC2          2
#define    B_DDC1          1
#define    B_DDC0          0

#define    B_PINC7         7
#define    B_PINC6         6
#define    B_PINC5         5
#define    B_PINC4         4
#define    B_PINC3         3
#define    B_PINC2         2
#define    B_PINC1         1
#define    B_PINC0         0

#define    B_PD7           7
#define    B_PD6           6
#define    B_PD5           5
#define    B_PD4           4
#define    B_PD3           3
#define    B_PD2           2
#define    B_PD1           1
#define    B_PD0           0

#define    B_DDD7          7
#define    B_DDD6          6
#define    B_DDD5          5
#define    B_DDD4          4
#define    B_DDD3          3
#define    B_DDD2          2
#define    B_DDD1          1
#define    B_DDD0          0

#define    B_PIND7         7
#define    B_PIND6         6
#define    B_PIND5         5
#define    B_PIND4         4
#define    B_PIND3         3
#define    B_PIND2         2
#define    B_PIND1         1
#define    B_PIND0         0

#define    B_RXC           7
#define    B_TXC           6
#define    B_UDRE          5
#define    B_FE            4
#define    B_OR            3

#define    B_SPIE          7
#define    B_SPE           6
#define    B_DORD          5
#define    B_MSTR          4
#define    B_CPOL          3
#define    B_CPHA          2
#define    B_SPR1          1
#define    B_SPR0          0

#define    B_SPIF          7
#define    B_WCOL          6

#define    B_RXCIE         7
#define    B_TXCIE         6
#define    B_UDRIE         5
#define    B_RXEN          4
#define    B_TXEN          3
#define    B_CHR9          2
#define    B_RXB8          1
#define    B_TXB8          0

#define    B_ACD           7
#define    B_ACO           5
#define    B_ACI           4
#define    B_ACIE          3
#define    B_ACIC          2
#define    B_ACIS1         1
#define    B_ACIS0         0

// Mask Definitions
#define    M_INT1          (1<<B_INT1)
#define    M_INT0          (1<<B_INT0)

#define    M_TOIE1         (1<<B_TOIE1)
#define    M_OCIE1A        (1<<B_OCIE1A)
#define    M_OCIE1B        (1<<B_OCIE1B)
#define    M_TICIE1        (1<<B_TICIE1)
#define    M_TOIE0         (1<<B_TOIE0)

#define    M_TOV1          (1<<B_TOV1)
#define    M_OCF1A         (1<<B_OCF1A)
#define    M_OCF1B         (1<<B_OCF1B)
#define    M_ICF1          (1<<B_ICF1)
#define    M_TOV0          (1<<B_TOV0)

#define    M_SRE           (1<<B_SRE)
#define    M_SRW           (1<<B_SRW)
#define    M_SE            (1<<B_SE)
#define    M_SM            (1<<B_SM)
#define    M_ISC11         (1<<B_ISC11)
#define    M_ISC10         (1<<B_ISC10)
#define    M_ISC01         (1<<B_ISC01)
#define    M_ISC00         (1<<B_ISC00)

#define    M_COM1A1        (1<<B_COM1A1)
#define    M_COM1A0        (1<<B_COM1A0)
#define    M_COM1B1        (1<<B_COM1B1)
#define    M_COM1B0        (1<<B_COM1B0)
#define    M_PWM11         (1<<B_PWM11)
#define    M_PWM10         (1<<B_PWM10)

#define    M_ICNC1         (1<<B_ICNC1)
#define    M_ICES1         (1<<B_ICES1)
#define    M_CTC1          (1<<B_CTC1)
#define    M_CS12          (1<<B_CS12)
#define    M_CS11          (1<<B_CS11)
#define    M_CS10          (1<<B_CS10)

#define    M_WDTOE         (1<<B_WDTOE)
#define    M_WDE           (1<<B_WDE)
#define    M_WDP2          (1<<B_WDP2)
#define    M_WDP1          (1<<B_WDP1)
#define    M_WDP0          (1<<B_WDP0)

#define    M_EEMWE         (1<<B_EEMWE)
#define    M_EEWE          (1<<B_EEWE)
#define    M_EERE          (1<<B_EERE)

#define    M_PA7           (1<<B_PA7)
#define    M_PA6           (1<<B_PA6)
#define    M_PA5           (1<<B_PA5)
#define    M_PA4           (1<<B_PA4)
#define    M_PA3           (1<<B_PA3)
#define    M_PA2           (1<<B_PA2)
#define    M_PA1           (1<<B_PA1)
#define    M_PA0           (1<<B_PA0)

#define    M_DDA7          (1<<B_DDA7)
#define    M_DDA6          (1<<B_DDA6)
#define    M_DDA5          (1<<B_DDA5)
#define    M_DDA4          (1<<B_DDA4)
#define    M_DDA3          (1<<B_DDA3)
#define    M_DDA2          (1<<B_DDA2)
#define    M_DDA1          (1<<B_DDA1)
#define    M_DDA0          (1<<B_DDA0)

#define    M_PINA7         (1<<B_PINA7)
#define    M_PINA6         (1<<B_PINA6)
#define    M_PINA5         (1<<B_PINA5)
#define    M_PINA4         (1<<B_PINA4)
#define    M_PINA3         (1<<B_PINA3)
#define    M_PINA2         (1<<B_PINA2)
#define    M_PINA1         (1<<B_PINA1)
#define    M_PINA0         (1<<B_PINA0)

#define    M_PB7           (1<<B_PB7)
#define    M_PB6           (1<<B_PB6)
#define    M_PB5           (1<<B_PB5)
#define    M_PB4           (1<<B_PB4)
#define    M_PB3           (1<<B_PB3)
#define    M_PB2           (1<<B_PB2)
#define    M_PB1           (1<<B_PB1)
#define    M_PB0           (1<<B_PB0)

#define    M_DDB7          (1<<B_DDB7)
#define    M_DDB6          (1<<B_DDB6)
#define    M_DDB5          (1<<B_DDB5)
#define    M_DDB4          (1<<B_DDB4)
#define    M_DDB3          (1<<B_DDB3)
#define    M_DDB2          (1<<B_DDB2)
#define    M_DDB1          (1<<B_DDB1)
#define    M_DDB0          (1<<B_DDB0)

#define    M_PINB7         (1<<B_PINB7)
#define    M_PINB6         (1<<B_PINB6)
#define    M_PINB5         (1<<B_PINB5)
#define    M_PINB4         (1<<B_PINB4)
#define    M_PINB3         (1<<B_PINB3)
#define    M_PINB2         (1<<B_PINB2)
#define    M_PINB1         (1<<B_PINB1)
#define    M_PINB0         (1<<B_PINB0)

#define    M_PC7           (1<<B_PC7)
#define    M_PC6           (1<<B_PC6)
#define    M_PC5           (1<<B_PC5)
#define    M_PC4           (1<<B_PC4)
#define    M_PC3           (1<<B_PC3)
#define    M_PC2           (1<<B_PC2)
#define    M_PC1           (1<<B_PC1)
#define    M_PC0           (1<<B_PC0)

#define    M_DDC7          (1<<B_DDC7)
#define    M_DDC6          (1<<B_DDC6)
#define    M_DDC5          (1<<B_DDC5)
#define    M_DDC4          (1<<B_DDC4)
#define    M_DDC3          (1<<B_DDC3)
#define    M_DDC2          (1<<B_DDC2)
#define    M_DDC1          (1<<B_DDC1)
#define    M_DDC0          (1<<B_DDC0)

#define    M_PINC7         (1<<B_PINC7)
#define    M_PINC6         (1<<B_PINC6)
#define    M_PINC5         (1<<B_PINC5)
#define    M_PINC4         (1<<B_PINC4)
#define    M_PINC3         (1<<B_PINC3)
#define    M_PINC2         (1<<B_PINC2)
#define    M_PINC1         (1<<B_PINC1)
#define    M_PINC0         (1<<B_PINC0)

#define    M_PD7           (1<<B_PD7)
#define    M_PD6           (1<<B_PD6)
#define    M_PD5           (1<<B_PD5)
#define    M_PD4           (1<<B_PD4)
#define    M_PD3           (1<<B_PD3)
#define    M_PD2           (1<<B_PD2)
#define    M_PD1           (1<<B_PD1)
#define    M_PD0           (1<<B_PD0)

#define    M_DDD7          (1<<B_DDD7)
#define    M_DDD6          (1<<B_DDD6)
#define    M_DDD5          (1<<B_DDD5)
#define    M_DDD4          (1<<B_DDD4)
#define    M_DDD3          (1<<B_DDD3)
#define    M_DDD2          (1<<B_DDD2)
#define    M_DDD1          (1<<B_DDD1)
#define    M_DDD0          (1<<B_DDD0)

#define    M_PIND7         (1<<B_PIND7)
#define    M_PIND6         (1<<B_PIND6)
#define    M_PIND5         (1<<B_PIND5)
#define    M_PIND4         (1<<B_PIND4)
#define    M_PIND3         (1<<B_PIND3)
#define    M_PIND2         (1<<B_PIND2)
#define    M_PIND1         (1<<B_PIND1)
#define    M_PIND0         (1<<B_PIND0)

#define    M_RXC           (1<<B_RXC)
#define    M_TXC           (1<<B_TXC)
#define    M_UDRE          (1<<B_UDRE)
#define    M_FE            (1<<B_FE)
#define    M_OR            (1<<B_OR)

#define    M_SPIE          (1<<B_SPIE)
#define    M_SPE           (1<<B_SPE)
#define    M_DORD          (1<<B_DORD)
#define    M_MSTR          (1<<B_MSTR)
#define    M_CPOL          (1<<B_CPOL)
#define    M_CPHA          (1<<B_CPHA)
#define    M_SPR1          (1<<B_SPR1)
#define    M_SPR0          (1<<B_SPR0)

#define    M_SPIF          (1<<B_SPIF)
#define    M_WCOL          (1<<B_WCOL)

#define    M_RXCIE         (1<<B_RXCIE)
#define    M_TXCIE         (1<<B_TXCIE)
#define    M_UDRIE         (1<<B_UDRIE)
#define    M_RXEN          (1<<B_RXEN)
#define    M_TXEN          (1<<B_TXEN)
#define    M_CHR9          (1<<B_CHR9)
#define    M_RXB8          (1<<B_RXB8)
#define    M_TXB8          (1<<B_TXB8)

#define    M_ACD           (1<<B_ACD)
#define    M_ACO           (1<<B_ACO)
#define    M_ACI           (1<<B_ACI)
#define    M_ACIE          (1<<B_ACIE)
#define    M_ACIC          (1<<B_ACIC)
#define    M_ACIS1         (1<<B_ACIS1)
#define    M_ACIS0         (1<<B_ACIS0)

////////////////////////////////////////////////////////////////////////////////

#define    RAMEND       0x25F    // Last On-Chip SRAM Location

#define    INT0addr      0x001   // External Interrupt0 Vector Address
#define    INT1addr      0x002   // External Interrupt1 Vector Address
#define    ICP1addr      0x003   // Input Capture1 Interrupt Vector Address
#define    OC1Aaddr      0x004   // Output Compare1A Interrupt Vector Address
#define    OC1Baddr      0x005   // Output Compare1B Interrupt Vector Address
#define    OVF1addr      0x006   // Overflow1 Interrupt Vector Address
#define    OVF0addr      0x007   // Overflow0 Interrupt Vector Address
#define    SPIaddr       0x008   // SPI Interrupt Vector Address
#define    URXCaddr      0x009   // UART Receive Complete Interrupt Vector Address
#define    UDREaddr      0x00a   // UART Data Register Empty Interrupt Vector Address
#define    UTXCaddr      0x00b   // UART Transmit Complete Interrupt Vector Address
#define    ACIaddr       0x00c   // Analog Comparator Interrupt Vector Address

////////////////////////////////////////////////////////////////////////////////

#endif

