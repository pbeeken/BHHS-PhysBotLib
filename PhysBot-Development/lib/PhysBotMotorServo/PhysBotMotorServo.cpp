/****************************************************************
Declaration for PhysBotMotorServo

Declaration for motor servo methods. Servo involves using feedback to limit
our robots actions. We can servo to time, distance or speed.  By reading sensors
we can use the information to control or limit the actions we take.  This is a vital
aspect of machine control.  When you set a thermostat in your house to a temperature
you expect your climate control to turn on when the temperature passes a setpoint in
one direction and off when the temperature goes in the other.  This is a classic 
"Bang-Bang" servo.  We do little more than that here.  Speed control can be more
sophisticated where if we are a little off from the set point we can gently nudge
things back.

17 Aug 2015- P. Beeken, Byram Hils High School

Tested and developed in Arduino 1.6.5 on a SparkFun RedBot ROB-13166
all of our machines also come with wheel encoders.  They are not 
supplied with whiskers or xBees. We are too cheap.
****************************************************************/
#include <Arduino.h>
#include <PhysBotMotorServo.h>
//#include <Streaming.h>

/*** 
 * Instantiate PhysBotServo a subclass of PhysBotMotors which can control limits without
 * blocking.  The idea is that a caller creates the object
 */
PhysBotMotorServo::PhysBotMotorServo() : PhysBotMotors(), PhysBotKinematics()
{
	_mode = DONE;
	_speedLimit = 0.0;  // No speed limit. Speed is whatever is set and not servoed.
}

void 
PhysBotMotorServo::drive( int speed, WHEEL wheel=BOTH )
{
	PhysBotMotors::drive( speed, wheel );
	_startTime = millis();  // note the time.
	_startPoint = distanceTraveled( BOTH );
}

void
PhysBotMotorServo::drive( int speed, WHEEL wheel, float distanceInCm )
{
	setDistanceLimit( distanceInCm );
	drive( speed, wheel );
}

void
PhysBotMotorServo::drive( int speed, WHEEL wheel, unsigned long delTimeInMs )
{
	setTimeLimit( delTimeInMs );
	drive( speed, wheel );
}


/*** 
 * Set the speed to servo to.
 */
void
PhysBotMotorServo::setSpeedLimit( float speedInCmPerSec )
{
	_speedLimit = speedInCmPerSec;
	_mode = SERVO_ONLY;
}

/*** 
 * Set the distance limit and set the limit flag to the Distance.
 */
void 
PhysBotMotorServo::setDistanceLimit( float distanceInCm )
{
	clearAll(); // clean the slate
	_distanceLimit = distanceInCm;
	_mode = DISTANCE_LIMIT;
}

/*** 
 * Set the time limit and set the limit flag to Time.
 */
void 
PhysBotMotorServo::setTimeLimit( unsigned long delTimeInMs )
{
	clearAll(); // clean the slate
	_timeLimit = delTimeInMs;
	_mode = TIME_LIMIT;
}

/*** 
 * Set the mode for limiting a use for this is to turn off limiting but maintain "SERVO_ONLY" or to choose
 * TIME or DISTANCE depending on which comes first: "AUTOMATIC"
 */
void
PhysBotMotorServo::setMode( LIMITMODE setMode )
{
	_mode = setMode;
}

/***
 * Loop call.  This routine is intended to be placed in the loop() method to periodically
 * check the current value against the settings.
 */
void
PhysBotMotorServo::update()
{
	if ( _mode == DONE ) return;  // we are done monitoring

	long tickCount = getTicks(BOTH);
	if ( _lastTick == tickCount ) return;	// nothing has moved since our last visit so exit
	_lastTick = tickCount; // mark when we were here last.

 	float instVel = intstantaneousSpeed( BOTH );

// #ifdef SERVO_SPEED  // don't bother if this isn't defined
// 	// Servo handler
// 	float instVel = PhysBotKinematics::kinematicsObj->intstantaneousSpeed( BOTH );
// 	// Right now this isn't working well because other parts are still evolving.  I will get back to this soon.
// 	if ( tickCount > 4 ) { // after one revolution...
// 		int errorSig = (int) (_speedLimit - instVel);
// 		int newSpeed = getSpeed( BOTH ) + (int) errorSig / 2;	// adjust by a little
// 		drive( newSpeed );
// 		}
// #endif

	unsigned long delT = millis()-_startTime;
	if ( (_mode == TIME_LIMIT || _mode == AUTOMATIC) && delT > _timeLimit ) {
		// DECISION TIME:  Do we want to stop or hold until another command comes along?
		//coast();  
		_mode = DONE;
		return;  // Cave Johnson, We're done here.
	}

	float delD = distanceTraveled( BOTH )-_startPoint;
	if ( (_mode == DISTANCE_LIMIT || _mode == AUTOMATIC) && delD > _distanceLimit ) {
		// DECISION TIME:  Do we want to stop or 		hold until another command comes along?
		//coast(); 
		_mode = DONE;
		return;  // Cave Johnson, We're done here.
	}

// The following is for diagnostics.
//	Serial << "{" << getTime(BOTH) << ", " << distanceTraveled( BOTH ) << ", " << instVel << "}," << endl; // DEBUG

}
