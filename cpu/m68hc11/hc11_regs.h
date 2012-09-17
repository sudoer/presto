
#ifndef _HC11REGS_H_
#define _HC11REGS_H_

////////////////////////////////////////////////////////////////////////////////

// this stuff was stolen from the ImageCraft compiler libraries

// base address of register block, change this if you relocate the register
// block. This is from an A8. May need to be changed for other HC11 members
// or if you relocate the IO base address.

#define IO_BASE  0x1000
#define PORTA  *(volatile unsigned char *)(IO_BASE + 0x00)
#define PIOC   *(volatile unsigned char *)(IO_BASE + 0x02)
#define PORTC  *(volatile unsigned char *)(IO_BASE + 0x03)
#define PORTB  *(volatile unsigned char *)(IO_BASE + 0x04)
#define PORTCL *(volatile unsigned char *)(IO_BASE + 0x05)
#define DDRC   *(volatile unsigned char *)(IO_BASE + 0x07)
#define PORTD  *(volatile unsigned char *)(IO_BASE + 0x08)
#define DDRD   *(volatile unsigned char *)(IO_BASE + 0x09)
#define PORTE  *(volatile unsigned char *)(IO_BASE + 0x0A)
#define CFORC  *(volatile unsigned char *)(IO_BASE + 0x0B)
#define OC1M   *(volatile unsigned char *)(IO_BASE + 0x0C)
#define OC1D   *(volatile unsigned char *)(IO_BASE + 0x0D)
#define TCNT   *(volatile unsigned short *)(IO_BASE + 0x0E)
#define TIC1   *(volatile unsigned short *)(IO_BASE + 0x10)
#define TIC2   *(volatile unsigned short *)(IO_BASE + 0x12)
#define TIC3   *(volatile unsigned short *)(IO_BASE + 0x14)
#define TOC1   *(volatile unsigned short *)(IO_BASE + 0x16)
#define TOC2   *(volatile unsigned short *)(IO_BASE + 0x18)
#define TOC3   *(volatile unsigned short *)(IO_BASE + 0x1A)
#define TOC4   *(volatile unsigned short *)(IO_BASE + 0x1C)
#define TOC5   *(volatile unsigned short *)(IO_BASE + 0x1E)
#define TCTL1  *(volatile unsigned char *)(IO_BASE + 0x20)
#define TCTL2  *(volatile unsigned char *)(IO_BASE + 0x21)
#define TMSK1  *(volatile unsigned char *)(IO_BASE + 0x22)
#define TFLG1  *(volatile unsigned char *)(IO_BASE + 0x23)
#define TMSK2  *(volatile unsigned char *)(IO_BASE + 0x24)
#define TFLG2  *(volatile unsigned char *)(IO_BASE + 0x25)
#define PACTL  *(volatile unsigned char *)(IO_BASE + 0x26)
#define PACNT  *(volatile unsigned char *)(IO_BASE + 0x27)
#define SPCR   *(volatile unsigned char *)(IO_BASE + 0x28)
#define SPSR   *(volatile unsigned char *)(IO_BASE + 0x29)
#define SPDR   *(volatile unsigned char *)(IO_BASE + 0x2A)
#define BAUD   *(volatile unsigned char *)(IO_BASE + 0x2B)
#define SCCR1  *(volatile unsigned char *)(IO_BASE + 0x2C)
#define SCCR2  *(volatile unsigned char *)(IO_BASE + 0x2D)
#define SCSR   *(volatile unsigned char *)(IO_BASE + 0x2E)
#define SCDR   *(volatile unsigned char *)(IO_BASE + 0x2F)
#define ADCTL  *(volatile unsigned char *)(IO_BASE + 0x30)
#define ADR1   *(volatile unsigned char *)(IO_BASE + 0x31)
#define ADR2   *(volatile unsigned char *)(IO_BASE + 0x32)
#define ADR3   *(volatile unsigned char *)(IO_BASE + 0x33)
#define ADR4   *(volatile unsigned char *)(IO_BASE + 0x34)
#define OPTION *(volatile unsigned char *)(IO_BASE + 0x39)
#define COPRST *(volatile unsigned char *)(IO_BASE + 0x3A)
#define PPROG  *(volatile unsigned char *)(IO_BASE + 0x3B)
#define HPRIO  *(volatile unsigned char *)(IO_BASE + 0x3C)
#define INIT   *(volatile unsigned char *)(IO_BASE + 0x3D)
#define TEST1  *(volatile unsigned char *)(IO_BASE + 0x3E)
#define CONFIG *(volatile unsigned char *)(IO_BASE + 0x3F)

