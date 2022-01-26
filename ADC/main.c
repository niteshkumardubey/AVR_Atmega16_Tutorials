/*
 * This is a simple example. We can remove the port pattern option
 * ADC.c (Single ended conversion) 
 * Version 1
 * Created: 23-01-2022 18:58:06
 * Author : Nitesh Kumar Dubey
 */ 

/************************************************************************/
/* In this code Temperature Sensor (LM35) is calibrated. Temperature value is send to PORTC. 
   PORTC will be toggle as per the patterns of temperature in degree celcius. 

   Temperature = (ADC_Value * V_REF_mV)/(1024 * 10);*/
/************************************************************************/
#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>

void ADC_init(void);
uint16_t adc_channel_read(uint8_t channel_number);

int main(void)
{
	uint16_t adc_value = 0;
	float temperature = 0;
	DDRC = 0xff;				// Set PORTC as output
	
	ADC_init();					// Initialize ADC by calling function
	
    while (1) 
    {
		adc_value = adc_channel_read(4);
		temperature = (adc_value * 5000)/(1024 * 10); // Formula to convert ADC value to temperature in Celcius
		PORTC = temperature;
    }
}

/************************************************************************/
/* ADC single ended initialization. Call this function in main function once.*/
/************************************************************************/
void ADC_init(void)
{
	ADC = 0;					// Set ADCL and ADCH to zero.We can also write these registers to zero individually (like ADCL = 0; ADCH = 0;).
	ADMUX &= ~(1 << 5);			// Result is right adjusted.
//	ADMUX |= (1 << 7)|(1 << 6);	// ADC reference voltage to internal 2.56V with external capacitor at Aref pin.
	ADMUX = (0x1 << 6);			// ADC reference voltage to external with external capacitor at Aref pin.
	ADCSRA |= (1 << 7)|(1 << 6)|(1 << 5)|(1 << 2);	// ADC enabled, ADC conversion started, ADC auto trigger enabled, prescaler is 16.
	SFIOR &= ~(0x0 << 5);		// ADC in free running mode.
}

/************************************************************************/
/* Reads takes analog signal from selected channel and converts to ADC value.
Arguments : channel_number = ADC channel on which we want to get signal (0 to 4).
Returns: ADC value in 16 bit                                             */
/************************************************************************/
uint16_t adc_channel_read(uint8_t channel_number)
{
	uint16_t adc_value = 0;
	ADMUX = (channel_number << 0); // Set channel
	ADCSRA|= (1 << 6);				// ADC conversion start bit must be on at the time of conversion.
	adc_value = ADC;
	return (adc_value);
}