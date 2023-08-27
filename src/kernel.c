#include "printf.h"
#include "timer.h"
#include "irq.h"
#include "mini_uart.h"

#include "framebuffer.h"

void kernel_main(void)
{
	uart_init();
	init_printf(0, put_character);


	printf("Start\n\n");

	//rpi4_osdev_uart_init();
	/*
	

	irq_vector_init();
	timer_init();
	enable_interrupt_controller();
	enable_irq();

	
	int el = get_el();
	printf("Exception level: %d \r\n", el);
	*/

	fb_init();

	drawRect(150,150,400,400,0x03,0);
    drawRect(300,300,350,350,0x2e,1);

    drawCircle(960,540,250,0x0e,0);
    drawCircle(960,540,50,0x13,1);

    drawPixel(250,250,0x0e);

    drawChar('O',500,500,0x05);
    drawString(100,100,"Hello world!",0x0f);

    drawLine(100,500,350,700,0x0c);

	while (1){
		//uart_send_character(uart_recv());
	}	
}