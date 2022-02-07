/*
 * I2C_slave.c
 *
 * Created: 30-01-2022 00:21:36
 * Author : Nitesh Kumar Dubey
 */ 
#define F_CPU 8000000UL
#define EEPROM_WRITE_ADDRESS 0x20
#define EEPROM_READ_ADDRESS 0x21

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "lib/uart_atmega16.h"

void I2C_Slave_Init(uint8_t slave_address);
int8_t I2C_Slave_Listen(void);
int8_t I2C_Slave_Transmit(char data);
char I2C_Slave_Receive(void);

int main(void)
{
   char buffer[10];
	int count = 0;
	
	I2C_Slave_Init(EEPROM_WRITE_ADDRESS);
    while (1) 
    {
		switch(I2C_Slave_Listen())	/* Check for SLA+W or SLA+R */
		{
			case 0:
			{
				do
				{
					sprintf(buffer, "%d    ", count);
					uart_sendString_ln(buffer);
					
					count = I2C_Slave_Receive();/* Receive data byte*/
					} while (count != -1);/* Receive until STOP/REPEATED START */
					count = 0;
					break;
				}
				case 1:
				{
					int8_t Ack_status;
					do
					{
						Ack_status = I2C_Slave_Transmit(count);/* Send data byte */
						sprintf(buffer, "%d    ",count);
						uart_sendString_ln(buffer);
						count++;
						} while (Ack_status == 0);/* Send until Ack is receive */
						break;
					}
					default:
					break;
				}
    }
}

void I2C_Slave_Init(uint8_t slave_address)
{
	TWAR=slave_address;		/* Assign Address in TWI address register */
	TWCR=(1<<TWEN)|(1<<TWEA)|(1<<TWINT);/* Enable TWI, Enable ack generation */
}

int8_t I2C_Slave_Listen(void)
{
	while(1)
	{
		uint8_t status;			/* Declare variable */
		while(!(TWCR&(1<<TWINT)));	/* Wait to be addressed */
		status=TWSR&0xF8;		/* Read TWI status register */
		if(status==0x60||status==0x68)	/* Own SLA+W received &ack returned */
		return 0;			/* Return 0 to indicate ack returned */
		if(status==0xA8||status==0xB0)	/* Own SLA+R received &ack returned */
		return 1;			/* Return 0 to indicate ack returned */
		if(status==0x70||status==0x78)	/* General call received &ack returned */
		return 2;			/* Return 1 to indicate ack returned */
		else
		continue;			/* Else continue */
	}
}

int8_t I2C_Slave_Transmit(char data)
{
	uint8_t status;
	TWDR=data;			/* Write data to TWDR to be transmitted */
	TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA);/* Enable TWI & clear interrupt flag */
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	status=TWSR&0xF8;		/* Read TWI status register */
	if(status==0xA0)		/* Check for STOP/REPEATED START received */
	{
		TWCR|=(1<<TWINT);	/* Clear interrupt flag & return -1 */
		return -1;
	}
	if(status==0xB8)		/* Check for data transmitted &ack received */
	return 0;			/* If yes then return 0 */
	if(status==0xC0)		/* Check for data transmitted &nack received */
	{
		TWCR|=(1<<TWINT);	/* Clear interrupt flag & return -2 */
		return -2;
	}
	if(status==0xC8)		/* Last byte transmitted with ack received */
	return -3;			/* If yes then return -3 */
	else			/* else return -4 */
	return -4;
}

char I2C_Slave_Receive(void)
{
	uint8_t status;		/* Declare variable */
	TWCR=(1<<TWEN)|(1<<TWEA)|(1<<TWINT);/* Enable TWI & generation of ack */
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	status=TWSR&0xF8;		/* Read TWI status register */
	if(status==0x80||status==0x90)/* Check for data received &ack returned */
	return TWDR;		/* If yes then return received data */

	/* Check for data received, nack returned & switched to not addressed slave mode */
	if(status==0x88||status==0x98)
	return TWDR;		/* If yes then return received data */
	if(status==0xA0)		/* Check wether STOP/REPEATED START */
	{
		TWCR|=(1<<TWINT);	/* Clear interrupt flag & return -1 */
		return -1;
	}
	else
	return -2;			/* Else return -2 */
}