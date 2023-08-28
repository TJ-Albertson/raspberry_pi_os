#include "printf.h"
#include "timer.h"
#include "irq.h"
#include "mini_uart.h"

#include "hvs.h"
#include "framebuffer.h"

#include "v3d.h"

#include <stdint.h>

void kernel_main(void)
{
	uart_init();
	init_printf(0, put_character);

    printf("\n|--------------------------------------------------------------|\n");
	printf("Kernel Version: %s\n", CURRENT_TIME);


	

	irq_vector_init();
	timer_init();
	enable_interrupt_controller();
	enable_irq();

	
	int el = get_el();
	printf("Exception level: %d \r\n", el);
	

	fb_init();

	drawRect(150,150,400,400,0x03,0);
    drawRect(300,300,350,350,0x2e,1);

    drawCircle(960,540,250,0x0e,0);
    drawCircle(960,540,50,0x13,1);

    drawPixel(250,250,0x0e);

    drawChar('O',500,500,0x05);
    drawString(100,100,"Hello world!",0x0f);

    drawLine(100,500,350,700,0x0c);


	delay(9000000);


	uint16_t* const fb_one = (uint16_t*)(0x10000000);

	const uint16_t screen_width = 1920, screen_height = 1080;
    const uint16_t fb_width = screen_width / 2, fb_height = screen_height / 2;
    const uint16_t fb_center_x = fb_width / 2;
    const uint16_t fb_center_y = fb_height / 2;

	hvs_plane plane = {
        .format = HVS_PIXEL_FORMAT_RGB565,
        .pixel_order = HVS_PIXEL_ORDER_ARGB,
        .start_x = (screen_width - fb_width) / 2,
        .start_y = (screen_height - fb_height) / 2,
        .height = fb_height,
        .width = fb_width,
        .pitch = fb_width * sizeof(uint16_t),
        .framebuffer = fb_one
    };
    printf("Writing initial display list.r\n\n");
    write_display_list(&plane, 1);


    
    printf("before 2nd Delay\n\n");
    delay(9000000);
    
    
    
    printf("Before V3D test\n\n");
    v3d_test();

    uint16_t* const fb_two     = (uint16_t*)(0x10100000);

    hvs_plane plane2 = {
        .format = HVS_PIXEL_FORMAT_RGB565,
        .pixel_order = HVS_PIXEL_ORDER_ARGB,
        .start_x = (screen_width - fb_width) / 2,
        .start_y = (screen_height - fb_height) / 2,
        .height = fb_height,
        .width = fb_width,
        .pitch = fb_width * sizeof(uint16_t),
        .framebuffer = fb_two
    };
    printf("Writing gpu display list.r\n");
    write_display_list(&plane2, 1);
    

	while (1){
		//uart_send_character(uart_recv());
	}	
}