/****************************************************************
Implementation for PhysBot motor control.

This code is beerware; if you use it, please buy me (or any other
SparkFun employee) a cold beverage next time you run into one of
us at the local.

21 Jan 2014- Mike Hord, SparkFun Electronics

Code developed in Arduino 1.0.5, on an SparkFun Redbot v12.

Modified code to allow for asynchronous operation.  Removed redundant calls
to simplify code which I can get away with since I am renaming these classes.
Included utilities to allow for line following and servo operations.

17 Aug 2015- P. Beeken, Byram Hills School District

Tested and developed in Arduino 1.6.5 on a SparkFun RedBot ROB-13166
all of our machines also come with wheel encoders.  They are not
supplied with whiskers or xBees. We are too cheap.
****************************************************************/
#include <Arduino.h>
#include <PhysBotMotors.h>
//#include <Streaming.h>

// Pin aliases for the motor controller.
#define    L_CTRL_1   2
#define    L_CTRL_2   4
#define    PWM_L      5

#define    R_CTRL_1   7
#define    R_CTRL_2   8
#define    PWM_R      6

/** Constructor. Mostly for pin setup; note that it's not necessary to configure
 *  PWM pins as they will be automatically configured with the analogWrite()
 *  function is called.
 */
//static PhysBotMotors* motorObj = 0L;

PhysBotMotors::PhysBotMotors()
{
  /** The interface to the motor driver is kind of ugly. It's three pins per
   * channel: two that define role (forward, reverse, stop, brake) and one
   * PWM input for speed.
   */
  pinMode(R_CTRL_1, OUTPUT);
  pinMode(R_CTRL_2, OUTPUT);
  pinMode(L_CTRL_1, OUTPUT);
  pinMode(L_CTRL_2, OUTPUT);
  _leftSpeed = 0;	_leftTweak = 0;
  _rightSpeed = 0;	_rightTweak = 0;

//  motorObj = this;	// We want a local pointer to the class member that is
            //  instantiated in the sketch, so we can get access to its
            //  methods using other

}

/** coast allows the motors to stop unimpeded by magnetic fields. The function opens the
 * circuit to allow the motors to coast. Pragmatically with the 48:1 gear ratio the physical
 * friction will bring the 'bot to a stop pretty quickly.
 * @param {WHEEL} wheel is the wheel to operate upon. use BOTH to indicate both wheels.
 */
void
PhysBotMotors::coast(WHEEL wheel)
{
	switch( wheel ) {
		case LEFT:
			leftCoast();
			break;
		case RIGHT:
			rightCoast();
			break;
		case BOTH:
			coast(RIGHT);
			coast(LEFT);
			break;
	}
}

/** brake allows the motors to stop dynamically by shorting the ends of the motor coils together.
 * the energy of the magnetic field is essentially shunted through a very low resistance causing
 * the motor to stop quickly.
 * @param {WHEEL} wheel is the wheel to operate upon. use BOTH to indicate both wheels.
 */
void
PhysBotMotors::brake(WHEEL wheel)
{
 	switch( wheel ) {
		case LEFT:
			leftBrake();
			break;
		case RIGHT:
			rightBrake();
			break;
		case BOTH:
			brake(RIGHT);
			brake(LEFT);
			break;
	}
}

/** drive() starts the motors. It figures out whether the motors should go
 * forward or reverse, then calls the appropriate individual functions. Note
 * the use of a 16-bit integer for the speed input; an 8-bit integer doesn't
 * have the range to reach full speed. The calls to the actual drive functions
 * are only 8-bit, since we only have 8-bit PWM.
 * @param {WHEEL} wheel is the wheel to operate upon. use BOTH to indicate both wheels.
 * @param {int} speed is the intended speed of the motor (-255 <-> 255) if the value
 *              is RESET then the original speeds are restored after a "Tweak"
 **/
