/*
* SimpleTCP.c
*
* Created: 31/10/2016 13:01:04
* Author : Kent Vugs Nielsen
*/

// Includes
#include <avr/io.h>
#include <avr/delay.h>
#include <string.h>
//#include <stdlib.h>
#include "uart.h"
#include "spi.h"
#include "Wiznet5100.h"
//#include "adc.h"

//Declaration
uint8_t strCmp(char *source, char *cmpStr);

// Global Variables
uint8_t buffer[MAX_BUF];
int preDefTemp = 55;
int temperature = 55;
int preDefWaterLvl = 10;
int waterLevel = 10;

int main(void)
{
	// Initializing the serial connection
	uart_init();
	stdout = &uart_str;
	
	//adc_init();

	_delay_ms(1000);

	//initializing the Wiznet5100 module and the SPI
	printf("Initializing Wiznet and SPI...\n");
	spiInit();
	_delay_ms(1000);
	WizInit();
	printf("Initializing done!\n");

	// Main local variables
	DDRD = 0xEC; // Sets PORTD7, 6, 5, 3, 2 as output - digital
	//DDRB = 0x03; // Sets PORTB8 and 9 as output - digital
	DDRC = 0xFF; // Sets all PORTC as output - analog

	PORTD = 0xEC;
	//PORTB = 0x03;
	PORTC = 0xFF;
	//double temp, calc;

	uint8_t socketState = 0;
	uint8_t recDataSize = 0;
	uint8_t dataSize = 0;

	while (1)
	{
		// Gets the current socket state and saves it ind the variable socketState
		socketState = getSocketState();

		// If socket state is closed - open it
		if (socketState == SOCK_CLOSED)
		{
			// Opens socket as TCP port 11
			openSocket(MR_TCP, 11);
			printf("Socketstate = OPEN\n");
		}

		// If socket is initialized, then open for the connection
		if (socketState == SOCK_INIT)
		{
			// Starting a listen so it's possible to connect to the socket
			listen();
			printf("Socketstate = LISTEN\n");
		}

		// If the socket state is established recieve and send data
		if (socketState == SOCK_ESTABLISHED)
		{
			// Get the size of the received data
			recDataSize = getAvailableData();

			// if data status is approved
			if (UCSR0A & (1<<RXC0))
			{
				// Get temperature and water level data from sink
				//temperature = uart_getch(NULL);
				//waterLevel = uart_getch(NULL);
				printf("I'm in, brouhaha!\r\n");

				//temp = read_adc(0);
				//calc = (temp * 5 * 100) / 1024;
				//printf("%d\n", calc);

				send("You are in my system!\r\n", 23);
				//send(temperature, sizeof(int));
				//send(waterLevel, sizeof(int));
			}

			// Process the data
			if (recDataSize > 0) // If there is data
			{
				// Make sure there is data to process
				if (recieve(&buffer[dataSize], recDataSize) <= 0)
				{
					printf("Could not find any data!\n");
					break; // Error, there was no data
				}

				// Print the received data
				printf("Bytes received = %d\n", recDataSize);

				// Now we got data to process
				dataSize += recDataSize;

				// Send the buffer and received data size
				if (buffer[dataSize -1] == '\n')
				{

					dataSize = 0;

					// If received input is equal to <ENTER> send a welcome
					if (buffer[0] == '\n' || buffer[0] == '\r')
					{
						// Sending welcome
						send("Welcome!\r\nYou are connected to me, Arduino and wish to do something.\r\nYour wish is my command, sir!\r\n", 101);
						printf("Welcome is send!\n");
					}

					// If received input equals to quit or exit. send disconnect
					else if (strCmp(buffer, "quit") != 0 || strCmp(buffer, "exit") != 0)
					{
						// Sending disconnect
						send("Have a marvelous day, sir!\r\nDisconnecting...", 44);
						printf("Disconnect is send!\n");
						disconnect();
					}

					// Else if start is received open ports!
					else if (strCmp(buffer, "start") != 0)
					{
						// Turn of the system

						/*DDRD = 0xEC; // Sets PORTD7, 6, 5, 3, 2 as output - digital
						//DDRB = 0x03; // Sets PORTB8 and 9 as output - digital
						DDRC = 0xFF; // Sets all PORTC as output - analog

						PORTD = 0xEC;
						//PORTB = 0x03;
						PORTC = 0xFF;*/

						send("Turned the system ON!\r\n", 23);
						printf("Start is send!\n");
					}

					// Else if stop is received close ports!
					else if (strCmp(buffer, "stop") != 0)
					{
						// Turn of the system

						//PORTC = ~PORTC;
						//PORTD = ~PORTD;
						//PORTB = ~PORTB; 

						send("Turned the system OFF!\r\n", 24);
						printf("Stop is send!\n");
					}

					// Else just send the data
					else
					{
						printf("Returning the data!\r\n");
						send(buffer, dataSize);
						_delay_ms(100);
					}

					// Reset the size of data
					dataSize = 0;
				}

				else if (buffer[0] == 255)
				{
					printf("Received trashing command!\nTrashing %d bytes data\n", dataSize);
				}
			}
		}
		if (socketState == SOCK_CLOSING)
		{
			// free used resources
			printf("Socket Closing...");
		}

		if (socketState == SOCK_CLOSE_WAIT)
		{
			// Client waits for the connection to close... Close it
			disconnect();
		}

		if (socketState == SOCK_LAST_ACK)
		{
			// Client has closed the connection. So lets do that to!
			close();
		}
		
	}

	return 0;
}

uint8_t strCmp(char *source, char *cmpStr)
{
	for(int i = 0; cmpStr[i] != '\0'; i++)
	{
		if(source[i] != cmpStr[i])
		return 0;
	}
	return 1;
}