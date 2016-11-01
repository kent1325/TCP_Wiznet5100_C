#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>

// spi port defineringer
#define SPI_PORT PORTB
#define SPI_DDR  DDRB
#define SPI_CS   PORTB2

extern void spiInit(void);
extern void SPI_Write8(unsigned int addr,unsigned char data);
extern unsigned char SPI_Read8(unsigned int addr);
extern void SPI_Write16(unsigned int addr,uint16_t data);
extern uint16_t SPI_Read16(unsigned int addr);

#endif /* SPI_H_ */