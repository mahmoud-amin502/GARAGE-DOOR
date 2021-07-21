/*
 * LCD_4bitMode.c
 *
 *  Created on: Nov 5, 2020
 *      Author: MRM
 */



#include "LCD_4bitMode.h"




void LCD_Init()
{
	LCD_CTRL_PORT_DIR |= (1<<E) | (1<<RS) | (1<<RW); /* Configure the control pins(E,RS,RW) as output pins */

#if (DATA_BITS_MODE == 4)
#ifdef UPPER_PORT_PINS
	LCD_DATA_PORT_DIR |= 0xF0; /* Configure the highest 4 bits of the data port as output pins */
#else
	LCD_DATA_PORT_DIR |= 0x0F; /* Configure the lowest 4 bits of the data port as output pins */
#endif
	LCD_sendCommand(FOUR_BITS_DATA_MODE); /* initialize LCD in 4-bit mode */
	LCD_sendCommand(TWO_LINE_LCD_Four_BIT_MODE); /* use 2-line lcd + 4-bit Data Mode + 5*7 dot display Mode */
#elif (DATA_BITS_MODE == 8)
	LCD_DATA_PORT_DIR = 0xFF; /* Configure the data port as output port */
	LCD_sendCommand(TWO_LINE_LCD_Eight_BIT_MODE); /* use 2-line lcd + 8-bit Data Mode + 5*7 dot display Mode */
#endif

	LCD_sendCommand(CURSOR_OFF); /* cursor off */
	LCD_sendCommand(CLEAR_COMMAND); /* clear LCD at the beginning */
}


void LCD_sendCommand(uint8 command)
{

	CLEAR_BIT(LCD_CTRL_PORT,PD4);  //RS pin
	CLEAR_BIT(LCD_CTRL_PORT,PD5);  //R/W pin

	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT,PD6);   //Enable pin
	_delay_ms(1);

#if(DATA_BITS_MODE == 4)

#ifdef UPPER_PORT_PINS

	LCD_DATA_PORT = ((LCD_DATA_PORT & 0x0f) | (command & 0xf0)) ;

	_delay_ms(1);
	CLEAR_BIT(LCD_CTRL_PORT,PD6);
	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT,PD6);   //Enable pin
	_delay_ms(1);

	LCD_DATA_PORT = ((LCD_DATA_PORT & 0x0f) | ((command & 0x0f) << 4));
	_delay_ms(1);
	CLEAR_BIT(LCD_CTRL_PORT,PD6);
	_delay_ms(1);

#else

	LCD_DATA_PORT = ((LCD_DATA_PORT & 0xf0) | ((command & 0xf0)>>4));

	_delay_ms(1);
	CLEAR_BIT(LCD_CTRL_PORT,PD6);
	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT,PD6);   //Enable pin
	_delay_ms(1);

	LCD_DATA_PORT = ((LCD_DATA_PORT & 0xf0) | (command & 0x0f));

	_delay_ms(1);
	CLEAR_BIT(LCD_CTRL_PORT,PD6);
	_delay_ms(1);

#endif


#elif (DATA_BITS_MODE == 8)

	LCD_DATA_PORT = command;
	_delay_ms(1);

	CLEAR_BIT(LCD_CTRL_PORT,PD6);
	_delay_ms(1);

#endif

}



void LCD_displayChar(uint8 data)
{

	SET_BIT(LCD_CTRL_PORT,PD4);  //RS pin
	CLEAR_BIT(LCD_CTRL_PORT,PD5);  //R/W pin

	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT,PD6);   //Enable pin
	_delay_ms(1);

#if(DATA_BITS_MODE == 4)

#ifdef UPPER_PORT_PINS

	LCD_DATA_PORT = ((LCD_DATA_PORT & 0x0f) | (data & 0xf0)) ;

	_delay_ms(1);
	CLEAR_BIT(LCD_CTRL_PORT,PD6);
	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT,PD6);   //Enable pin
	_delay_ms(1);

	LCD_DATA_PORT = ((LCD_DATA_PORT & 0x0f) | ((data & 0x0f) << 4));
	_delay_ms(1);
	CLEAR_BIT(LCD_CTRL_PORT,PD6);
	_delay_ms(1);

#else

	LCD_DATA_PORT = ((LCD_DATA_PORT & 0xf0) | ((data & 0xf0)>>4));

	_delay_ms(1);
	CLEAR_BIT(LCD_CTRL_PORT,PD6);
	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT,PD6);   //Enable pin
	_delay_ms(1);

	LCD_DATA_PORT = ((LCD_DATA_PORT & 0xf0) | (data & 0x0f));

	_delay_ms(1);
	CLEAR_BIT(LCD_CTRL_PORT,PD6);
	_delay_ms(1);

#endif


#elif (DATA_BITS_MODE == 8)

	LCD_DATA_PORT = data;
	_delay_ms(1);

	CLEAR_BIT(LCD_CTRL_PORT,PD6);
	_delay_ms(1);

#endif

}




void LCD_displayString(uint8 * str)
{
	uint8 i=0;
	while(*str != '\0')
	{
		if(i<16)
		{
			LCD_displayChar(*str);
			str++;
			++i;
		}

		else
		{
			LCD_goToRowColumn(2,((i-16)+1));
			LCD_displayChar(*str);
			str++;
			i++;
		}

	}
}



void LCD_goToRowColumn(uint8 row, uint8 col)
{

	uint8 address;

	row-=1;
	col-=1;


	switch (row)
	{
	case 0:
		address = col;
		break;

	case 1:
		address = 0x40 + col;
		break;

	case 2:
		address = 0x10 + col;
		break;

	case 3:
		address = 0x50 + col;
		break;
	}


	LCD_sendCommand((address | 0x80));
}


void LCD_displayStringRowColumn(uint8 row,uint8 col, uint8 *Str)
{
	LCD_goToRowColumn(row, col);
	LCD_displayString(Str);
}



void LCD_clearScreen()
{
	LCD_sendCommand(0x01);
}


void LCD_integerToString(int num)
{
	char *buffer[16];
	itoa(num,buffer,10);
	LCD_displayString(buffer);
}

