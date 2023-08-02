#ifndef _P_UART_PL011_H
#define _P_UART_PL011_H

void uart_init ( void );
char uart_recv ( void );
void uart_send_character ( char c );
void uart_send_string(char* str);
void put_character ( void* p, char c );

#endif  /*_MINI_UART_H */
