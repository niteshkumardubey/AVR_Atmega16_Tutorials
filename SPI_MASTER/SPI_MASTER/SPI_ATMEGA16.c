#include "SPI_ATMEGA16.h"


/********************* MASTER FUNCTIONS DEFINITION *****************************/
void SPI_masterInit(void)
{
	/* Set MOSI and SCK output, all others input */
	DDR_SPI |= (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS);
	DDR_SPI &= ~(1<<DD_MISO);
	
	/* Set slave select pin high */
	PORTB |= (1<<DD_SS);
	
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	
	/* Disable speed doubler */
	SPSR &= ~(1<<SPI2X);
}

void SPI_masterTransmit(char cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
}

char SPI_masterReceive(void)
{
	return 0;
}

/********************* MASTER FUNCTIONS DEFINITION *****************************/
void SPI_slaveInit(void)
{
	/* Set MISO output, all others input */
	DDR_SPI |= (1<<DD_MISO);
	DDR_SPI &= ~((1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS));
	/* Enable SPI */
	SPCR = (1<<SPE);
}

void SPI_slaveTransmit(char cData)
{
	;
}

char SPI_slaveReceive(void)
{
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)))
	;
	/* Return data register */
	return (SPDR);
}