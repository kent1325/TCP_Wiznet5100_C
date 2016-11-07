/*
 * adc.c
 *
 * Created: 03/11/2016 14:54:09
 *  Author: kvn
 */ 
 /*
 #include "adc.h";

 void adc_init(void)
 {
	 ADCSRA = (1<<ADEN) | // AD-enable
	 (1 << ADPS2) | // Prescale langsomste klok & konvertering
	 (1 << ADPS1) | // 16MHz/128
	 (1 << ADPS0);
	 ADMUX = (1<<REFS1) | (1<<REFS0); // Vælg intern ref. Spænding 1.1v

	 return;
 }
 uint16_t read_adc(uint8_t adc_input)
 {
	 ADMUX = adc_input; // Indstil MUX
	 ADCSRA |= (1<<ADSC); // Start konvertering
	 while ((ADCSRA & (1<<ADIF))==0); // Vent på komplet konvertering
	 ADCSRA |= (1<<ADIF); // Clear flag til næste konvertering
	 return ADCW; // Returner aflæste værdi. 0 - 1023
 }
 */