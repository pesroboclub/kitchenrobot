 
#include "robocontrol.h"

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
/*
*	Global Variable Declaration
*/

// Declare any global variable if you require here
unsigned char rangeSensorValue;
unsigned char leftSensorValue;
unsigned char middleSensorValue;
unsigned char rightSensorValue;



extern moving_area_t moving_area;


#define	BLACKAREA_SENSE_VALUE	128
#define WHITEAREA_SENSE_VALUE	15
#define THRESHHOLD_10CM_VALUE	98

#define RIGHT_SENSOR_CHANNEL	1
#define CENTER_SENSOR_CHANNEL	2
#define LEFT_SENSOR_CHANNEL		3






// Function for robot velocity control
/*
* Function Name: velocity
* Input: none
* Output: none
* Logic: Function for robot velocity control
Example Call: velocity (left motor velocity, right motor velocity)
*/
void velocity (unsigned char left_motor, unsigned char right_motor)
{
	OCR5AL = (unsigned char)left_motor;
	OCR5BL = (unsigned char)right_motor;
}



/*
* Function Name: forward 
* Input: none
* Output: none
* Logic: Code used for setting motor's direction 
		 viz.PORTx.
Example Call: forward()
*/
void forward (void) //both wheels forward
{
  // Port A value is written without disturbing the higher nibble
  // Port A 3:0 = 0110
  PORTA |= 0x06;   // Write sutiable value in PORTA to set direction of both wheels as forward.
  PORTA &= 0xF6;   
}

/*
* Function Name: left 
* Input: none
* Output: none
* Logic: Code used for setting motor's direction 
		 viz.PORTx.
Example Call: left()
*/
void left (void) 
{
  // Port A value is written without disturbing the higher nibble
  // Port A 3:0 = 0101
 PORTA |= 0x05; //Write suitable value in PORTA register to rotate Left wheel backward and Right wheel forward
 PORTA &= 0xF5;
}
/*
* Function Name: right 
* Input: none
* Output: none
* Logic: Code used for setting motor's direction 
		 viz.PORTx.
Example Call: right()
*/
void right (void) 
{
  // Port A value is written without disturbing the higher nibble
  // Port A 3:0 = 1010
  PORTA |= 0x0A;	//Write suitable value in PORTA register to rotate Left wheel forward and Right wheel backward
  PORTA &= 0xFA;
}
/*
* Function Name: stop 
* Input: none
* Output: none
* Logic: Code used for setting motor's direction 
		 viz.PORTx.
Example Call: stop()
*/
void stop (void)
{
 // Port A value is written without disturbing the higher nibble
 // Port A 3:0 = 0000
  PORTA &= 0xF0;	//Write suitable value in PORTA register to stop the robot.
  lcd_cursor(2, 15);
  lcd_string(".s");

}


#define VELOCTY_VALUE	10




/* This variable is to enable handling of B B B as sensor values. 
   When the robot is in B B B, it is not possible just with those
   values to find out whether it should take left or right.
   There is a need to know previous position to find out whether
   it is on right portion or left portion of the track.
*/








void forward_vel()
{
	velocity(110, 110);
	moving_area = CENTRE;
	lcd_cursor(2, 8);
  	lcd_string("f_vel()  ");

}

void forward_vel_fast()
{
	velocity(200, 200);
	moving_area = CENTRE;
	lcd_cursor(2, 8);
  	lcd_string("f_vfast()");

}


void forward_slow()
{
	velocity(70,70);
	moving_area = CENTRE;
	lcd_cursor(2, 8);
  	lcd_string("fslow()  ");

}




void forward_left()
{
	velocity(0,190);

	// 1. Left turn from node takes the robo
	// to right part of the track

	// 2. Left turn from normal path takes the robo
	// to left part of the track



	lcd_cursor(2, 8);
  	lcd_string("fleft()  ");

}



void forward_right()
{
	velocity(190,0);

	// 1. Right turn from node takes the robo
	// to left part of the track

	// 2. Right turn from normal path takes the robo
	// to right part of the track




	lcd_cursor(2, 8);
  	lcd_string("fright() ");


}

void smoothLeft()
{
	velocity(90,130);
	moving_area = LEFT;
	
	lcd_cursor(2, 8);
  	lcd_string("sLeft()  ");


}

void smoothRight()
{
	velocity(130,90);
	moving_area = RIGHT;
	
	lcd_cursor(2, 8);
  	lcd_string("sRight()");


}



