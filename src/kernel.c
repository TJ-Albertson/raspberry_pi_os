#include "printf.h"
#include "utils.h"
#include "mini_uart.h"

void kernel_main(void)
{
	uart_init();
	init_printf(0, put_character);

	int exception_level = get_el();

	printf("Exception level: %d \r\n", exception_level);

	while (1) {
		uart_send_character(uart_recv());
	}
}
