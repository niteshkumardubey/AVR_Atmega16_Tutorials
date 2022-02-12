/*
 * LCD_16X2.c
 *
 * Created: 30-01-2022 22:06:42
 * Author : Nitesh Kumar Dubey
 */ 

#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "lib/LCD_16X2.h"

int main(void)
{
	LCD_16X2_init();
	LCD_16X2_sendString_XY (2, 3, "Nitesh");
// 	LCD_16X2_sendString("I love my India.");
// 	LCD_16X2_sendChar('N');
//	LCD_16X2_sendInt(12305);
//	LCD_16X2_sendFloat(12.61, 3);
    while (1) 
    {
		;
    }
}