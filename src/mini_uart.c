#include "utils.h"
#include "peripherals/p_mini_uart.h"
#include "peripherals/p_gpio.h"


/*
* (BCM2837-ARM-Peripherals page 8)
*
* AUX_MU_LSR_REG = Mini Uart Line Status = 8 bits
* 	Bit 0 = Data ready
* 	Bit 5 = Transmitter empty 
*
* AUX_MU_IO_REG  = Mini Uart I/O Data    = 8 bits
* 	The AUX_MU_IO_REG register is primary used to write data to and read data from the UART FIFOs.
*/
void uart_send_character(char c)
{
	while(1) {
		if(get32(AUX_MU_LSR_REG) & 0x20) // Check Bit 5, Transmitter empty
			break;
	}
	put32(AUX_MU_IO_REG, c);
}

char uart_recv(void)
{
	while(1) {
		if(get32(AUX_MU_LSR_REG) & 0x01) // Check Bit 0, Data ready
			break;
	}
	return(get32(AUX_MU_IO_REG) & 0xFF); // & 0xFF is to make sure other bits of the 32 are zero
}

void uart_send_string(char* str)
{
	for (int i = 0; str[i] != '\0'; i ++) {
		uart_send_character((char)str[i]);
	}
}


// Prepare mini_uart output on GPIO pins 14 and 15
void uart_init(void)
{
	unsigned int selector;
	/*
	* Each GPIO on BCM2837 has 6 alternative functions (0-5).
	* ALT5 on GPIO14 is Mini Uart Transmit.
	* ALT5 on GPIO15 is Mini Uart Recieve.
	* GFSEL1 register is for GPIO pins 10-19.
    * (page 92 of BCM2837 ARM Peripherals manual)
	*/
	selector = get32(GPFSEL1);

	selector &= ~(7<<12);	// clean gpio14 (zero out bits)
	selector &= ~(7<<15);	// clean gpio15 (zero out bits)
	// selector = xx xxx xxx xxx xxx 000 000 xxx xxx xxx xxx

	selector |= 2<<12;		// set alt5 for gpio14
	selector |= 2<<15;		// set alt5 for gpio15
	// selector = xx xxx xxx xxx xxx 010 010 xxx xxx xxx xxx

	put32(GPFSEL1,selector);

	/*
    * GPIO pull-up/down (page 101 of BCM2837 ARM Peripherals manual)
	*
 	* The GPIO Pull-up/down Clock Registers control the actuation of internal pull-downs on
 	* the respective GPIO pins. These registers must be used in conjunction with the GPPUD
 	* register to effect GPIO Pull-up/down changes. The following sequence of events is required:
 	*/
 	// 1. Write to GPPUD to set the required control signal (i.e. Pull-up or Pull-Down or neither to remove the current Pull-up/down)
 	put32(GPPUD, 0); // 0 = neither

	// 2. Wait 150 cycles – this provides the required set-up time for the control signal
	delay(150);

 	// 3. Write to GPPUDCLK0/1 to clock the control signal into the GPIO pads you wish to
 	//    modify – NOTE only the pads which receive a clock will be modified, all others will
 	//    retain their previous state.
	put32(GPPUDCLK0, (1<<14)|(1<<15));
 	
	// 4. Wait 150 cycles – this provides the required hold time for the control signal
 	delay(150);

	// 5. Write to GPPUD to remove the control signal
	put32(GPPUD, 0);

	// 6. Write to GPPUDCLK0/1 to remove the clocks
	put32(GPPUDCLK0, 0);
	// Done. The last two steps are so that you don't affect any future alterations of GPIO.
	

	// Register settings
	put32(AUX_ENABLES ,1);                   // Enable mini uart (this also enables access to its registers)
	put32(AUX_MU_CNTL_REG, 0);               // Disable auto flow control and disable receiver and transmitter (for now)
	put32(AUX_MU_IER_REG, 0);                // Disable receive and transmit interrupts
	put32(AUX_MU_LCR_REG, 3);                // Enable 8 bit mode
	put32(AUX_MU_MCR_REG, 0);                // Set RTS line to be always high
	put32(AUX_MU_BAUD_REG, 270);             // Set baud rate to 115200

	put32(AUX_MU_CNTL_REG, 3);               // Enable transmitter and receiver
}

// This function is required by printf function
void put_character(void* pointer, char character)
{
	uart_send_character(character);
}