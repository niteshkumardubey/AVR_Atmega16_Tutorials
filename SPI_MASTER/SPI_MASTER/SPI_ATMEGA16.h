#ifdef __cplusplus
	extern "C"
	{
#endif

#ifndef SPI_ATMEGA16_H_
#define SPI_ATMEGA16_H_

#define DDR_SPI DDRB
#define DD_SS DDB4
#define DD_MOSI DDB5
#define DD_MISO DDB6
#define DD_SCK DDB7

/********************* MASTER FUNCTIONS DECLARATIONS *****************************/
void SPI_masterInit(void);
void SPI_masterTransmit(char cData);
char SPI_masterReceive(void);

/********************* SLAVE FUNCTIONS DECLARATIONS *****************************/
void SPI_slaveInit(void);
void SPI_slaveTransmit(char cData);
char SPI_slaveReceive(void);

#endif

#ifdef __cplusplus
	extern "C"
	}
#endif