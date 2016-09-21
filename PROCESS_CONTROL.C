
#define F_CPU 14745600

#define __OPTIMIZE__ -O0

#include <util/delay.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include "sensor.h"
#include "lcd.h"
#include "hw_init.h"
#include "globalvalues.h"
#include "buzzer.h"
#include "process_control.h"

unsigned char rangeSensorValue;


#define	BLUE_VALUE_L  60 //70
#define	BLUE_VALUE_H  80
#define GREEN_VALUE_L  50	//60
#define GREEN_VALUE_H  59
#define RED_VALUE_L	  35	//50
#define RED_VALUE_H	 45



void orderCheckList();


typedef enum {
	RED,
	GREEN,
	BLUE,
	NONE

} food_t;


food_t foodType()
{

	food_t retFoodType;

	if ((rangeSensorValue >= BLUE_VALUE_L) && (rangeSensorValue <= BLUE_VALUE_H) )
	{	
	
		lcd_cursor(2, 8);
  		lcd_string("BLUfoodty");
		_delay_ms(1000);
		retFoodType = BLUE;
	}

	else if ((rangeSensorValue >= GREEN_VALUE_L) && (rangeSensorValue <= GREEN_VALUE_H))
	{	
		lcd_cursor(2, 8);
  		lcd_string("GREfoodty");
		_delay_ms(1000);
		retFoodType = GREEN;
	}

	else if (   (rangeSensorValue  >= RED_VALUE_L) && (rangeSensorValue  <= RED_VALUE_H))
	{	
			lcd_cursor(2, 8);
  		lcd_string("REDfoodty");
		_delay_ms(1000);
		retFoodType = RED;
	}
	else
	{

			lcd_cursor(2, 8);
  		lcd_string("NONEfoodt");
		_delay_ms(1000);
		retFoodType = NONE;
	}
	


	return(retFoodType);

}


 unsigned int redOrderValue=0;
 unsigned int blueOrderValue=0;
 unsigned int greenOrderValue=0;

#define MAX_TABLES	9

static unsigned char foodRed[MAX_TABLES] = {0,  };
static unsigned char foodGreen[MAX_TABLES] = {0, };
static unsigned char foodBlue[MAX_TABLES] = {0, }; 


unsigned char processTables(unsigned char table_no_in)
{
///////////////////////////////////////////////////
		lcd_cursor(2, 8);
  		lcd_string("ptabst   ");
		_delay_ms(1000);

		food_t retFoodType;

		unsigned char tableFound = 0;

		led_config();

	
		retFoodType = foodType();

		switch(retFoodType)
		{

			case BLUE:
				foodBlue[blueOrderValue] = table_no_in;
				blueOrderValue++;
				blinkBlueLed();
				tableFound = 1;
				lcd_cursor(2, 8);
  				lcd_string("BLUproces");
				_delay_ms(1000);
				break;

			case GREEN:
				foodGreen[greenOrderValue] = table_no_in;
				greenOrderValue++;
				blinkGreenLed();
				tableFound = 1;
				lcd_cursor(2, 8);
  				lcd_string("GREproces");
				_delay_ms(1000);;
				break;

			case RED:
				foodRed[redOrderValue] = table_no_in;
				redOrderValue++;
				blinkRedLed();
				tableFound = 1;
				lcd_cursor(2, 8);
  				lcd_string("REDproces");
				_delay_ms(1000);
				break;

		case NONE:
				
				tableFound = 0;
				lcd_cursor(2, 8);
  				lcd_string("NONproces");
				_delay_ms(1000);
				break;



		}
			lcd_cursor(2, 8);
  		lcd_string("ptexit   ");
			_delay_ms(1000);
		
		return (tableFound);

}

int orderDetails(int *orders)
{
	orders[0] = redOrderValue;
    orders[1] = greenOrderValue;
    orders[2] = blueOrderValue;
	return *orders;
}

void blinkRedLed()
{

				lcd_cursor(2, 8);
  				lcd_string("REDblik  ");
				_delay_ms(1000);
	PORTJ = 0x0;		// Switch off LEDs
	PORTJ = 0x10;
	_delay_ms(500);
	//PORTJ = 0x0;
	
}

void blinkGreenLed()
{
	lcd_cursor(2, 8);
  	lcd_string("GRNblik  ");
	_delay_ms(1000);
	PORTJ = 0x0;		// Switch off LEDs
	PORTJ = 0x08;
	_delay_ms(500);
	//PORTJ = 0x0;

}

void blinkBlueLed()
{
	lcd_cursor(2, 8);
  	lcd_string("BLUblik  ");
	_delay_ms(1000);
	PORTJ = 0x0;		// Switch off LEDs
	PORTJ = 0x04;
	_delay_ms(500);
	//PORTJ = 0x0;
}
		


/*
void orderCheckList()
{
	unsigned int i,j,k;

	food_t retFoodType;
	retFoodType = foodType();	

	switch (retFoodType)
	{

		case BLUE:
			for(i=0;i<=blueOrderValue;i++)
			{
				lcd_cursor(2, 8);
  				lcd_string("BLUorder ");
				_delay_ms(1000);
				buzzer();
				blinkBlueLed();
				_delay_ms(100);
			}
			break;

		case RED:
			for(k=0; k<=redOrderValue;k++)
			{
				lcd_cursor(2, 8);
  				lcd_string("REDorder ");
				_delay_ms(1000);
				buzzer();
				blinkRedLed();
				_delay_ms(100);
			}	
			break;

		case GREEN:

			for(j=0; j<=greenOrderValue;j++)
			{
				lcd_cursor(2, 8);
  				lcd_string("GRNorder ");
				_delay_ms(1000);
				buzzer();
				blinkGreenLed();
				_delay_ms(100);
			}
			break;

		case NONE:
			break;
	}

}
  */


  void orderCheckList(int position)
{
	unsigned int i,j,k;

	food_t retFoodType;
	retFoodType = foodType();	

	switch (position)
	{

		case 22:
			for(i=0;i<=blueOrderValue;i++)
			{
				lcd_cursor(2, 8);
  				lcd_string("BLUorder ");
				_delay_ms(1000);
				buzzer();
				blinkBlueLed();
				_delay_ms(100);
			}
			break;

		case 20:
			for(k=0; k<=redOrderValue;k++)
			{
				lcd_cursor(2, 8);
  				lcd_string("REDorder ");
				_delay_ms(1000);
				buzzer();
				blinkRedLed();
				_delay_ms(100);
			}	
			break;

		case 18:

			for(j=0; j<=greenOrderValue;j++)
			{
				lcd_cursor(2, 8);
  				lcd_string("GRNorder ");
				_delay_ms(1000);
				buzzer();
				blinkGreenLed();
				_delay_ms(100);
			}
			break;

		case NONE:
			break;
	}

}

