/************************************************************************/
/* In this library all the I2C functions are defined and declared. 
   Tested on only AtMega16 micro controller.     
   
   Author: Nitesh Kumar Dubey */
/************************************************************************/

#include "I2C_ATMEGA16.h"	// Include I2C functions

/*################################################################################################################################################*/
/************************************************************************/
/* I2C master functions definitions.                                     */
/************************************************************************/
/*################################################################################################################################################*/
/************************************************************************/
/* Initializes the I2C in master mode
   Inputs: void
   Returns: void                                     */
/************************************************************************/
void I2C_masterInit(void)
{
	TWBR = BITRATE(TWSR = 0x00);							// Get bit rate register value from formula defined by macro.
}

/************************************************************************/
/* Send start condition for writing operation.
   Inputs: Slave device write address.
   Returns: Status of transmission.                                     */
/************************************************************************/
uint8_t I2C_start(char write_address)
{
	uint8_t status;											// Declare variable to store status
	TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);					// Enable TWI, generate start condition and clear interrupt flag 
	
	while (!(TWCR & (1<<TWINT)));							// Wait until TWI finish its current job (start condition) 
	status = TWSR & 0xF8;									// Read TWI status register with masking lower three bits 
	if (status != 0x08)										// Check weather start condition transmitted successfully or not? 
	{
		return 0;											// If not then return 0 to indicate start condition fail 
	}
	TWDR = write_address;									// If yes then write SLA+W in TWI data register 
	TWCR = (1<<TWEN)|(1<<TWINT);							// Enable TWI and clear interrupt flag 
	
	while (!(TWCR & (1<<TWINT)));							// Wait until TWI finish its current job (Write operation) 
	status = TWSR & 0xF8;									// Read TWI status register with masking lower three bits 
	if (status == 0x18)										// Check weather SLA+W transmitted & ACK received or not? 
	{
		return 1;											// If yes then return 1 to indicate ACK received i.e. ready to accept data byte
	} 
	if (status == 0x20)										// Check weather SLA+W transmitted & NACK received or not? 
	{
		return 2;											// If yes then return 2 to indicate NACK received i.e. device is busy
	}
	else
	{
		return 3;											// Else return 3 to indicate SLA+W failed
	}
}

/************************************************************************/
/* Send repeated start condition for reading operation.
   Inputs: Slave device read address.
   Returns: Status of transmission.                                     */
/************************************************************************/
uint8_t I2C_repeatedStart(char readAddress)
{
	uint8_t status;											// Declare variable to store status
	TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);					// Enable TWI, generate start condition and clear interrupt flag
	
	while (!(TWCR & (1<<TWINT)));							// Wait until TWI finish its current job (start condition) 
	status = TWSR & 0xF8;									// Read TWI status register with masking lower three bits 
	if (status != 0x10)										// Check weather repeated start condition transmitted successfully or not?
	{
		return 0;											// If no then return 0 to indicate repeated start condition fail
	}
	TWDR = readAddress;									// If yes then write slave address with read byte (SLA+R) in TWI data register
	TWCR = (1<<TWEN)|(1<<TWINT);							// Enable TWI and clear interrupt flag
	
	while (!(TWCR & (1<<TWINT)));							// Wait until TWI finish its current job (Write operation)
	status = TWSR & 0xF8;									// Read TWI status register with masking lower three bits
	if (status == 0x40)										// Check weather SLA+R transmitted & ACK received or not?
	{
		return 1;											// If yes then return 1 to indicate ACK received
	}
	if (status == 0x20)										// Check weather SLA+R transmitted & NACK received or not?
	{
		return 2;											// If yes then return 2 to indicate NACK received i.e. device is busy
	}
	else
	{
		return 3;											// Else return 3 to indicate SLA+R failed
	}
}

/************************************************************************/
/* Stops I2C transmission.
   Inputs: void.
   Returns: void.                                     */
/************************************************************************/
void I2C_stopTransmission(void)
{
	TWCR=(1<<TWSTO)|(1<<TWINT)|(1<<TWEN);					// Enable TWI, generate stop condition and clear interrupt flag
	while(TWCR & (1<<TWSTO));								// Wait until stop condition
}

/************************************************************************/
/* Send start condition for writing operation.
   Inputs: Slave device write address.
   Returns: void                                     */
/************************************************************************/
void I2C_startWait(char writeAddress)	
{
	uint8_t status;											// Declare variable to store status
	while (1)
	{
		TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);				// Clear the interrupt, wait till bus is free, enable I2C operation
		
		while (!(TWCR & (1<<TWINT)));						// Wait till interrupt occurs. Indicates that start condition is transmitted
		status = TWSR & 0xF8;								// Check the status. Status table is given in controllers data sheet (Page no 178).
		if (status != 0x08)									// Check whether start transmitted or not?
		{
			continue;										// If no then continue with start loop again
		}
		TWDR = writeAddress;								// Write slave device write address (SLA + W) in I2C data register
		TWCR = (1<<TWEN)|(1<<TWINT);						// Enable I2C and clear interrupt flag
		
		while (!(TWCR & (1<<TWINT)));						// Wait till interrupt occurs. Indicates that address and write bit is transmitted
		status = TWSR & 0xF8;								// Check the status. Status table is given in controllers data sheet.
		if (status != 0x18 )								// If slave write address transmitted and ACK received
		{
			I2C_stopTransmission();							// If not then generate stop condition
			continue;										// continue with start loop again
		}
		break;												// If yes then break loop
	}
}

/************************************************************************/
/* This function writes data/address on the bus.
   Inputs: - Data and address.
   Return: - Status of the event.                           */
