/*
 * adc.c
 *
 * Created: 03/11/2016 14:54:09
 *  Author: kvn
 */ 

 #include <avr/io.h>

void InitADC()
{
	ADMUX=(1<<REFS0); // For Aref=AVcc;
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Rescale div factor = 128
}

uint16_t ReadADC(uint8_t ch)
{
	// Select ADC Channel ch must be 0-7
	ch=ch&0x07;
	ADMUX|=ch;

	// Start Single conversion
	ADCSRA|=(1<<ADSC);

	// Wait for conversion to complete
	while(!(ADCSRA & (1<<ADIF)));

	// Clear ADIF by writing one to it
	ADCSRA|=(1<<ADIF);

	// Returning the adc
	return(ADC);
}