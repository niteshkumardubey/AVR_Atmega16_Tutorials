
#ifndef SPI_ATMEGA16_H_
#define SPI_ATMEGA16_H_

#define DDR_SPI DDRB
#define DD_SS DDB4
#define DD_MOSI DDB5
#define DD_MISO DDB6
#define DD_SCK DDB7

/********************* MASTER FUNCTIONS DECLARATIONS *****************************/
void SPI_masterInit(void);
char SPI_masterReceive(void);

/********************* SLAVE FUNCTIONS DECLARATIONS *****************************/
void SPI_slaveInit(void);
char SPI_slaveReceive(void);

/********************* COMMON FUNCTIONS DECLARATIONS *****************************/
void SPI_transmit(char cData);

#endif