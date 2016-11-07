#include <avr/io.h>
#include <stdio.h>

#define BAUD_RATE 4800	// Seriel hastighed
#define F_CPU 16000000UL

void uart_flush(void)
{
  unsigned char dummy;  while (UCSR0A & (1<<RXC0)) dummy = UDR0;
}


void uart_init(void)
{
  UBRR0H = (((F_CPU/BAUD_RATE)/16)-1)>>8;	// set baud rate
  UBRR0L = (((F_CPU/BAUD_RATE)/16)-1);
  UCSR0B = (1<<RXEN0)|(1<<TXEN0); 		// enable Rx & Tx
  UCSR0C=  (1<<UCSZ01)|(1<<UCSZ00);  	       // config USART; 8N1
  
  uart_flush();
} 

int uart_putch(char ch,FILE *stream)
{
    if (ch == '\n')
    	uart_putch('\r', stream); 

	while (!(UCSR0A & (1<<UDRE0)));
    UDR0=ch;   

	return 0;
}

int uart_getch(FILE *stream)
{
   unsigned char ch;   
   while (!(UCSR0A & (1<<RXC0)));
   ch=UDR0;  
   uart_putch(ch,stream);       

   return ch;
}

