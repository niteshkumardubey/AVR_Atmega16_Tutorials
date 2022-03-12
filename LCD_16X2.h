#ifndef LCD_16X2_H_
#define LCD_16X2_H_

#define LCD_COMMAND_PORT		PORTA
#define LCD_COMMAND_PORT_DDR	DDRA
#define LCD_DATA_PORT			PORTB
#define LCD_DATA_PORT_DDR		DDRB
#define RS 0
#define RW 1
#define EN 2
#define D0 0
#define D1 1
#define D2 2
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7

#define LCD_COMMAND_MODE		false
#define LCD_DATA_MODE			true
#define LCD_8BIT_MODE			false
#define LCD_4BIT_MODE			true

#include <string.h>
#include <stdbool.h>

void LCD_16X2_init(void);
void LCD_16X2_sendCmd(uint8_t command);
void LCD_16X2_sendChar(unsigned char data);
void LCD_16X2_sendString(char* string);
void LCD_16X2_sendString_XY (char row, char column, char *str);
void LCD_16X2_sendInt(long val);
void LCD_16X2_sendFloat(double val, int afterDecimal);

#endif