/*
 * UART.c
 *
 * Created: 26-01-2022 20:10:58
 * Author : Nitesh Kumar Dubey
 */ 
#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

void uart_init(uint16_t baud_rate);
void uart_transmitData(unsigned char data);
int main(void)
{
	sei();
	uart_init(9600);
    while (1) 
    {
		uart_transmitData(1);
    }
}

void uart_init(uint16_t baud_rate)
{
	uint16_t UBRR_val = (uint16_t)(F_CPU/(16 * baud_rate))-1;			// Formula to calculate UBRRH and UBRRL values
	UDR = 0;
	UCSRA |= (1 << 6);										// Clear the transmit complete flag
	UCSRB |= (1 << 7)|(1 << 6)|(1 << 5)|(1 << 4)|(1 << 3);	// Receive complete, Transmit complete, UDR empty interrupt enable and rx, tx enable 
	UCSRB &= ~((1 << 2)|(1 << 1)|(1 << 0));					// USART char size bit 2 (8 bit data), RXB8, TXB8 
	UCSRC &= ~(1 << 7);										// This bit must be 0 for updating UBRRH register.
	UBRRH |= (UBRR_val >> 8);								// Set baud rate
	UBRRL |= UBRR_val;
	UCSRC |= (1 << 7)|(1 << 2)|(1 << 1);					// 7th bit must be 1 to update UCSRC, USART char size bit 1 and 0 (8 bit data)
	UCSRC &= ~((1 << 6)|(1 << 5)|(1 << 4)|(1 << 3)|(1 << 0));// Asynchronous operation, Parity disabled, stop bit 1, Clock polarity 0 in asynchronous mode
}

void uart_transmitData(unsigned char data)
{
	while(!(UCSRA & (1 << 5)));
	UDR = data;
}