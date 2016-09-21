/*sep_03
*
* File name: Experiment-2
* Description: This experiment is a part of Task 6. It is aimed to get you familiar with whiteline following.
* This experiment involves writing a code to make the robot follow a white line.
  Author: e-Yantra Team
*/

//#define _DEBUG 1
#define __OPTIMIZE__ -O0
#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h> 
#include "lcd.h"
#include "globalvalues.h"
#include "buzzer.h"
#include "hw_init.h"
#include "sensor.h"
#include "robocontrol.h"
#include "process_control.h"


//global variable,
moving_area_t moving_area;
int foundNode = 0;
const int position[] = {GO_FORWARD, GO_LEFT, GO_RIGHT, GO_STOP_FORWARD, GO_STOP_FORWARD, GO_STOP_FORWARD,
						 GO_RIGHT, GO_STOP_FORWARD, GO_STOP_FORWARD, GO_STOP_FORWARD, GO_RIGHT, 
						 GO_STOP_FORWARD, GO_STOP_FORWARD, GO_STOP_FORWARD, GO_RIGHT, GO_RIGHT,
						 GO_LEFT, GO_RIGHT, GO_STOP_FORWARD, GO_RIGHT, GO_STOP_FORWARD, GO_RIGHT,
						 GO_STOP_FORWARD, GO_RIGHT, GO_LEFT, GO_FORWARD, GO_STOP};
unsigned int pos_index ;
unsigned char rangeSensorValue;
unsigned char leftSensorValue;
unsigned char middleSensorValue;
unsigned char rightSensorValue;

/*
#define WWW ( (leftSensorValue <= THRESHOLD) && (middleSensorValue <= THRESHOLD) && (rightSensorValue <=THRESHOLD))
#define BWB ( (leftSensorValue > THRESHOLD) && (middleSensorValue <= THRESHOLD) && (rightSensorValue >=THRESHOLD))
#define BBW ( (leftSensorValue > THRESHOLD) && (middleSensorValue > THRESHOLD) && (rightSensorValue <= THRESHOLD)) 
#define WBB ( (leftSensorValue <= THRESHOLD) && (middleSensorValue > THRESHOLD) && (rightSensorValue > THRESHOLD))
#define WWB ( (leftSensorValue <= THRESHOLD) && (middleSensorValue <= THRESHOLD) && (rightSensorValue > THRESHOLD))
#define BWW ( (leftSensorValue > THRESHOLD) && (middleSensorValue <=THRESHOLD) && (rightSensorValue <= THRESHOLD)) 
#define WBW ( (leftSensorValue <= THRESHOLD) && (middleSensorValue > THRESHOLD) && (rightSensorValue <= THRESHOLD)) 
#define BBB ( (leftSensorValue > THRESHOLD) && (middleSensorValue > THRESHOLD) && (rightSensorValue >=THRESHOLD)) 
*/
#define WWW ( (leftSensorValue < THRESHOLD) && (middleSensorValue < THRESHOLD) && (rightSensorValue < THRESHOLD))
#define BWB ( (leftSensorValue >= THRESHOLD) && (middleSensorValue < THRESHOLD) && (rightSensorValue >=THRESHOLD))
#define BBW ( (leftSensorValue >= THRESHOLD) && (middleSensorValue >= THRESHOLD) && (rightSensorValue < THRESHOLD)) 
#define WBB ( (leftSensorValue < THRESHOLD) && (middleSensorValue >= THRESHOLD) && (rightSensorValue >= THRESHOLD))
#define WWB ( (leftSensorValue < THRESHOLD) && (middleSensorValue < THRESHOLD) && (rightSensorValue >= THRESHOLD))
#define BWW ( (leftSensorValue >= THRESHOLD) && (middleSensorValue < THRESHOLD) && (rightSensorValue < THRESHOLD)) 
#define WBW ( (leftSensorValue < THRESHOLD) && (middleSensorValue >= THRESHOLD) && (rightSensorValue < THRESHOLD)) 
#define BBB ( (leftSensorValue >= THRESHOLD) && (middleSensorValue >= THRESHOLD) && (rightSensorValue >=THRESHOLD)) 

void waitUntilTableOrderCrossed();

typedef enum
{
	ACTION_LEFT,
	ACTION_RIGHT,
	ACTION_FORWARD,
	ACTION_SMOOTHRIGHT,
	ACTION_SMOOTHLEFT,
	ACTION_STOP
} action_t;

action_t lastAction = ACTION_STOP;
positionStatus_t lastPositionStatus = eNULL;
unsigned char index2Table[] = { 0, 0, 0, 8, 6, 4, 0, 1,2,3,0,5,7,9,0,0,0,0,0,0,0,0,0,0,0,0,0};


