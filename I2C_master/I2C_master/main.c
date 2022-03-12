/*
 * I2C_master.c
 * Master Mode non-modular
 * Created: 29-01-2022 08:29:23
 * Author : Nitesh Kumar Dubey
 * References: https://www.electronicwings.com/avr-atmega/atmega1632-i2c , and Atmega16 data sheet
 */ 
#define F_CPU 8000000UL						/* Define CPU clock Frequency e.g. here its 8MHz */
#include <avr/io.h>							/* Include AVR std. library file */
#include <util/delay.h>						/* Include inbuilt defined Delay header file */
#include <stdio.h>							/* Include standard I/O header file */
#include <string.h>							/* Include string header file */
#include "lib/I2C_ATMEGA16.h"				/* Include I2C header file */
#include "lib/LCD_16X2.h"					/* Include LCD header file */

#define Slave_Write_Address		0x20
#define Slave_Read_Address		0x21
#define	count					10

int main(void)
{
	char buffer[10];
	
	LCD_16X2_init();								/* Initialize LCD */
	I2C_masterInit();								/* Initialize I2C */
	
	LCD_16X2_sendString_XY(1, 0, "Master Device");
	
	while (1)
	{
		LCD_16X2_sendString_XY(2, 0, "Sending :       ");
 		I2C_startWait(Slave_Write_Address);/* Start I2C communication with SLA+W */
		_delay_ms(5);
		
		for (uint8_t i = 0; i < count ; i++)
		{
			sprintf(buffer, "%d    ",i);
			LCD_16X2_sendString_XY(2, 13, buffer);
			I2C_write(i);					/* Send Incrementing count */
			_delay_ms(500);
		}
		
		LCD_16X2_sendString_XY(2, 0, "Receiving :       ");
		I2C_repeatedStart(Slave_Read_Address);	/* Repeated Start I2C communication with SLA+R */
		_delay_ms(5);
		
		for (uint8_t i = 0; i < count; i++)
		{
			if(i < count - 1)
			sprintf(buffer, "%d    ", I2C_readAck());/* Read and send Acknowledge of data */
			else
			sprintf(buffer, "%d    ", I2C_readNack());/* Read and Not Acknowledge to data */
			LCD_16X2_sendString_XY(2, 13, buffer);
			_delay_ms(500);
		}
		
 		I2C_stopTransmission();							/* Stop I2C */
	}
}
