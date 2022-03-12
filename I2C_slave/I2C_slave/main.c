/*
 * I2C_slave.c
 *
 * Created: 30-01-2022 00:21:36
 * Author : Nitesh Kumar Dubey
 * References: https://www.electronicwings.com/avr-atmega/atmega1632-i2c , and Atmega16 and Atmega32 data sheet
 */ 
#define F_CPU 8000000UL							/* Define CPU clock Frequency e.g. here its 8MHz */
#include <avr/io.h>								/* Include AVR std. library file */
#include <util/delay.h>							/* Include inbuilt defined Delay header file */
#include <stdio.h>								/* Include standard I/O header file */
#include <string.h>								/* Include string header file */
#include "lib/LCD_16X2.h"					/* Include LCD header file */
#include "lib/I2C_ATMEGA16.h"					/* Include I2C slave header file */

#define Slave_Address			0x20

int main(void)
{
	char buffer[10];
	int8_t count = 0;
	
	LCD_16X2_init();
	I2C_slaveInit(Slave_Address);
	
	LCD_16X2_sendString_XY(1, 0, "Slave Device");
	
	while (1)
	{
		switch(I2C_slaveListen())				/* Check for any SLA+W or SLA+R */
		{
			case 0:
			{
				LCD_16X2_sendString_XY(2, 0, "Receiving :       ");
				do
				{
					sprintf(buffer, "%d    ", count);
					LCD_16X2_sendString_XY(2, 13, buffer);
					count = I2C_slaveReceive();/* Receive data byte*/
				} while (count != -1);			/* Receive until STOP/REPEATED START received */
				count = 0;
				break;
			}
			case 1:
			{
				int8_t Ack_status;
				LCD_16X2_sendString_XY(2, 0, "Sending :       ");
				do
				{
					Ack_status = I2C_slaveTransmit(count);	/* Send data byte */
					sprintf(buffer, "%d    ",count);
					LCD_16X2_sendString_XY(2, 13, buffer);
					count++;
				} while (Ack_status == 0);		/* Send until Acknowledgment is received */
				break;
			}
			default:
			break;
		}
	}
}