void waitUntilLeftTurn()
{

	
	forward_left();
	_delay_ms(FAST_TURN_DELAY);

	waitTillBackOnTrack(GO_LEFT);
}

void waitUntilRightTurn()
{

	forward_right();
	_delay_ms(FAST_TURN_DELAY);
	
	waitTillBackOnTrack(GO_RIGHT);

}

void waitUntilLeftTurnSmooth()
{
	
	smoothLeft();
	_delay_ms(SMOOTH_TURN_DELAY);
	waitTillBackOnTrack(GO_LEFT);
}

void waitUntilRightTurnSmooth()
{

	smoothRight();
	_delay_ms(SMOOTH_TURN_DELAY);
	waitTillBackOnTrack(GO_RIGHT);
	

}



void waitTillBackOnTrack(moving_dir_t moving_dir)
{


	stop();

	// Keep taking turn until W B W is reached
	do
	{

		forward();
		velocity(0,0);

		if (moving_dir == GO_LEFT)
			smoothLeft();
			//forward_left();
		else
			smoothRight();
			//forward_right();

		_delay_ms(SMOOTH_TURN_DELAY);
		stop();
		_delay_ms(100);
		readSensorValues();


	} 
	while (WWW);

/*
	while (WWW)
	{

		forward();
		velocity(0,0);

		if (moving_dir == GO_LEFT)
			smoothLeft();
		else
			smoothRight();

		_delay_ms(SMOOTH_TURN_DELAY);
		stop();
		_delay_ms(100);
    	readSensorValues();

	}

*/
	forward();
	velocity(0,0);

	lcd_cursor(2, 8);
	lcd_string("Exit_W   ");
}




void waitUntilNodeCrossed()
{

   	readSensorValues();


	while (BBB)
	{

		forward_vel();
		_delay_ms(FOWARD_DELAY_WHITE);
		readSensorValues();

	}
}




		

/*

void followTrack(positionStatus_t positionStatus_in)
{

	while (!WBW)
	{

		if ((positionStatus_in == eBBW) || (positionStatus_in == eBWW))
			smoothLeft();

		else if ((positionStatus_in == eWWB) || (positionStatus_in == eWBB))
			smoothRight();

		_delay_ms(SMOOTH_TURN_DELAY);
		stop();
		forward();
		velocity(0,0);
		_delay_ms(100);
		readSensorValues();

	}
}

*/



/******** Very smooth left functions */

void waitTillVerySmoothLeft()
{

	verySmoothLeft();
	_delay_ms(SMOOTH_TURN_DELAY);
	waitTillBackOnTrackVerySmooth(GO_LEFT);

}


void waitTillVerySmoothRight()
{

	verySmoothLeft();
	_delay_ms(SMOOTH_TURN_DELAY);
	waitTillBackOnTrackVerySmooth(GO_LEFT);
}


void waitTillBackOnTrackVerySmooth(moving_dir_t moving_dir)
{


	stop();

	// Keep taking turn until W B W is reached
	do
	{

		forward();
		velocity(0,0);

		if (moving_dir == GO_LEFT)
			verySmoothLeft();
			//forward_left();
		else
			verySmoothRight();
			//forward_right();

		_delay_ms(SMOOTH_TURN_DELAY);
		stop();
		_delay_ms(100);
		readSensorValues();


	} 
	while (WWW);

	forward();
	velocity(0,0);

	lcd_cursor(2, 8);
	lcd_string("Exit_SW   ");
}


void verySmoothRight()
{
	velocity(130,110);
	moving_area = RIGHT;
	
	lcd_cursor(2, 8);
  	lcd_string("vsRight()");


}

void verySmoothLeft()
{
	velocity(119,130);
	moving_area = LEFT;
	
	lcd_cursor(2, 8);
  	lcd_string("vsLeft()  ");

}


void tryLeftAndRightToBeOnTrack()
{
	int steps = 0;

	while ((steps <= 10) && (WWW))
	{
		smoothLeft();
		_delay_ms(SMOOTH_TURN_DELAY);
		readSensorValues();
		steps++;
	}

	if (WWW)
	{

		
		steps = 0;

		while ((steps <= 22) && (WWW))
		{
			smoothRight();
			_delay_ms(SMOOTH_TURN_DELAY);
			readSensorValues();
			steps++;
		}

	}
	

}
