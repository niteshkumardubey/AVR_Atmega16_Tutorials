/*
 * I2C_master.c
 * Master Mode non-modular
 * Created: 29-01-2022 08:29:23
 * Author : Nitesh Kumar Dubey
 * References: https://www.electronicwings.com/avr-atmega/atmega1632-i2c , and Atmega16 datasheet
 */ 
#define F_CPU 8000000UL
#define F_I2C 200000
#define I2C_TWBR (((F_CPU/F_I2C) - 16)/(2 * 64))

#include <avr/io.h>
#include <util/delay.h>

void I2C_init(void);
uint8_t start_condition_init(char writeAddress);

int main(void)
{
	I2C_init();
	TWSR |= (1 << TWPS1)|(1 << TWPS0);
    while (1) 
    {
    }
}

void I2C_init(void)
{
	TWBR = I2C_TWBR;
}

uint8_t start_condition_init(char writeAddress)
{
	uint8_t status = 0;
	TWCR |= (1 << TWINT)|(1 << TWSTA)|(1 << TWEN);		// Clear the interrupt, wait till bus is free, enable I2C operation
	
	while(!(TWCR & (1 << TWINT)));						// Wait till interrupt occurs
	status = (TWSR & 0xf8);								// Check the status. Status table is given in controlers datasheet.
}