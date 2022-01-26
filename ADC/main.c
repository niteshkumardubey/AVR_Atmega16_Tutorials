/*
 * ADC.c (Single ended conversion) 
 * Version 1
 * Created: 23-01-2022 18:58:06
 * Author : Nitesh Kumar Dubey
 */ 

/************************************************************************/
/* In this code ADC value is send to PORTC and PORTD. 
   PORTS will be toggle as per the patterns of ADC values. 
   
   ADC_Value  = (Vin*1024)/V_ref   */

/************************************************************************/
#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRC = 0xff;				// Set PORTC as output
	DDRD = 0xff;				// Set PORTD as output
	
	/* ADC initialization start. */
	ADC = 0;					// Set ADCL and ADCH to zero.We can also write these registers to zero individually (like ADCL = 0; ADCH = 0;).
	ADMUX &= (0x0 << 0);		// ADC channel 0 is selected.
	ADMUX &= ~(1 << 5);			// Result is right adjusted.
	ADMUX |= (1 << 7)|(1 << 6);	// ADC reference voltage to internal 2.56V with external capacitor at Aref pin.
//	ADMUX = (0x1 << 6);			// ADC reference voltage to external with external capacitor at Aref pin.
	ADCSRA |= (1 << 7)|(1 << 6)|(1 << 5)|(1 << 2);	// ADC enabled, ADC conversion started, ADC auto trigger enabled, prescaler is 16.
	SFIOR &= ~(0x0 << 5);		// ADC in free running mode.
	
    while (1) 
    {
		ADCSRA|= (1 << 6);		// ADC conversion start bit must be on at the time of conversion.
		
		PORTC = ADCL;			// Moving 8-bits of ADCL to PORTC.
		PORTD = ADCH;			// Moving 8-bits of ADCH to PORTD.
    }
}

