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
#ifndef PhysBotMotorServo_h
#define PhysBotMotorServo_h

#include <PhysBotLib.h>
#include <PhysBotKinematics.h>

enum LIMITMODE {DONE,TIME_LIMIT,DISTANCE_LIMIT,AUTOMATIC,SERVO_ONLY};

/***
 * PhysBotMotorServo provides a unification of the position sensing and timing
 * so that we can asynchronously control the bot.
 */
class PhysBotMotorServo : public PhysBotMotors, public PhysBotKinematics {

  public:
    PhysBotMotorServo();

	// Overrides
	void drive( int speed, WHEEL wheel );
	void drive( int speed, WHEEL wheel, float distanceInCm );
	void drive( int speed, WHEEL wheel, unsigned long delTimeInMs );

    // These routines set the upper limits on the robot's actions
    void setSpeedLimit( float speedInCmPerSec ); // Set speed to servo to.
    void setDistanceLimit( float distanceInCm );    // Set time limit before braking.
    void setTimeLimit( unsigned long delTimeInMs );  // Set distance limit before braking.

    void setMode( LIMITMODE setMode );  // Override the defaults above (use with care)

    void update();  // This is the routine you put in the loop function to
                    // check on the status of the end conditions and run the servo

    boolean isDone() { return _mode == DONE; } // tests if we are done.

    float getDistanceLimit() { return _distanceLimit; }
    unsigned long getTimeLimit() { return _timeLimit; }
    float getSpeedLimit() { return _speedLimit; }

  private:
    LIMITMODE   	_mode;          // current mode
    unsigned long   _timeLimit;     // time limit value
    unsigned long	_startTime;		// time to start
    float 			_distanceLimit;	// distance limit value
    float			_startPoint;	// place we begin.
    float 			_speedLimit;	// speed limit for servo
    long 			_lastTick;		// last tick we visited update

};



#endif