////////////////////////////////////////////////////////////////////////////////

// SPECIAL FUNCTION REGISTERS

#define OC1M_OC1M7  0x80
#define OC1M_OC1M6  0x40
#define OC1M_OC1M5  0x20
#define OC1M_OC1M4  0x10
#define OC1M_OC1M3  0x08

#define OC1D_OC1D7  0x80
#define OC1D_OC1D6  0x40
#define OC1D_OC1D5  0x20
#define OC1D_OC1D4  0x10
#define OC1D_OC1D3  0x08

#define TCTL1_OM2   0x80
#define TCTL1_OL2   0x40
#define TCTL1_OM3   0x20
#define TCTL1_OL3   0x10
#define TCTL1_OM4   0x08
#define TCTL1_OL4   0x04
#define TCTL1_OM5   0x02
#define TCTL1_OL5   0x01

#define TCTL2_EDG1B 0x20
#define TCTL2_EDG1A 0x10
#define TCTL2_EDG2B 0x08
#define TCTL2_EDG2A 0x04
#define TCTL2_EDG3B 0x02
#define TCTL2_EDG3A 0x01

#define TMSK1_OC1I  0x80
#define TMSK1_OC2I  0x40
#define TMSK1_OC3I  0x20
#define TMSK1_OC4I  0x10
#define TMSK1_OC5I  0x08
#define TMSK1_IC1I  0x04
#define TMSK1_IC2I  0x02
#define TMSK1_IC3I  0x01

#define TFLG1_OC1F  0x80
#define TFLG1_OC2F  0x40
#define TFLG1_OC3F  0x20
#define TFLG1_OC4F  0x10
#define TFLG1_OC5F  0x08
#define TFLG1_IC1F  0x04
#define TFLG1_IC2F  0x02
#define TFLG1_IC3F  0x01

#define SCCR1_R8    0x80
#define SCCR1_T8    0x40
#define SCCR1_M     0x10
#define SCCR1_WAKE  0x04

#define SCSR_TDRE   0x80
#define SCSR_TC     0x40
#define SCSR_RDRF   0x20
#define SCSR_IDLE   0x10
#define SCSR_OR     0x08
#define SCSR_NF     0x04
#define SCSR_FE     0x02

#define SCCR2_TIE   0x80
#define SCCR2_TCIE  0x40
#define SCCR2_RIE   0x20
#define SCCR2_ILIE  0x10
#define SCCR2_TE    0x08
#define SCCR2_RE    0x04
#define SCCR2_RWU   0x02
#define SCCR2_SBK   0x01

#define BAUD_TCLR   0x80
#define BAUD_SCP1   0x20
#define BAUD_SCP0   0x10
#define BAUD_RCKB   0x08
#define BAUD_SCR2   0x04
#define BAUD_SCR1   0x02
#define BAUD_SCR0   0x01

#define OPTION_ADPU 0x80
#define OPTION_CSEL 0x40
#define OPTION_IRQE 0x20
#define OPTION_DLY  0x10
#define OPTION_CME  0x08
#define OPTION_CR1  0x02
#define OPTION_CR0  0x01

#define ADCTL_CCF   0x80
#define ADCTL_SCAN  0x20
#define ADCTL_MULT  0x10
#define ADCTL_CD    0x08
#define ADCTL_CC    0x04
#define ADCTL_CB    0x02
#define ADCTL_CA    0x01

////////////////////////////////////////////////////////////////////////////////

// STATUS REGISTER BITS

#define CCR_STOP_DISABLE   0x80
#define CCR_X_INTERRUPT    0x40
#define CCR_HALF_CARRY     0x20
#define CCR_INTERRUPT      0x10
#define CCR_NEGATIVE       0x08
#define CCR_ZERO           0x04
#define CCR_OVERFLOW       0x02
#define CCR_CARRY          0x01

////////////////////////////////////////////////////////////////////////////////

#endif

