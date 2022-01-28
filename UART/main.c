/*
 * UART.c
 *
 * Created: 26-01-2022 20:10:58
 * Author : Nitesh Kumar Dubey
 */ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include "lib/uart_atmega16.h"


int main(void)
{
	char c;
	uart_init(9600);
	
    while (1) 
    {
// 		c = uart_getChar();
// 		uart_sendChar(c);
		uart_sendChar('A');
		uart_sendString_ln("Nitesh");
		uart_sendInt(13);
		uart_sendFloat(12.55, 2);
    }
}