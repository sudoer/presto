
#ifndef _INTVECT_H_
#define _INTVECT_H_

////////////////////////////////////////////////////////////////////////////////

#include "types.h"

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
#define NUM_INTERRUPTS 21

////////////////////////////////////////////////////////////////////////////////

extern void set_interrupt(BYTE intr, void (*vector)(void));
extern void init_interrupts(void);

////////////////////////////////////////////////////////////////////////////////

#endif


