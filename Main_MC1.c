/*
 * Main.c
 *
 *  Created on: Nov 28, 2020
 *      Author: MRM
 */


#include "LCD_4bitMode.h"
#include "keypad.h"
#include "UART.h"
#include "Main_MC1.h"
#include "Counter.h"



/* function to count the required time */

void APP_Counter(char time_sec)
{
	count ++;
	if(count == (4000*time_sec))    // checks if the global variable count reached the desired time
	{
		count = 0;
	}
}

/*********************************************************************************************************************************************************************/

/* function to take the desired amount of time to be counted time */

void my_Delay(char val)
{
	set_counter_value(val);   // set the value to be passed by the app_counter function.
	APP_Counter(val);  // call the app_counter function passing the required time to be counted

	Counter_Set_CallBack(APP_Counter,Timer2,Normal_Mode);  // set the call back passing the application function name, the timer type and the mode
	Counter_Config ptr = {Timer2,Normal_Mode,0,256,scale_8}; //configure the desired counter
	counter_ini(&ptr);  // passing the configuration pointer
	while(count != 0 );  //checks if the counter finished counting the desired time
	counter_Stop();  // stops the counter
}

void get_Pass(uint8 *Pass)
{
	int i;
	for(i=0;i<5;i++)
	{
		char ch;
		ch = KeyPad_getPressedKey();
		_delay_ms(500);
		Pass[i] = (ch + '30');
		LCD_displayChar('*');
	}
	Pass[i] = '#';
	while(KeyPad_getPressedKey() != 13);
}

void display_Option1(uint8 option)
{
	UART_receiveString(message);
	LCD_displayString(message);
	get_Pass(Pass);
	_delay_ms(100);
	LCD_clearScreen();
	UART_sendString(Pass);

	if(UART_receiveByte() == '0')
	{
		UART_receiveString(message);
		LCD_displayString(message);
		_delay_ms(500);
		LCD_clearScreen();
	}
	else
	{
		UART_receiveString(message);
		LCD_displayString(message);
		_delay_ms(500);

		get_Pass(Pass);
		_delay_ms(100);
		LCD_clearScreen();

		UART_sendString(Pass);

		UART_receiveString(message);
		LCD_displayString(message);
		_delay_ms(500);

		get_Pass(Pass);
		_delay_ms(100);
		LCD_clearScreen();

		UART_sendString(Pass);

		UART_receiveString(message);
		LCD_displayString(message);
		_delay_ms(500);
		LCD_clearScreen();
	}

}


void set_Pass()
{
	get_Pass(Pass);
	_delay_ms(500);
	LCD_clearScreen();

	UART_sendString(Pass);


	UART_receiveString(message);
	LCD_displayString(message);
	_delay_ms(500);
	get_Pass(Pass);
	_delay_ms(500);
	LCD_clearScreen();
	UART_sendString(Pass);
	_delay_ms(100);


	UART_receiveString(message);
	LCD_displayString(message);
	_delay_ms(500);
	LCD_clearScreen();
}


void display_Door_Status()
{
	UART_receiveString(message);
	LCD_displayString(message);
	my_Delay(15);
	LCD_clearScreen();


	UART_receiveString(message);
	LCD_displayString(message);
	my_Delay(15);
	LCD_clearScreen();
	_delay_ms(500);
}




void display_Option2(uint8 option)
{
	if(option == 2)
	{
		UART_receiveString(message);
		LCD_displayString(message);
		get_Pass(Pass);

		UART_sendString(Pass);
		LCD_clearScreen();
		_delay_ms(500);

		ch = UART_receiveByte();
		if(ch == '1')
		{
			display_Door_Status();
		}

		else if(ch == '0')
		{
			uint8 counter = 2;
			while(counter > 0)
			{
				UART_receiveString(message);
				LCD_displayString(message);
				get_Pass(Pass);
				_delay_ms(500);
				UART_sendString(Pass);
				LCD_clearScreen();
				counter --;

				if(UART_receiveByte() != no)
				{
					display_Door_Status();
					counter = 0;
					flag1 = 1;
				}
			}
			if(counter == 0 && flag1 == 0)
			{
				LCD_displayString(wrong_Pass);
				_delay_ms(3000);
				LCD_clearScreen();
			}
		}
	}

}















int main(void)
{
	uart_ConfigType u1 = {rate_9600,two,eight_bits,disabled};
	UART_init(&u1);
	LCD_Init();
	LCD_clearScreen();

	while(1)
	{
		UART_sendByte(ready);
		UART_receiveString(message);
		LCD_displayString(message);

		if(strcmp(message,"Set Pass:") == 0)
		{
			set_Pass();
		}

		else
		{
			menu_Option = KeyPad_getPressedKey();
			_delay_ms(500);
			LCD_integerToString(menu_Option);
			_delay_ms(300);

			UART_sendByte(menu_Option);
			LCD_clearScreen();
			if(menu_Option == 1)
			{
				display_Option1(menu_Option);
			}
			else if(menu_Option == 2)
			{
				display_Option2(menu_Option);
			}

		}
	}
}



