#ifndef	_TIMER_H
#define	_TIMER_H

/*
System Timer (BCM2837-ARM-Peripherals page 172)

The system timer is a 64 bit counter (CLO + CHI). There are four 32 bit compare registers (C0 to C3).
If the value in one of the compare registers matches the lower 32 bits of the counter (CLO). Then it generates an interrupt.

You can only use two of the compare registers, C1 and C3, to generate interrupts on the CPU because C0 and C2 are used by GPU.

C1 corresponds to entry 1. system timer match 1
C3 corresponds to entry 3. system timer match 3

You have to enable IRQ's from C1 and/or C3. I am only using C1 so I enable it and call handle_timer_irq to handle it in irq.c

*/

void timer_init ( void );
void handle_timer_irq ( void );

#endif  /*_TIMER_H */
