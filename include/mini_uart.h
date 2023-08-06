#ifndef _P_UART_PL011_H
#define _P_UART_PL011_H

// Initialize Mini UART on pins 14(Transmt) and 15(Receive)
void uart_init ( void );
// Get char on Mini UART
char uart_recv ( void );
// Send char on Mini UART
void uart_send_character ( char c );
// Send String on Mini UART
void uart_send_string(char* str);
// Function defined for printf library
void put_character(void* p, char c);

#endif  /*_MINI_UART_H */
