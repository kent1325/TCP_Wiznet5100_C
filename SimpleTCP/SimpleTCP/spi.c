/*
 * spi.c
 * Denne er specielt lavet til Wiznet 5100\
 * og håndterer alt SPI kommunikation til og fra kortet...
 */ 

#include"spi.h"
#include "Wiznet5100.h"

void spiInit(void)
{
	// MOSI SS and CLK pins set to output (PORTB4, MISO is default 0 = input)
	SPI_DDR = (1<<PORTB3)|(1<<PORTB5)|(1<<PORTB2);  
	// CS pin - low = active 
	SPI_PORT |= (1<<SPI_CS);  
	// Enable SPI, Master Mode 0, set clock rate to fck/2
	SPCR = (1<<SPE)|(1<<MSTR);
	SPSR |= (1<<SPI2X);  
}

//skriver 8 bit via spi til addr
void SPI_Write8(unsigned int addr,unsigned char data)
{
  // Activate the CS pin by setting it to low
  SPI_PORT &= ~(1<<SPI_CS); 

  // Start Wiznet W5100 Write OpCode transmission
  SPDR = WIZNET_WRITE_OPCODE;  

  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));  

  // Start Wiznet W5100 Address High Bytes transmission
  SPDR = (addr & 0xFF00) >> 8;  

  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));  

  // Start Wiznet W5100 Address Low Bytes transmission
  SPDR = addr & 0x00FF;  

  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));   

  // Start Data transmission
  SPDR = data;  

  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF))); 
   
  // CS pin is not activated by setting it to high
  SPI_PORT |= (1<<SPI_CS);
}

//Læser 8 bit via spi fra addr.
unsigned char SPI_Read8(unsigned int addr)
{
  // Activate the CS pin by setting it to low
  SPI_PORT &= ~(1<<SPI_CS);  

  // Start Wiznet W5100 Read OpCode transmission
  SPDR = WIZNET_READ_OPCODE;  

  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));  

  // Start Wiznet W5100 Address High Bytes transmission
  SPDR = (addr & 0xFF00) >> 8;  

  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));  

  // Start Wiznet W5100 Address Low Bytes transmission
  SPDR = addr & 0x00FF;  

  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));   

  // Send Dummy transmission for reading the data
  SPDR = 0x00;  

  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));  

  // CS pin is not activated by setting it to high
  SPI_PORT |= (1<<SPI_CS);  

  // RX reads the received data from the SPI data register and returns it
  return(SPDR);
}

//Skriver 16 bit til spi på addr og addr + 1
void SPI_Write16(unsigned int addr,uint16_t data)
{
	
	SPI_Write8(addr, (data & 0xFF00) >> 8);
	SPI_Write8(addr + 1, (data & 0xFF));
}

//læser 16 bit fra addr og addr + 1
uint16_t SPI_Read16(unsigned int addr)
{
	
	uint16_t retval = 0;

	//retval = (SPI_Read8(addr) << 8) | (0xFF &  SPI_Read8(addr +1));
	retval = SPI_Read8(addr);
	retval = (((retval & 0x00ff)<<8) + SPI_Read8(addr + 1));
	
	return retval;
}
