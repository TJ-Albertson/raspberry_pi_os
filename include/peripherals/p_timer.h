#ifndef	_P_TIMER_H
#define	_P_TIMER_H

#include "peripherals/p_base.h"

// System Timer Control/Status
#define TIMER_CS        (PBASE+0x00003000) 
// System Timer Counter Lower 32 bits
#define TIMER_CLO       (PBASE+0x00003004)
// System Timer Counter Higher 32 bits
#define TIMER_CHI       (PBASE+0x00003008)
// System Timer Compare 0
#define TIMER_C0        (PBASE+0x0000300C)
// System Timer Compare 1
#define TIMER_C1        (PBASE+0x00003010)
// System Timer Compare 2
#define TIMER_C2        (PBASE+0x00003014)
// System Timer Compare 3
#define TIMER_C3        (PBASE+0x00003018)

// System Timer Match 0
#define TIMER_CS_M0	    (1 << 0)
// System Timer Match 1
#define TIMER_CS_M1	    (1 << 1)
// System Timer Match 2
#define TIMER_CS_M2     (1 << 2)
// System Timer Match 3
#define TIMER_CS_M3	    (1 << 3)

#endif  /*_P_TIMER_H */
