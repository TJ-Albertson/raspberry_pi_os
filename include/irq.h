#ifndef	_IRQ_H
#define	_IRQ_H

// Un-masks interrupt from System Timer 1
void enable_interrupt_controller( void );

// Loads Vector Base Address Register for EL1 (VBAR_EL1) with Vector Table address
void irq_vector_init( void );
// Un-mask IRQ bit
void enable_irq( void );
// Mask IRQ bit
void disable_irq( void );

#endif  /*_IRQ_H */
