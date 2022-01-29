/*
 * I2C_master.c
 * Master Mode non-modular
 * Created: 29-01-2022 08:29:23
 * Author : Nitesh Kumar Dubey
 * References: https://www.electronicwings.com/avr-atmega/atmega1632-i2c , and Atmega16 data sheet
 */ 
#define F_CPU 8000000UL
#define F_I2C 200000
#define I2C_TWBR (((F_CPU/F_I2C) - 16)/(2 * 64))

#include <avr/io.h>
#include <util/delay.h>

void I2C_init(void);
uint8_t I2C_start_condition_init(char writeAddress);

int main(void)
{
	I2C_init();
    while (1) 
    {
    }
}

void I2C_init(void)
{
	TWSR |= (1 << TWPS1)|(1 << TWPS0);
	TWBR = I2C_TWBR;
}

/************************************************************************/
/* Send start condition for writing operation.
   Inputs: Slave device write address.
   Returns: Status of transmission.                                     */
/************************************************************************/
uint8_t I2C_start_condition_init(char writeAddress)
{
	uint8_t status = 0;
	TWCR |= (1 << TWINT)|(1 << TWSTA)|(1 << TWEN);		// Clear the interrupt, wait till bus is free, enable I2C operation
	
	while(!(TWCR & (1 << TWINT)));						// Wait till interrupt occurs. Indicates that start condition is transmitted
	status = (TWSR & 0xf8);								// Check the status. Status table is given in controllers data sheet (Page no 178).
	
	if (status != 0x08)									// Check whether start transmitted or not?
	{
		return 0;
	}
	
	TWDR = writeAddress;								// Write slave device write address in I2C data register
	TWCR |= (1 << TWEN)|(1 << TWINT);					// Enable I2C and clear interrupt flag
	
	while (!(TWCR & (1 << TWINT)));						// Wait till interrupt occurs. Indicates that address and write bit is transmitted
	status = (TWSR & 0xf8);								// Check the status. Status table is given in controllers data sheet.
	
	if (status == 0x18)									// If slave write address transmitted and ACK received
	{
		return 1;
	}
	else if (status == 0x20)							// If slave write address transmitted and NACK received
	{
		return 2;
	}
	else												// If transmission failed
	{
		return 3;
	}
}

/************************************************************************/
/* Send repeated start condition for reading operation.
   Inputs: Slave device read address.
   Returns: Status of transmission.                                     */
/************************************************************************/
uint8_t I2C_repeated_start_condition_init(char readAddress)
{
	uint8_t status = 0;
	TWCR |= (1 << TWINT)|(1 << TWSTA)|(1 << TWEN);		// Clear the interrupt, wait till bus is free, enable I2C operation
	
	while(!(TWCR & (1 << TWINT)));						// Wait till interrupt occurs. Indicates that start condition is transmitted
	status = (TWSR & 0xf8);								// Check the status. Status table is given in controllers data sheet (Page no. 178).
	
	if (status != 0x10)									// Check whether repeated start transmitted or not?
	{
		return 0;
	}
	
	TWDR = readAddress;									// Write slave device read address in I2C data register
	TWCR |= (1 << TWEN)|(1 << TWINT);					// Enable I2C and clear interrupt flag
	
	while (!(TWCR & (1 << TWINT)));						// Wait till interrupt occurs. Indicates that address and write bit is transmitted
	status = (TWSR & 0xf8);								// Check the status. Status table is given in controllers data sheet.
	
	if (status == 0x40)									// If slave read address transmitted and ACK received
	{
		return 1;
	}
	else if (status == 0x48)							// If slave read address transmitted and NACK received
	{
		return 2;
	}
	else												// If transmission failed
	{
		return 3;
	}
}

/************************************************************************/
/* This function writes data/address on the bus.
   Inputs: - Data and address.
   Return: - Status of the event.                           */
/************************************************************************/
uint8_t I2C_write(char data)
{
	uint8_t status = 0;
	TWDR = data;										// Load the data to data register.
	TWCR |= (1 << TWEN)|(1 << TWINT);					// Enable I2C and clear interrupt flag
	
	while (!(TWCR & (1 << TWINT)));						// Wait till interrupt occurs. Indicates that address and write bit is transmitted
	status = (TWSR & 0xf8);								// Read the status.
	
	if (status == 0x28)									// If data transmitted and ACK received.
	{
		return 0;
	} 
	else if (status == 0x30)							// If data transmitted and NACK received.
	{
		return 1;
	} 
	else												// If transmission failed
	{
		return 3;
	}
}

char I2C_read_ack(void)
{
	TWCR |= (1 << TWEN)|(1 << TWINT)|(1 << TWEA);		// I2C enable, clear interrupt, ACK enable
	
	while (!(TWCR & (1 << TWINT)));						// Wait till interrupt occurs. Indicated that transmission completed.
	
	return TWDR;
}

char I2C_read_nack(void)
{
	TWCR |= (1 << TWEN)|(1 << TWINT);					// I2C enable, clear interrupt, ACK disable
	
	while (!(TWCR & (1 << TWINT)));						// Wait till interrupt occurs. Indicated that transmission completed.
	
	return TWDR;
}

void I2C_stop_transmission(void)
{
	TWCR |= (1 << TWSTO)|(1 << TWINT)|(1 << TWEN);
}

