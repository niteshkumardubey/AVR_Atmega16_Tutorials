#ifndef LCD_16X2_H_
#define LCD_16X2_H_

#define LCD_COMMAND_PORT		PORTC
#define LCD_COMMAND_PORT_DDR	DDRC
#define LCD_DATA_PORT			PORTD
#define LCD_DATA_PORT_DDR		DDRD
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

char LCD_16X2_BUFF[50];

void LCD_16X2_init(void)
{
	LCD_COMMAND_PORT_DDR |= (1 << RS)|(1 << RW)|(1 << EN);
	LCD_DATA_PORT_DDR |= (1 << D0)|(1 << D1)|(1 << D2)|(1 << D3)|(1 << D4)|(1 << D5)|(1 << D6)|(1 << D7);
	LCD_COMMAND_PORT &= ~((1 << RS)|(1 << RW)|(1 << EN));
	LCD_DATA_PORT &= ~((1 << D0)|(1 << D1)|(1 << D2)|(1 << D3)|(1 << D4)|(1 << D5)|(1 << D6)|(1 << D7));
	
	LCD_16X2_sendCmd(0x01);		// LCD clear
	LCD_16X2_sendCmd(0x38);		// Enable dots matrix
	LCD_16X2_sendCmd(0x0e);		// Display on cursor on
// 	LCD_16X2_sendCmd(0x0c);		// Display on, cursor off
	LCD_16X2_sendCmd(0x06);		// Increment of cursor to the right
	LCD_16X2_sendCmd(0x80);		// Set cursor to 1st line, 1st column
}

void LCD_16X2_setMode(bool RS_mode)
{
	if (RS_mode)
	{
		LCD_COMMAND_PORT |= (1 << RS);
	}
	else
	{
		LCD_COMMAND_PORT &= ~(1 << RS);
	}
	
	LCD_COMMAND_PORT &= ~(1 << RW);
	LCD_COMMAND_PORT |= (1 << EN);
	_delay_ms(50);
	LCD_COMMAND_PORT &= ~(1 << EN);
	LCD_DATA_PORT = 0;
}

void LCD_16X2_sendCmd(uint8_t command)
{
	LCD_DATA_PORT = command;						// Send all bits
	LCD_16X2_setMode(LCD_COMMAND_MODE);
}

void LCD_16X2_sendChar(unsigned char data)
{
	LCD_DATA_PORT = data;							// Send all bits
	LCD_16X2_setMode(LCD_DATA_MODE);
}

void LCD_16X2_sendString(char* string)
{
	size_t i = 0;
//	LCD_16X2_sendCmd(0x01); // Clear display
	while(string[i] != '\0')
	{
		LCD_16X2_sendChar(string[i]);
		i++;
	}
}

void LCD_16X2_sendString_XY (char row, char column, char *string)	// Send string to LCD function
{
	if (row == 1)
	LCD_16X2_sendCmd((column & 0x0F)|0x80);			// Command of first row and required position<16 
	else if (row == 2)
	LCD_16X2_sendCmd((column & 0x0F)|0xC0);			// Command of Second row and required position<16 
	LCD_16X2_sendString(string);						// Call LCD string function 
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

void LCD_16X2_sendInt(long val)
{
	intToStr(val, LCD_16X2_BUFF, 0);
	LCD_16X2_sendString(LCD_16X2_BUFF);
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
		fpart = (fpart * pow(10, afterpoint)) + (1 / pow(10, afterpoint));

		intToStr((int)fpart, res + i + 1, afterpoint);
	}
}

void LCD_16X2_sendFloat(double val, int afterDecimal)
{
	ftoa(val, LCD_16X2_BUFF, afterDecimal);
	LCD_16X2_sendString(LCD_16X2_BUFF);
}
#endif