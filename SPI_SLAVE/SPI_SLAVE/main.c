/*
 * SPI_SLAVE.c
 *
 * Created: 12-03-2022 23:57:54
 * Author : Nitesh Kumar Dubey
 */ 

#define F_CPU 8000000UL
#define count 10

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "../../lib/LCD_16X2.h"
#include "../../lib/SPI_ATMEGA16.c"

int main(void)
{
	uint8_t count1;
	char buffer[10];
	
	LCD_16X2_init();
	SPI_slaveInit();
	
	LCD_16X2_sendString_XY(1, 0, "SLAVE LCD");
	
	while (1)
	{
// 		LCD_16X2_sendString_XY(2, 0, "RECEIVING:    ");
// 		count1 = SPI_slaveReceive();
// 		sprintf(buffer, "%d   ", count1);
// 		LCD_16X2_sendString_XY(2, 13, buffer);
		
		LCD_16X2_sendString_XY(2, 0, "SENDING:    ");
		for (uint8_t i = 0; i < 10 ; i++)
		{
			sprintf(buffer, "%d    ",i);
			LCD_16X2_sendString_XY(2, 13, buffer);
			SPI_transmit(i);
			_delay_ms(500);
		}
	}
}