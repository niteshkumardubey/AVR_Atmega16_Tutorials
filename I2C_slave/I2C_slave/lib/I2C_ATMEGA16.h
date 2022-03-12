/************************************************************************/
/* In this library all the I2C functions are declared. 
   Tested on only AtMega16 micro controller.     */
/************************************************************************/
#ifndef I2C_ATMEGA16_H_										// If I2C_ATMEGA16.h is not defined then define it to protect multi include
#define I2C_ATMEGA16_H_
																	
#define SCL_CLK 100000L																	// Define I2C SCL frequency
#define BITRATE(TWSR)	((F_CPU/SCL_CLK)-16)/(2*pow(4,(TWSR&((1<<TWPS0)|(1<<TWPS1)))))	// Define bit rate

#include <math.h>																		// Include math function

/************************************************************************/
/* I2C master functions declarations.                                   */
/************************************************************************/
void I2C_masterInit(void);										// I2C initialize function
uint8_t  I2C_start(char writeAddress);						// I2C start function
uint8_t  I2C_repeatedStart(char readAddress);				// I2C repeated start function
void I2C_stopTransmission(void);							// I2C stop function
void I2C_startWait(char writeAddress);						// I2C start wait function
uint8_t  I2C_write(char data);								// I2C write function
char I2C_readAck(void);										// I2C read ACK function
char I2C_readNack(void);									// I2C read NACK function

/************************************************************************/
/* I2C slave functions declarations.                                   */
/************************************************************************/
void I2C_slaveInit(uint8_t slaveAddress);
int8_t I2C_slaveListen(void);
int8_t I2C_slaveTransmit(char data);
char I2C_slaveReceive(void);

#endif														/* I2C_ATMEGA16_H_ */