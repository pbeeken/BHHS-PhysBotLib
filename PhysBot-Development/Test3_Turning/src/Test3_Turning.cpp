/***********************************************************************
 * Exp3_Turning -- PhysBot Experiment 3
 *
 * Explore turning with the RedBot by controlling speed 'Tweaks'
 *
 * Hardware setup:
 * This code requires only the most basic setup: the motors must be
 * connected, and the board must be receiving power from the battery pack.
 *
 * 23 Sept 2013 N. Seidle/M. Hord
 * 04 Oct 2014 B. Huang
 * 17 Aug 2015 P. Beeken
 ***********************************************************************/
#include <Arduino.h>
#include <Streaming.h>
#include <PhysBotLib.h>  // This line "includes" the library into your sketch.
#include <PhysBotMotors.h>
#include <PhysBotEncoder.h>

PhysBotMotors motors; // Instantiate the motor control object.

int testCondition = 0;


void setup()
{
  Serial.begin( 115200 );
  Serial << "Welcome to experiment 3!" << endl;
  Serial << "------------------------" << endl;

}

void loop()
{
	switch( testCondition ) {

		case 2:
			// drive forward -- then use the adjust method to vary the speed a bit.
			Serial << "Both motors at same speed..." << endl;
			motors.drive(100); // Turn on the motors
			delay( 1000 );

			// The "turn" method tweaks the speed on each wheel to make the turn
			Serial << "Left is 25% slower and right is 25% faster." << endl;
			motors.turn( 50 );  // slow down right wheel 25%
			delay( 500 );

			// The "turn" method tweaks the speed on each wheel to make the turn
			Serial << "Right is 25% slower and left is 25% faster." << endl;
			motors.turn( -50 );  // slow down right wheel 25%
			delay( 500 );

			// Setting the value to 0 restores straightness.
			Serial << " -turn- " << endl;
			motors.turn( 0 ); // restore straightness
			delay( 1000 );

			Serial << "...stop..." << endl;
			motors.brake();
			break;

		case 1:
			// Figure 8
			Serial << "Figure 8..." << endl;
			for( int i=0; i<2; i++ ) {
				motors.drive( 100 ); // turn on motors
				delay( 100 );
				Serial << " -turn- " << endl;
				motors.turn( 80 );
				delay( 2000 );
				Serial << " -turn- " << endl;
				motors.turn( -80 );
				delay( 2000 );
				Serial << " -turn- " << endl;
				motors.turn(0);
				delay( 500 );
				motors.brake();
				}
			break;

		case 3:
			// Pivot
			Serial << "Pivot..." << endl;
			motors.pivot( 80 ); // turn on motors
			delay( 1000 );
			motors.brake();
			break;

		default:
			motors.coast();
			break;
		}

		delay( 3000 );
		testCondition++;

}
