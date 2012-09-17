
#ifndef _CPU_DEBUG_H_
#define _CPU_DEBUG_H_

////////////////////////////////////////////////////////////////////////////////

#include "registers.h"
#include "handyboard.h"
#include "types.h"

////////////////////////////////////////////////////////////////////////////////
//   I N D I C A T I O N S / F E E D B A C K
////////////////////////////////////////////////////////////////////////////////

#define CPU_DEBUG_SWI_START()    BITSET(PORTD,PIN_SWI);
#define CPU_DEBUG_SWI_END()      BITCLR(PORTD,PIN_SWI);

////////////////////////////////////////////////////////////////////////////////

#define CPU_DEBUG_TASK_SWITCH()  TOGGLE_SPEAKER();

////////////////////////////////////////////////////////////////////////////////

#define CPU_DEBUG_TICK_START()   BITSET(PORTD,PIN_TIMER);
#define CPU_DEBUG_TICK_END()     BITCLR(PORTD,PIN_TIMER);

////////////////////////////////////////////////////////////////////////////////

#endif // _CPU_DEBUG_H_