int main()
{
	lcd_port_config();
	lcd_init();
	adc_pin_config();
	adc_init();
	motion_pin_config();
	motion_timer5_init();
	led_config ();
	//led();
	//orderCheckList();
	//waitUntilTableOrderCrossed();
	//orderDetails();

	
	/*blinkRedLed();
	_delay_ms(500);

	blinkBlueLed();
	_delay_ms(500);
	blinkGreenLed();
	_delay_ms(500);
	buzzer();
	_delay_ms(100);
	
*/
	forward();
	velocity(0,0);

	int count = 0;
	int a=0 ,b=0, c =0;
	while(1)
	{


	  //Read the three white line sensor values.
	  //Read the front sharp sensor value.
  
	  //write code to implement white line following algorithm. 
	  //Refer to the tutorial uploaded on the portal.
  
	  //stop the robot if any obstacle is detected within 10 cm distance
	  //You can use lcd_print function to print the sensor values for 
	  // better debugging.

//#ifdef _DEBUG
	//lcd_cursor(2, 8);
  	//lcd_string("starti   ");
	//_delay_ms(1000);
//#endif

	readSensorValues();
	
		
//orderCheckList();
// Set the direction and velocity to start with
// Only motor controls are needed to follow the white line
	forward();
	velocity(0,0);


/* B W B */
	if (BWB)
	{

		lastPositionStatus = eBWB;
		lastAction = ACTION_FORWARD;
//Cannot get this
		moving_area = CENTRE;
		forward_vel();
	}

/* B B W */
	else if (BBW)
	{
		lastPositionStatus = eBBW;
		lastAction = ACTION_SMOOTHLEFT;
		smoothLeft();
		//followTrack(eBBW);
	}

/* W B B */
	else if (WBB)
	{
		lastPositionStatus = eWBB;
		lastAction = ACTION_SMOOTHRIGHT;
		smoothRight();
		//followTrack(eWBB);
	}

/* W W B */
	else if (WWB)
	{

		lastPositionStatus = eWWB;
		lastAction = ACTION_SMOOTHRIGHT;
		smoothRight();
		//followTrack(eWWB);
	}
/* B W W */
	else if (BWW)
	{
		lastPositionStatus = eBWW;
		lastAction = ACTION_SMOOTHLEFT;
		smoothLeft();
		//followTrack(eBWW);
	}

/* W B W */
	else if (WBW)
	{
		lastPositionStatus = eWBW;
		lastAction = ACTION_FORWARD;
		forward_vel();
		_delay_ms(FOWARD_DELAY);
	}

/* B B B */
	else if (BBB) 
	{
		lastPositionStatus = eBBB;
		{
			foundNode = 1;
			count = 0;
			stop();
			_delay_ms(2000);
			lcd_cursor(2, 8);
			lcd_string("index=");
			lcd_print(2, 15, pos_index, 2);
			_delay_ms(2000);
			forward();
			velocity(0,0);
				if (position[pos_index] == GO_FORWARD)
				{
					forward_vel();
					waitUntilNodeCrossed();
					lastAction = ACTION_FORWARD;
					_delay_ms(NODE_FOWARD_DELAY);//delay after node 00 which is fast.
				}

				else if (position[pos_index] == GO_LEFT)
				{

					if (foundNode == 1)
						moving_area = RIGHT;
					else 
						moving_area = LEFT;

					waitUntilLeftTurn();	

					lastAction = ACTION_LEFT;

				}
				else if (position[pos_index] == GO_RIGHT)
				{
					if (foundNode == 1)
						moving_area = LEFT;
					else 
						moving_area = RIGHT;
					waitUntilRightTurn();

					
					lastAction = ACTION_RIGHT;
		
				}
				else if (position[pos_index] == GO_STOP_FORWARD)
				{
					//stop();
					//_delay_ms(1000);
					forward();
					velocity(0,0);
					//forward_vel();
					//forward_slow();
//#ifdef _DEBUG
					lcd_cursor(2, 8);
  					lcd_string("enter1   ");
					_delay_ms(1000);

//#endif

					
					
					// Ordering only on specific nodes assigned as kitchen
					if ((pos_index == 18) || (pos_index == 20) || (pos_index == 22))
					{
						orderCheckList(pos_index);	
					}
				
					if ((pos_index == 3) || (pos_index == 4) || (pos_index == 5) || (pos_index == 7) || (pos_index == 8) || (pos_index == 9) || (pos_index == 11) || (pos_index == 12) || (pos_index == 13))
						{
						waitUntilTableOrderCrossed();
						}
						
				


//#ifdef _DEBUG
		   			lcd_cursor(2, 8);
  					lcd_string("RETURN   ");
					_delay_ms(1000);//low speeds at nodes
//#endif
					forward_vel();
					_delay_ms(NODE_FOWARD_DELAY_MEDIUM);// delay after node is sensed.
					
//#ifdef _DEBUG
					lcd_cursor(2, 8);
  					lcd_string("EXIT1    ");
					_delay_ms(1000);
//#endif
					lastAction = ACTION_FORWARD;
				}

				else if (position[pos_index] == GO_STOP)
				{

					lcd_cursor(1, 1);
					lcd_string("                 ");

					lcd_cursor(2, 1);
					lcd_string("                 ");


					orderDetails(&a,&b,&c);
					lcd_cursor(1, 1);
					lcd_string("A=");
					lcd_print(1, 3, a, 1);

					lcd_cursor(1,4);
					lcd_string("B=");
					lcd_print(1, 6, b, 1);


					lcd_cursor(1, 7);
					lcd_string("C=");
					lcd_print(1, 9, c, 1);
					buzzer();
					_delay_ms(8000);
					lcd_cursor(1, 1);
					lcd_string("                 ");

					lcd_cursor(2, 1);
					lcd_string("                 ");
				
					stop();
					_delay_ms(8000);
					return (1);


				}

				foundNode = 0;
				pos_index++;
				stop();
				lcd_cursor(2, 8);
				lcd_string("index=");
				lcd_print(2, 15, pos_index, 2);
				_delay_ms(2000);
				forward();
				velocity(0,0);



			}

			
		}

		/* W W W */
		else if (WWW)
		{

			unsigned char threeTimeCheck = 0;

			do
			{

				threeTimeCheck++;
				readSensorValues();
				_delay_ms(50);
	

			}
			while ((threeTimeCheck <=2) && (WWW));

			if (WWW && (threeTimeCheck > 2))
			{

				if ( (lastAction == ACTION_LEFT) || (lastAction == ACTION_SMOOTHLEFT))
					waitUntilRightTurnSmooth();
				else if ( (lastAction == ACTION_RIGHT) || (lastAction == ACTION_SMOOTHRIGHT))
					waitUntilLeftTurnSmooth();

				else if ((lastAction == ACTION_FORWARD))
				{
					forward_vel();
					_delay_ms(FOWARD_DELAY_WHITE);
					//tryLeftAndRightToBeOnTrack();

				}
			}

/*		if (moving_area == LEFT)
				waitUntilRightTurnSmooth();	
						
			else if (moving_area == RIGHT)
				waitUntilLeftTurnSmooth();	
				

			else if (moving_area == CENTRE)
				forward_vel();

			while (WWW)
			{
				readSensorValues();
			}

*/
			
			//forward_vel();
			//_delay_ms(FOWARD_DELAY_WHITE);


		}

		_delay_ms(75);
		stop();
		_delay_ms(WAIT_BEFORE_NEXT_STEP);


		
		


 	}

}




