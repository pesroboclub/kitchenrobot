
#define __OPTIMIZE__ -O0
#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h> 
#include "lcd.h"

#include "hw_init.h"




///////////////////////////////////LCD initialization ///////////////////////////////
/*
* Function Name: lcd_port_config 
* Input: none
* Output: none
* Logic: Code to initialize desired I/O port using IO port registers 
		 viz. DDRx and PORTx.
Example Call: lcd_port_config 
*/

void lcd_port_config ()
{
	 
	 // All pins except pin 3 are forced to 1
	 DDRC |= 0xF7; // All Port C pins except Pin no. 4 i.e(PC3) have to be set as output.

	// All pin values execept pin 3 is forced to 0
	PORTC &= 0x08; // Write suitable value in the PORT C register to make initial values to be "0" 
	 		  //Set initial value as "0" on all pins except Pin no.4 (PC3)
}
//////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////LED config///////////////
void led_config ()
{
 //DDRH |= 0xE0;
//	PORTH |= 0xE0;

DDRJ=0xFF;//initialize all the pins of port J as output pins.
		PORTJ=0x0;//set the values that all the LEDs remains off initially
}
////////////////////////////////////////////////////////////////





//////////////////////////////ADC related functions////////////////////////////////////
/*
* Function Name: adc_pin_config 
* Input: none
* Output: none
* Logic: Code to initialize desired I/O port using IO port registers 
		 viz. DDRx and PORTx.
Example Call: adc_pin_config 
*/
void adc_pin_config (void)
{
 DDRF = 0;// All Port F pins have to be set as input.
 PORTF = 0;// Write suitable value in the PORT F register to make initial values to be "0"
 DDRK = 0;//All Port K pins have to be set as input
 PORTK = 0;// Write suitable value in the PORT K register to make initial values to be "0"
}



/*
* Function Name: motion_pin_config 
* Input: none
* Output: none
* Logic: Code to initialize desired I/O port using IO port registers 
		 viz. DDRx and PORTx.
Example Call: motion_pin_config () 
*/
void motion_pin_config (void) 
{
 DDRA |= 0x0F;	//  Port A pins(PA3-PA0) have to be set as outputs.
 PORTA &= 0xF0;	// Write suitable value in the PORT A register to make initial values to be "0"
 DDRL |= 0x18;  //Setting PL3 and PL4 pins as output for PWM generation
 PORTL |= 0x18;	//Write suitable value in the PORT L register to set initial value of the Port L pins (PL3, PL4) to logic 1.
}






/*
* Function Name: motion_timer5_init 
* Input: none
* Output: none
* Logic: Code to initialize the register to generate the PWM using timer 5 
		 viz. TCCR5A,TCCR5B,TCNT5H,TCNT5L,OCR5AL and OCR5BL
Example Call: motion_timer5_init () 
*/
// Timer 5 initialized in PWM mode for velocity control
// Prescale:256
// PWM 8bit fast, TOP=0x00FF
// Timer Frequency:225.000Hz
void motion_timer5_init()
{
	TCCR5B = 0x00;	//Stop the timer initially
	TCNT5H = 0xFF; //Counter higher 8-bit value to which OCR5xL value is compared with
	TCNT5L = 0x01; //Counter lower 8-bit value to which OCR5xL value is compared with
	OCR5AL = 0xFF;//Output compare register low value for Left Motor
	OCR5BL = 0xFF;//Output compare register low value for Right Motor
	TCCR5A = 0xA9;	//Write suitable value in this register to override normal port functionality and to select Fast PWM 8-bit Mode.
	
	TCCR5B = 0x0B;	//Write suitable value in this register to set the desired waveform generation mode and to select a prescalar of 64.
}






/*
* Function Name: adc_init 
* Input: none
* Output: none
* Logic: Code to initialize the register to generate the PWM using timer 5 
		 viz. ADCSRA,ADCSRB,ADMUX,ACSR
Example Call: adc_init()) 
*/
void adc_init()
{
	// Precsale:2
	// ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0
	//   0   0      0    0    0    1     1     0

	ADCSRA = 0x06; //Write suitable value in this register to disable ADC during initialization.

	// -   ACME  -  -   MUX5 ADTS2 ADTS1 ADTS0
    // -    0            0   0      0     0
	ADCSRB = 0;	//Write suitable value in this register for initialization.

	// REFS1 REFS0 ADLAR MUX4 MUX3 MUX2 MUX1 MUX0
	//   0    0      1    0    0     0   0     0
	ADMUX = 0x20;	//Write appropraite value in the ADMUX register to select external Reference voltage (connected to AREF pin) and left adjustment active.

	ACSR = 0x80 ;	//Disable the analog comparator.(optional)

	// Prescale: retained as previous, ADC enabled
	ADCSRA |= 0x80;	//Write suitable value to this register to enable ADC and to select the required prescalar. 
}
