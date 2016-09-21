
#define F_CPU 14745600


#define __OPTIMIZE__ -O0

#include <util/delay.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "sensor.h"
#include "lcd.h"

unsigned char rangeSensorValue;
unsigned char leftSensorValue;
unsigned char middleSensorValue;
unsigned char rightSensorValue;

#define RIGHT_SENSOR_CHANNEL	1
#define CENTER_SENSOR_CHANNEL	2
#define LEFT_SENSOR_CHANNEL		3


/*
* Function Name: ADC_Conversion_wl_sensor 
* Input: ADC Channel number 
* Output: Digital value
* Logic: Code to convert the analog value of the three whiteline sensors into digital value.
Example Call: ADC_Conversion_wl_sensor( ADC channel number) 
*/

//Function For ADC Conversion
unsigned char ADC_Conversion_wl_sensor(unsigned char Ch) 
{
	//This function accepts ADC channel number as an input and returns the digital equivalent of the analog value read by the sensor.  
	// Complete the code to write appropriate value in the ADMUX register based on the input channel number passed.
	// Start the ADC conversion and return its digital equivalent value.
		//This function accepts ADC channel number as an input and returns the digital equivalent of the analog value read by the sensor.  
	// Complete the code to write appropriate value in the ADMUX register based on the input channel number passed.
	// Start the ADC conversion and return its digital equivalent value.
	if (Ch <= 7)
	{
		
		// MUX5 = 0; for channel number <=7
		ADCSRB &= 0xF7;   

		// Ensure lower 5 bits of ADMUX is 0 before 
		// considering channel number
		ADMUX &= 0xE0;

		// ADMUX4:0 = channel number
		ADMUX |= Ch;
	

	}
	else
		// Invalid channel number as we are addressing white light sensor only
		// Higher than channel number 7 is not processed in this function
		return 0;


	// ADSC=1 Start conversion	
	ADCSRA |= 0x40;

	// Wait for ADIF = 1
	while ( (ADCSRA & 0x10) == 0)
		;

	return (ADCH);

}

/*
* Function Name: ADC_Conversion_sharp_sensor
* Input: none
* Output: none
* Logic: Code to convert the analog value of the front sharp IR range sensor into digital value.
Example Call: ADC_Conversion_sharp_sensor
*/

unsigned char ADC_Conversion_sharp_sensor()
{


	// Complete the code to write appropriate value in the ADMUX register based on the ADC channel number of the front sharp IR range sensor.
	// Start the ADC conversion and return its digital equivalent value.
	//front sharp sensor is connected to ADC channel number 11

	// MUX5 = 1; for channel number >7
	ADCSRB |= 0x08;

	// Ensure lower 5 bits of ADMUX is 0 before 
	// considering channel number
	ADMUX &= 0xE0;

	// Channel 11 is used for front sensor.
	// ADMUX4:0 = 00011
	ADMUX |= 3;

	// ADSC=1 Start conversion	
	ADCSRA |= 0x40;

	// Wait for ADIF = 1
	while ( (ADCSRA & 0x10) == 0)
		;


	return (ADCH);


}




void readSensorValues()
{

		leftSensorValue = ADC_Conversion_wl_sensor(LEFT_SENSOR_CHANNEL);
		lcd_cursor(1,1);
		lcd_string("L");
		lcd_print(1, 2, leftSensorValue, 3);


		middleSensorValue = ADC_Conversion_wl_sensor(CENTER_SENSOR_CHANNEL);
		lcd_cursor(1, 6);
		lcd_string("C");
		lcd_print(1, 7, middleSensorValue, 3);


		rightSensorValue = ADC_Conversion_wl_sensor(RIGHT_SENSOR_CHANNEL);
		lcd_cursor(1, 11);
		lcd_string("R");
		lcd_print(1, 12, rightSensorValue, 3);

		rangeSensorValue = ADC_Conversion_sharp_sensor();
		lcd_cursor(2,1 );
		lcd_string("sh ");
		lcd_print(2, 4, rangeSensorValue, 3);
}

