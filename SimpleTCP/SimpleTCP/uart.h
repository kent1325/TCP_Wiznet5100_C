// Stream setup til printf.
#include <avr/io.h>
#include <stdio.h>

extern void uart_init(void);
extern void uart_flush(void);
extern int uart_putch(char ch,FILE *stream);
extern int uart_getch(FILE *stream);

FILE uart_str = FDEV_SETUP_STREAM(uart_putch, uart_getch, _FDEV_SETUP_RW);





