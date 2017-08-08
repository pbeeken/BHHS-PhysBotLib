/***
 *  Handler for motor servo
 */

#include "PhysBotMotorServo.h"
#include <Arduino.h>
#include <Streaming.h>

//extern PhysBotKinematics *kinematicsObject;

/*** 
 * Instantiate PhysBotServo a subclass of PhysBotMotors which can control limits without
 * blocking.  The idea is that a caller creates the object
 */
PhysBotMotorServo::PhysBotMotorServo() : PhysBotMotors()
{
	mode = DONE;
	speedLimit = 0.0;  // No speed limit. Speed is whatever is set and not servoed.
}

/*** 
 * Set the speed to servo to.
 */
void
PhysBotMotorServo::setSpeedLimit( float speedInCmPerSec )
{
	speedLimit = speedInCmPerSec;
}

/*** 
 * Set the distance limit and set the limit flag to the Distance.
 */
void 
PhysBotMotorServo::setDistanceLimit( float distanceInCm )
{
	distanceLimit = distanceInCm;
	mode = DISTANCE_LIMIT;
}

/*** 
 * Set the time limit and set the limit flag to Time.
 */
void 
PhysBotMotorServo::setTimeLimit( unsigned long delTimeInMs )
{
	timeLimit = delTimeInMs;
	mode = TIME_LIMIT;
}

/*** 
 * Set the mode for limiting a use for this is to turn off limiting but maintain "SERVO_ONLY" or to choose
 * TIME or DISTANCE depending on which comes first "AUTOMATIC"
 */
void
PhysBotMotorServo::setMode( LIMITMODE setMode )
{
	mode = setMode;
}

/***
 * Loop call.  This routine is intended to be placed in the loop() method to periodically
 * check the current value against the settings.
 */
void
PhysBotMotorServo::update()
{
	if ( mode == DONE ) return;  // we are done monitoring

	if ( PhysBotKinematics::kinematicsObject == 0 ) return;	// no pointer available to kinematicsControl

	long tickCount = PhysBotKinematics::kinematicsObject->getTicks(BOTH);
	if ( lastTick == tickCount ) return;	// nothing has moved since our last visit so exit

	if ( tickCount == 0 ) { // haven't gone anywhere yet.
		drive( 127 );  // All ahead 1/2
	}

	lastTick = tickCount; // mark when we were here last.
	// Servo handler
	float instVel = PhysBotKinematics::kinematicsObject->intstantaneousSpeed( BOTH );

#ifdef SERVO_SPEED  // don't bother if this isn't defined
	if ( tickCount > 4 ) { // after one revolution...
		int errorSig = (int) (speedLimit - instVel);
		int newSpeed = getSpeed( BOTH ) + (int) errorSig / 2;	// adjust by a little
		drive( newSpeed );
		}
#endif

	unsigned long now = millis();

	if ( (mode == TIME_LIMIT || mode == AUTOMATIC) && now > timeLimit ) {
		// DECISION TIME:  Do we want to stop or hold until another command comes along?
		//coast();  
		mode = DONE;
		return;  // We're done here.
	}

	float here = PhysBotKinematics::kinematicsObject->distanceTraveled( BOTH );

// The following is for diagnostics.
//	Serial << "{" << PhysBotKinematics::kinematicsObject->getTime(BOTH) << ", " << here << ", " << instVel << "}," << endl; // DEBUG

	if ( (mode == DISTANCE_LIMIT || mode == AUTOMATIC) && here > distanceLimit ) {
		// DECISION TIME:  Do we want to stop or 		hold until another command comes along?
		//coast(); 
		mode = DONE;
		return;  // We're done here.
	}

}