void
PhysBotMotors::drive( int speed, WHEEL wheel )
{
	switch( wheel ) {
		case RIGHT:
				if ( speed != RESET )
					_rightSpeed = speed;
				else
					speed = _rightSpeed;
				speed += _rightTweak;
				speed = constrain( speed, -255, 255 );
//				Serial << "   RGHT: " << speed << endl;
				if (speed > 0) {
					rightFwd((char)(abs(speed)));
				} else {
					rightRev((char)(abs(speed)));
				}
				break;
		case LEFT:
				if ( speed != RESET )
					_leftSpeed = speed;
				else
					speed = _leftSpeed;
				speed += _leftTweak;
				speed = constrain( speed, -255, 255 );
//				Serial << "   LEFT: " << speed << endl;
				if (speed > 0) {
					leftFwd((int)(abs(speed)));
				} else {
					leftRev((int)(abs(speed)));
				}
				break;
		case BOTH:
				drive( speed, RIGHT );
				drive( speed, LEFT );
				break;
	}
}

/** turn() executes a turn by "adjusting" the wheel speeds given by the amount.
 * @param {int} amount is the intended turning degree of the motor (-100 <-> 100) is
 *                     the % of turn. This allows us to steer hard or soft based on input.
 **/
void
PhysBotMotors::turn( int amount ) {

	int adj = amount/2;

	if ( abs(adj)>100 ) return;  // Do nothing, bad parameter.

	if ( adj == 0 ) {
		adjustSpeed( 0, BOTH );
		}
	else if ( adj < 0 ) { // Turn Left
    	adjustSpeed( adj, RIGHT );
		adjustSpeed( -adj, LEFT );
	}
	else if ( adj > 0 ) { // Turn Right
    	adjustSpeed( adj, RIGHT );
		adjustSpeed( -adj, LEFT );
	}


}

/**	same thing as drive but only operate for a duration of 'duration'.  Will block for this period
 *  @param {WHEEL} wheel is the wheel to operate upon. use BOTH to indicate both wheels.
 *  @param {int} speed is the intended speed of the motor (-255 <-> 255)
 *  @param {int} duration is the time (in ms) the motor should spin. Will block for this period.
 * I have killed all the blocking routines.
 **/
// void PhysBotMotors::drive( int speed, int duration, WHEEL wheel )
// { // this variant of drive() integrates a delay duration to allow for single commmand instruction.
//   drive( wheel, speed );
//   delay(duration);
//   coast( wheel );
// }


/** pivot() is very much like drive(), except the motors are driven in opposite
 * directions, so as to pivot the motor on it's central axis. Positive numbers
 * turn / rotate the robot clockwise (to the right) -- assuming the motors are hooked up properly.
 * @param {int} speed is the intended speed of the motor (-255 <-> 255)
 **/
void
PhysBotMotors::pivot(int speed)
{
  if (speed < 0)
  {
    leftFwd((int)(abs(speed)));
    rightRev((int)(abs(speed)));
  }
  else
  {
    leftRev((int)(abs(speed)));
    rightFwd((int)(abs(speed)));
  }
}

/**	same thing as pivot() but only operate for a duration of 'duration'.  Will block for this period
 *  @param {int} speed is the intended speed of the motor (-255 <-> 255)
 *  @param {int} duration is the time (in ms) the motor should spin. Will block for this period.
 * I have killed all the blocking routines.
 **/
// void PhysBotMotors::pivot(int speed, int duration)
// {
//   pivot( speed );
//   delay(duration);
//   coast( BOTH );
// }

/** A simple getter for the last set speed for asynchronous routines to interrogate
 *  @param {WHEEL} wheel is the wheel to operate upon. use BOTH to indicate both wheels.
 **/
int
PhysBotMotors::getSpeed( WHEEL wheel )
{
  switch(wheel)
  {
    case LEFT:
      return _leftSpeed;
      break;
    case RIGHT:
      return _rightSpeed;
      break;
    case BOTH:
      return (_rightSpeed+_leftSpeed)>>1;
      break;
  }
  return 0;
}

