

#define F_CPU 14745600
#define __OPTIMIZE__ -O0
#include <util/delay.h>
#include <avr/io.h>


static unsigned char buzzerPortInitialised = 0;


/////////////////////////////////BUZZER CONFIG////////////////////////

static void buzzer_config()
{
	DDRC |= 0x08;  // (appropriate value to set Port C pin 3 as output)
 	PORTC &=0xF7;  // (appropriate value to ensure that buzzer remains off initially)

}

void buzzer_on()
{
	if (buzzerPortInitialised == 0)
	{ 
		buzzer_config();
		buzzerPortInitialised = 1;

	}	
	// Intentionally, the port bits are read instead
	// of pins. Previous value of other port bits
	// are ensured to continue to be the same.
	// We do not see the real need of reading pins
	// and setting the port bits for this excercise
	PORTC |= 0x08;

}

void buzzer_off()
{

	if (buzzerPortInitialised == 0)
			{ 
				buzzer_config();
				buzzerPortInitialised = 1;

			}	
	// Intentionally, the port bits are read instead
	// of pins. Previous value of other port bits
	// are ensured to continue to be the same.
	// We do not see the real need of reading pins
	// and setting the port bits for this excercise
	PORTC &= 0x0F7;

}




void buzzer()
{
	
			if (buzzerPortInitialised == 0)
			{ 
				buzzer_config();
				buzzerPortInitialised = 1;

			}	
			buzzer_on();
			_delay_ms(500);
			buzzer_off();
			_delay_ms(500);
}

////////////////////////////////////////////////////////////////////////
