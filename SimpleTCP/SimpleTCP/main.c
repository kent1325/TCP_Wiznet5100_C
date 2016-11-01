/*
 * SimpleTCP.c
 *
 * Created: 31/10/2016 13:01:04
 * Author : kvn
 */ 

#include <avr/io.h>
//#include <avr/delay.h>
#include <stdio.h>
#include "uart.h"
#include "spi.h"
#include "Wiznet5100.h"

int main(void)
{
    uart_init();// Initierer serial forbindelsen
    stdout = &uart_str;
    
	//initier Wiznet5100 modulet med grundlæggende opsætning samt spi
	printf("Initializing Wiznet and SPI...\n");
    spiInit();
    WizInit();
    printf("Initializing done!\n");

    while (1) 
    {
    }

	return 0;
}