/** adjustSpeed to set the tweak to adjust the speeds without affecting the initial set speed
 * for turning or adjusting the relative speed.  Setting the value to 0 removes the adjustment.
 * values < 0 slow that speed down, values > 0 speed up.
 **/
void
PhysBotMotors::adjustSpeed( int percentage, WHEEL wheel ) {
	switch( wheel ) {
	case RIGHT:
				_rightTweak = (_rightSpeed * percentage) / 100;
				drive( RESET, RIGHT );
				break;
	case LEFT:
				_leftTweak = (_leftSpeed * percentage) / 100;
				drive( RESET, LEFT );
				break;
	case BOTH:
				adjustSpeed( percentage, RIGHT );
				adjustSpeed( percentage, LEFT );
				break;
	}

}


/******************************************************************************
Private functions for PhysBotMotor
******************************************************************************/
/** These are the motor-driver level abstractions for turning a given motor the
 *  right direction. Users never see them, and *should* never see them, so we
 *  make them private.
 * From here out, we deal with the nitty gritty details of telling the motor
 *  driver what to do. For more information about this, refer to the TB6612FNG
 *  datasheet.
 */
void
PhysBotMotors::leftBrake()
{
  digitalWrite(L_CTRL_1, HIGH);
  digitalWrite(L_CTRL_2, HIGH);
  analogWrite(PWM_L, 0);
}

void
PhysBotMotors::rightBrake()
{
  digitalWrite(R_CTRL_1, HIGH);
  digitalWrite(R_CTRL_2, HIGH);
  analogWrite(PWM_R, 0);
}

void
PhysBotMotors::leftCoast()
{
  digitalWrite(L_CTRL_1, LOW);
  digitalWrite(L_CTRL_2, LOW);
  analogWrite(PWM_L, 0);
}

void
PhysBotMotors::rightCoast()
{
  digitalWrite(R_CTRL_1, LOW);
  digitalWrite(R_CTRL_2, LOW);
  analogWrite(PWM_R, 0);
}


void
PhysBotMotors::leftFwd(int spd)
{
  digitalWrite(L_CTRL_1, HIGH);
  digitalWrite(L_CTRL_2, LOW);
  analogWrite(PWM_L, spd);
  // If we have an encoder in the system, we want to make sure that it counts
  //  in the right direction when ticks occur.
  if (PhysBotEncoders::encoderObj != 0) {
    PhysBotEncoders::encoderObj->_lDir = 1;
  	}
}

void
PhysBotMotors::leftRev(int spd)
{
  digitalWrite(L_CTRL_1, LOW);
  digitalWrite(L_CTRL_2, HIGH);
  analogWrite(PWM_L, spd);
  // If we have an encoder in the system, we want to make sure that it counts
  //  in the right direction when ticks occur.
  if (PhysBotEncoders::encoderObj != 0) {
    PhysBotEncoders::encoderObj->_lDir = -1;
  	}
}

void
PhysBotMotors::rightFwd(int spd)
{
  digitalWrite(R_CTRL_1, HIGH);
  digitalWrite(R_CTRL_2, LOW);
  analogWrite(PWM_R, spd);
  // If we have an encoder in the system, we want to make sure that it counts
  //  in the right direction when ticks occur.
  if (PhysBotEncoders::encoderObj != 0) {
    PhysBotEncoders::encoderObj->_rDir = 1;
  	}
}

void
PhysBotMotors::rightRev(int spd)
{
  digitalWrite(R_CTRL_1, LOW);
  digitalWrite(R_CTRL_2, HIGH);
  analogWrite(PWM_R, spd);
  // If we have an encoder in the system, we want to make sure that it counts
  //  in the right direction when ticks occur.
  if (PhysBotEncoders::encoderObj != 0) {
    PhysBotEncoders::encoderObj->_rDir = -1;
	}
}
