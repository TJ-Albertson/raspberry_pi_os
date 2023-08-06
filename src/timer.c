#include "utils.h"
#include "printf.h"
#include "peripherals/p_timer.h"

const unsigned int interval = 200000;
unsigned int curVal = 0;

void timer_init(void)
{
	curVal = get32(TIMER_CLO);	// Get current timer count
	curVal += interval;         // Increase by interval
	put32(TIMER_C1, curVal);	// Put new value in TIMER_C1, when TIMER_CLO == curVal then TIMERC_C1 will generate an interrupt. System Timer Match 1
}


void handle_timer_irq(void) 
{	
	// Prepare next interupt
	curVal += interval;
	put32(TIMER_C1, curVal);

	// Awknowled the interrupt happened so the next one can go through
	put32(TIMER_CS, TIMER_CS_M1);
	printf("Timer interrupt received\n\r");
}
