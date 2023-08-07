#include "printf.h"
#include "timer.h"
#include "irq.h"
#include "mini_uart.h"


void kernel_main(void)
{

	


	uart_init();
	init_printf(0, put_character);

	int el = get_el();
	printf("Exception level: %d \r\n", el);

	irq_vector_init();
	timer_init();
	enable_interrupt_controller();
	enable_irq();

	el = get_el();
	printf("Exception level: %d \r\n", el);

	while (1){
		uart_send_character(uart_recv());
	}	
}