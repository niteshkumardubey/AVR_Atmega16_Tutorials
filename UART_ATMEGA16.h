#ifndef UART_ATMEGA16_H_
#define UART_ATMEGA16_H_

#define UBRR_VAL ((F_CPU/(16 * baud_rate))-1)			// Formula to calculate UBRRH and UBRRL values

#include <math.h>

void uart_init(unsigned long baud_rate);
void uart_sendChar(unsigned char data);
void uart_sendString(char* string);
void uart_sendString_ln(char* string);
void uart_sendInt(long val);
void uart_sendFloat(double val, int afterDecimal);
char uart_getChar(void);

#endif
