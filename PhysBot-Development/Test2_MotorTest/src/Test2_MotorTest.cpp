/***********************************************************************
 * Exp2_DriveForward -- RedBot Experiment 2
 *
 * Drive forward and stop.
 *
 * Hardware setup:
 * The Power switch must be on, the motors must be connected, and the board must be receiving power
 * from the battery. The motor switch must also be switched to RUN.
 *
 * 23 Sept 2013 N. Seidle/M. Hord
 * 04 Oct 2014 B. Huang
 ***********************************************************************/
#include <Arduino.h>
#include <PhysBotMotors.h>  // This line "includes" the RedBot library into your sketch.
#include <PhysBotEncoder.h>  // Our motors have encoders so we need this one also.
#include <Streaming.h>
// Provides special objects, methods, and functions for the RedBot.

PhysBotMotors motors; // Instantiate the motor control object. This only needs
// to be done once.

void setup()
{

  Serial.begin( 115200 );
  Serial << "Welcome to experiment 2!" << endl;
  Serial << "------------------------" << endl;

  Serial << " motors at 200! "<< endl;
  motors.drive(200);   // Turn on Left and right motors at full speed forward.
  delay(2000);         // Waits for 2 seconds
  motors.coast();       // Stops both motors

  Serial << " left motor at 200! "<< endl;
  motors.drive(200,LEFT);   // Turn on Left motors at full speed forward.
  delay(2000);         // Waits for 2 seconds
  motors.brake();       // Stops both motors

  Serial << " right motor at 200! "<< endl;
  motors.drive(200,RIGHT);   // Turn on Left motors at full speed forward.
  delay(2000);         // Waits for 2 seconds
  motors.coast();       // Stops both motors

  Serial << " motors in opposite directions 200! "<< endl;
  motors.pivot( 200 );
  delay(1500);
  motors.coast();


  Serial << "Speed test..." << endl;
	// drive forward at different speeds:
	for( int k=10; k<200; k+=10 ) {
		motors.drive( k );
		delay( 100 );
		Serial << "Moving at speed: " << (k) << endl;
		}
	// drive backward at different speeds:
	for( int k=10; k<200; k+=10 ) {
		motors.drive( -k );
		delay( 100 );
		Serial << "Moving at speed: " << (-k) << endl;
		}

	motors.brake();

}

void loop()
{
  // Nothing here. We'll get to this in the next experiment.
}
