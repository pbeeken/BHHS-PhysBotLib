/****************************************************************
Core header file for all the various PhysBot functions.

- From the original RedBot.h file:
There is additional license info below regarding the use of the
SoftwareSerial library from Arduino 1.0.5; I had good and sound
reasons for creating a derivative class rather than asking users
to simply use the existing library, which are documented below.

This code is beerware; if you use it, please buy me (or any other
SparkFun employee) a cold beverage next time you run into one of
us at the local.

21 Jan 2014- Mike Hord, SparkFun Electronics
Code developed in Arduino 1.0.5, on an SparkFun Redbot v12.

Extensive modification. Some classes were left as is, many more
added. Most were adjusted and rewritten in one form or another.
Many of the lowest level access routines and interrupt service
handlers were left pretty much, as is.  Higher level routines were
modified to match our specific needs and hardware.  Most importantly:
routines that previously BLOCKED are now gone.  BLOCKING means that for
a given method, when called, would hold the full attention of the Arduino
until it finished its task, thus blocking other routines from doing their
job.  In all classes which need to perform periodic tasks look for a
method called 'update'.  This is the method that needs to be called once
per 'loop()' to check to see if something needs to be done.  If not, that
object's update method returns quickly so other object update routines can
get on with their jobs as well.  Ideally we could register these things in a
scheduler class but that is for another day.
17 Aug 2015- P. Beeken, Byram Hils High School

Tested and developed in Arduino 1.6.5 on a SparkFun RedBot ROB-13166
all of our machines also come with wheel encoders.  They are not
supplied with whiskers or xBees. We are too cheap.
****************************************************************/

#ifndef PhysBotMotors_h
#define PhysBotMotors_h

#include <PhysBotLib.h>
#include <PhysBotEncoders.h>

// real speed values cannot be this high so this works as a message.

/** This class handles motor functionality. I expect only one instance of this
 *  in a thread.  We can manipulate both or each wheel independently given the parameters
 **/
class PhysBotMotors
{
  public:
    PhysBotMotors();          // Constructor. Mainly sets up pins.

    // Non blocking routines
    void drive( int speed, WHEEL wheel=BOTH );	// Drive in direction given by sign, at speed given by magnitude of the parameter.
    void pivot( int speed );  					// Pivot more or less in place. Turns motors in opposite directions

	// These are the stopping/halting routines
	void coast( WHEEL wheel=BOTH );				// Stop motors, but allow them to coast to a halt.
	static void brake( WHEEL wheel=BOTH );		// Quick-stop the motors, shorting the leads.  static so we can access outside

	// getters and setters
    int getSpeed( WHEEL wheel=BOTH );         // As we move to async routines we need to recall this value.
    void adjustSpeed( int percentage, WHEEL wheel=BOTH );	// the percentage to adjust the wheel speed.  (-100<->100)
    		// This is useful for tuning direction without overriding the speed entirely.  For example we can use
    		// these parameters to adjust for a slight drift or we can use this to turn the 'bot with a line
    		// following servo to turn the cart without changing the assigned speed.
    void turn( int percentage );  // we can use this to perform an adjusted turn of the amount given. -100<perc<100
            //  is the degree to drift left or right.
        // A link to this object.  There should be only one copy of this object as there is only one set
        // of motors.  This allows
    static PhysBotMotors* motorObj;

  private:
	void rightCoast();				// Stop right motor, as with coast().
	void leftCoast();				// Stop left motor, as with coast().
	static void leftBrake();		// Quick-stop left motor, as with brake().	static so we can access outside
	static void rightBrake();		// Quick-stop right motor, as with brake().	static so we can access outside

    void leftFwd(int speed);		// These functions are pretty self-explanatory,
    void leftRev(int speed);		//  and are called by the above functions once
    void rightFwd(int speed);		//  sign has been used to determine direction.
    void rightRev(int speed);

	int _leftSpeed;					// memory of the last speed setting
	int _rightSpeed;

	int _leftTweak;					// tweaks applied to speed to vary the wheels
	int _rightTweak;

};

#endif
