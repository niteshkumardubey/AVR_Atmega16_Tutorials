/*
 * LCD_16X2.c
 *
 * Created: 30-01-2022 22:06:42
 * Author : Nitesh Kumar Dubey
 */ 

#define F_CPU 1000000UL

#define LCD_COMMAND_PORT		PORTC
#define LCD_COMMAND_PORT_DDR	DDRC
#define LCD_DATA_PORT			PORTD
#define LCD_DATA_PORT_DDR		DDRD
#define RS 0
#define RW 1
#define EN 2

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

void LCD_16X2_send_cmd(uint8_t command);
void LCD_16X2_send_char(unsigned char data);
void LCD_16X2_send_string(char* string);
void LCD_16X2_send_int(long val);
void LCD_16X2_send_float(double val, int afterDecimal);

char LCD_16X2_BUFF[50];

int main(void)
{
    LCD_COMMAND_PORT_DDR = 0xff;
	LCD_DATA_PORT_DDR = 0xff;
	
	LCD_16X2_send_cmd(0x01);		// LCD clear
	LCD_16X2_send_cmd(0x38);		// Enable dots matrix
//	LCD_16X2_send_cmd(0x0e);		// Display on cursor on
	LCD_16X2_send_cmd(0x0c);		// Display on, cursor off
	LCD_16X2_send_cmd(0x06);		// Increment of cursor to the right
	LCD_16X2_send_cmd(0x80);		// Set cursor to 1st line, 1st column
	
	
// 	LCD_16X2_send_string("NITESH");
// 	LCD_16X2_send_char(0x44);
//	LCD_16X2_send_int(12305);
	LCD_16X2_send_float(12.61, 3);
    while (1) 
    {
		;
    }
}

void LCD_16X2_send_cmd(uint8_t command)
{
	LCD_DATA_PORT = command;
	LCD_COMMAND_PORT &= ~(1 << RS);  // RS
	LCD_COMMAND_PORT &= ~(1 << RW);  // RW
	LCD_COMMAND_PORT |= (1 << EN);  // EN
	_delay_ms(50);
	LCD_COMMAND_PORT &= ~(1 << EN);
	LCD_DATA_PORT = 0;
}

void LCD_16X2_send_char(unsigned char data)
{
	LCD_DATA_PORT = data;
	LCD_COMMAND_PORT |= (1 << RS);  // RS
	LCD_COMMAND_PORT &= ~(1 << RW);  // RW
	LCD_COMMAND_PORT |= (1 << EN);  // EN
	_delay_ms(50);
	LCD_COMMAND_PORT &= ~(1 << EN);
	LCD_DATA_PORT = 0;
}

void LCD_16X2_send_string(char* string)
{
	size_t i = 0;
	LCD_16X2_send_cmd(0x01); // Clear display
	while(string[i] != '\0')
	{
		LCD_16X2_send_char(string[i]);
		i++;
	}
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

void LCD_16X2_send_int(long val)
{
	intToStr(val, LCD_16X2_BUFF, 0);
	LCD_16X2_send_string(LCD_16X2_BUFF);
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

void LCD_16X2_send_float(double val, int afterDecimal)
{
	ftoa(val, LCD_16X2_BUFF, afterDecimal);
	LCD_16X2_send_string(LCD_16X2_BUFF);
}