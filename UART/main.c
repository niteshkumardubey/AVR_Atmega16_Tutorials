/*
 * UART.c
 *
 * Created: 26-01-2022 20:10:58
 * Author : Nitesh Kumar Dubey
 */ 
#define F_CPU 8000000UL
#define UBRR_VAL ((F_CPU/(16 * baud_rate))-1)			// Formula to calculate UBRRH and UBRRL values

#include <avr/io.h>
//#include <stdlib.h>
#include <math.h>

char buff[50];

void uart_init(unsigned long baud_rate);
void uart_sendChar(unsigned char data);
void uart_sendString(char* string);
void uart_sendString_ln(char* string);
void uart_sendInt(long val);
void uart_sendFloat(double val, int afterDecimal);
char uart_getChar(void);

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

void uart_init(unsigned long baud_rate)
{
	UCSRB |= (1 << 4)|(1 << 3);								// rx, tx enable 
	UCSRB &= ~((1 << 2)|(1 << 1)|(1 << 0));					// USART char size bit 2 (8 bit data), RXB8, TXB8 
	UCSRC &= ~(1 << 7);										// This bit must be 0 for updating UBRRH register.
	UBRRH = (UBRR_VAL >> 8);								// Set baud rate
	UBRRL = UBRR_VAL;
	UCSRC |= (1 << 7)|(1 << 2)|(1 << 1);					// 7th bit must be 1 to update UCSRC, USART char size bit 1 and 0 (8 bit data)
	UCSRC &= ~((1 << 6)|(1 << 5)|(1 << 4)|(1 << 3)|(1 << 0));// Asynchronous operation, Parity disabled, stop bit 1, Clock polarity 0 in asynchronous mode
}

void uart_sendChar(unsigned char data)
{
	while(!(UCSRA & (1 << 5)));
	UDR = data;
}

void uart_sendString(char* string)
{
	uint8_t i = 0;
	while (string[i] != '\0')
	{
		uart_sendChar(string[i++]);
	}
}

void uart_sendString_ln(char* string)
{
	uint8_t i = 0;
	while (string[i] != '\0')
	{
		uart_sendChar(string[i++]);
	}
	uart_sendChar('\n');
}

void reverse(char* string, int len)
{
	int current_position = 0, next_position = len - 1, temp;
	while (current_position < next_position)
	{
		temp = string[current_position];
		string[current_position] = string[next_position];
		string[next_position] = temp;
		current_position++;
		next_position--;
	}
}

int intToStr(long val, char destination[], int digits)
{
	int i = 0;
	while (val)
	{
		destination[i++] = (val % 10) + '0';
		val = val / 10;
	}

	/* Add 0s in beginning. */
	while (i < digits)
	destination[i++] = '0';

	/* Reverse string. */
	reverse(destination, i);
	destination[i] = '\0';
	return i;
}

void uart_sendInt(long val)
{
	intToStr(val, buff, 0);
	uart_sendString_ln(buff);
}

void ftoa(double val, char* res, int afterpoint)
{
	// Extract integer part
	int ipart = (int)val;

	// Extract floating part
	float fpart = val - (float)ipart;

	// convert integer part to string
	int i = intToStr(ipart, res, 0);

	// check for display option after point
	if (afterpoint != 0) {
		res[i] = '.'; // add dot

		// Get the value of fraction part upto given no.
		// of points after dot. The third parameter
		// is needed to handle cases like 233.007
		fpart = fpart * pow(10, afterpoint);

		intToStr((int)fpart, res + i + 1, afterpoint);
	}
}

void uart_sendFloat(double val, int afterDecimal)
{
	ftoa(val, buff, afterDecimal);
	uart_sendString_ln(buff);
}

char uart_getChar(void)
{
	while(!(UCSRA & (1 << 7)));
	
	return (UDR);
}