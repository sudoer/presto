
#ifndef _HC11REGS_H_
#define _HC11REGS_H_

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

// INTERRUPTS

#define INTR_SCI     0
#define INTR_SPI     1
#define INTR_PAIE    2
#define INTR_PAO     3
#define INTR_TOF     4
#define INTR_TOC5    5
#define INTR_TOC4    6
#define INTR_TOC3    7
#define INTR_TOC2    8
#define INTR_TOC1    9
#define INTR_TIC3   10
#define INTR_TIC2   11
#define INTR_TIC1   12
#define INTR_RTI    13
#define INTR_IRQ    14
#define INTR_XIRQ   15
#define INTR_SWI    16
#define INTR_ILLOP  17
#define INTR_COP    18
#define INTR_CLM    19
#define INTR_RESET  20

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