void waitUntilTableOrderCrossed()
{
  	readSensorValues();
	unsigned short int steps = 0;

	 int tableFound_a;

	// tables
	//if ((pos_index == 3) || (pos_index == 4) || (pos_index == 5) || (pos_index == 7) || (pos_index == 8) || (pos_index == 9) || (pos_index == 11) || (pos_index == 12) || (pos_index == 13))
	//{
		while ((tableFound_a = processTables(index2Table[pos_index]) == 0)  && (steps<=3))
		{	
///////////////////////////////////////////	
//#ifdef _DEBUG
			lcd_cursor(2, 8);
  			lcd_string("lopent   ");
			_delay_ms(1000);
//#endif
			forward();
			velocity(0,0);
//#ifdef _DEBUG
			//forward_slow();
		    lcd_cursor(2, 8);
  			lcd_string("XX=      ");
			lcd_print(2,11 ,steps, 1);
 			_delay_ms(1000);
//#endif
			forward_vel();
 			_delay_ms(STEPS_UNTIL_TABLE);

			stop();
			steps++;
			
//#ifdef _DEBUG
			lcd_cursor(2, 8);
  			lcd_string("loopout  ");
			_delay_ms(1000);
//#endif
			
			readSensorValues();
			
			
		
		}
		lcd_cursor(2, 8);
  		lcd_string("XX=      ");
		lcd_print(2,11 ,steps, 1);
 		_delay_ms(1000);

		lcd_cursor(2, 8);
  		if (tableFound_a ==0) 
			lcd_string("T_NF");
		else
			lcd_string("T_F");
 		_delay_ms(1000);

		if ( (tableFound_a == 1 ) && (steps==4))
		{
			lcd_cursor(2, 8);
  		    lcd_string("noorder");
			_delay_ms(1000);
			buzzer();
		}



	//}

}
	


	






	