/************************************************************************/
uint8_t I2C_write(char data)
{
	uint8_t status;											// Declare variable to hold I2C status
	TWDR = data;											// Load the data to data register.
	TWCR = (1<<TWEN)|(1<<TWINT);							// Enable I2C and clear interrupt flag
	
	while (!(TWCR & (1<<TWINT)));							// Wait till interrupt occurs. Indicates that address and write bit is transmitted
	status = TWSR & 0xF8;									// Read the status.
	
	if (status == 0x28)										// If data transmitted and ACK received.
	{
		return 0;											// If yes then return 0 to indicate ACK received
	}
	if (status == 0x30)										// If data transmitted and NACK received.
	{
		return 1;											// If yes then return 1 to indicate NACK received
	}
	else
	{
		return 2;											// Else return 2 to indicate data transmission failed
	}
}

char I2C_readAck(void)
{
	TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA);					// I2C enable, clear interrupt, ACK enable
	while (!(TWCR & (1<<TWINT)));							// Wait till interrupt occurs. Indicated that transmission completed.
	return TWDR;											// Return received data
}

char I2C_readNack(void)
{
	TWCR=(1<<TWEN)|(1<<TWINT);								// I2C enable, clear interrupt, ACK disable
	while (!(TWCR & (1<<TWINT)));							// Wait till interrupt occurs. Indicated that transmission completed.
	return TWDR;											// Return received data
}


/*######################################################################################################################################################*/
/************************************************************************/
/* I2C slave functions definitions.                                     */
/************************************************************************/
/*######################################################################################################################################################*/
/************************************************************************/
/* Initializes the I2C in slave mode
   Inputs: slave address
   Returns: void                                     */
/************************************************************************/
void I2C_slaveInit(uint8_t slaveAddress)
{
	TWAR = slaveAddress;						// Assign address in TWI address register
	TWCR = (1<<TWEN) | (1<<TWEA) | (1<<TWINT);	// Enable TWI, Enable ACK generation, clear TWI interrupt
}

/************************************************************************/
/* Waits for start condition
   Inputs: void
   Returns: void                                     */
/************************************************************************/
int8_t I2C_slaveListen(void)
{
	while(1)
	{
		uint8_t status;								// Declare variable to hold I2C status
		while (!(TWCR & (1<<TWINT)));				// Wait till interrupt occurs. Indicates that address and write bit (SLA+W) is transmitted
		status = TWSR & 0xF8;						// Read TWI status register with masking lower three bits
		if ((status == 0x60) || (status == 0x68))	// Check weather own SLA+W received & ACK returned (TWEA = 1)
		{
			return 0;								// If yes then return 0 to indicate ACK returned 
		}
		if ((status == 0xA8) || (status == 0xB0))	// Check weather own SLA+R received & ACK returned (TWEA = 1)
		{
			return 1;								// If yes then return 1 to indicate ACK returned
		}
		if ((status == 0x70) || (status == 0x78))	// Check weather general call received & ACK returned (TWEA = 1)
		{
			return 2;								// If yes then return 2 to indicate ACK returned
		}
		else
		{
			continue;								// Else continue to listen
		}
	}
}

/************************************************************************/
/* Writes data to master.
   Inputs: (char) data
   Returns: (int8t) error status                                     */
/************************************************************************/
int8_t I2C_slaveTransmit(char data)
{
	uint8_t status;								// Declare variable to hold I2C status
	TWDR = data;								// Load data to TWDR to be transmitted
	TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWEA);		// Enable TWI and clear interrupt flag
	while (!(TWCR & (1<<TWINT)));				// Wait until TWI finish its current job (Write operation)
	status = TWSR & 0xF8;						// Read TWI status register with masking lower three bits
	if (status == 0xA0)							// Check weather STOP/REPEATED START received */
	{
		TWCR |= (1<<TWINT);						// If yes then clear interrupt flag & return -1
		return -1;
	}
	if (status == 0xB8)							// Check weather data transmitted & ACK received
	{
		return 0;								// If yes then return 0
	}
	if (status == 0xC0)							// Check weather data transmitted & NACK received
	{
		TWCR |= (1<<TWINT);						// If yes then clear interrupt flag & return -2
		return -2;
	}
	if (status == 0xC8)							// If last data byte transmitted with NACK received TWEA = 0
	{
		return -3;								// If yes then return -3
	}
	else										
	{
		return -4;								// else return -4
	}
}

/************************************************************************/
/* Waits for start condition
   Inputs: void
   Returns: void                                     */
/************************************************************************/
char I2C_slaveReceive(void)
{
	uint8_t status;								// Declare variable to hold I2C status
	TWCR=(1<<TWEN)|(1<<TWEA)|(1<<TWINT);		// Enable TWI, generation of ACK and clear interrupt flag 
	while (!(TWCR & (1<<TWINT)));				// Wait until TWI finish its current job (read operation) 
	status = TWSR & 0xF8;						// Read TWI status register with masking lower three bits 
	if (status == 0x80 || status == 0x90)		// Check weather data received & ACK returned (TWEA = 1) 
	{
		return TWDR;							// If yes then return received data 
	}
	if (status == 0x88 || status == 0x98)		// Check weather data received, NACK returned and switched to not addressed slave mode 
	{
		return TWDR;							// If yes then return received data 
	}
	if (status == 0xA0)							// Check weather STOP/REPEATED START received 
	{
		TWCR |= (1<<TWINT);						// If yes then clear interrupt flag & return 0 
		return -1;
	}
	else
	{
		return -2;								// Else return 1 
	}
}