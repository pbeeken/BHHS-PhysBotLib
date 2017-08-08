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

#ifndef PhysBotEncoders_h
#define PhysBotEncoders_h

#include <PhysBotLib.h>
#include <PhysBotEncoders.h>

#define COUNTS_PER_REV = 192;   // 4 pairs of N-S x 48:1 gearbox = 192 ticks per wheel rev

/** Encoder handler.  Like the motor handler above we are allowed only one instantiation
 * per running program.  It encapsulates all the methods and variables we need to manage
 * the information from the encoders.  It works closely with the motor instance to correctly
 * count the wheel movement.  If I were clever, I would actually subclass motor and make
 * a virtual method here that would override the ISR so we would track the movement.  Then
 * the PhysBotKinematics class which converts these actions into measurements would make more
 * sense.
 * From original file [RedBot.h]:
 * When a negative going edge happens on an encoder pin, a counter is
 * incremented (or decremented), depending on the direction last determined
 * by one of the motor direction commands.
 **/
class PhysBotEncoders
{
  // We declare a couple of friends, so they can have access to the private
  //  members of this class.
  friend class PhysBotMotors;  // Needs access to lDir and rDir.
  friend void pinFunctionHandler(int pinIndex); // Called from within the
                             //  ISRs, this function increments the counts
                             //  by calling wheelTick().
  public:
    PhysBotEncoders(int lPin=11, int rPin=10); // Constructor. Assigns pins, pin
                             //  functions, zeroes counters, and adds a
                             //  reference to the new encoder object for other
                             //  library members to access.
    void clear(WHEEL wheel=BOTH); // Zaps the encoder count for a given wheel (or
                             //  for both wheels).
    long getTicks(WHEEL wheel=BOTH); // Returns the encoder count for a wheel.
    unsigned long getPeriod(WHEEL wheel=BOTH); // Returns microseconds for a single encoder rotation
    unsigned long getTime(WHEEL wheel=BOTH); // Returns microseconds since last reset

  private:
  	static PhysBotEncoders* encoderObj;	// a pointer to the instance of this object for friends
										// this shouldn't be necessary if I were to declare wheelTick,
										// _lDir and _rDir as static.

    void wheelTick(WHEEL wheel);	// Increment or decrement a wheel's counts,
									//  depending on which way the motor is turning.
    long _lCounts, _rCounts;		// Holds the number of ticks for that wheel's encoder.
    char _lDir, _rDir;				// Direction is set by the motor class, according
									//  to what the most recent motion direction for
									//  the given wheel was.
    unsigned long _lTime, _rTime;	// The total time markers for the ticks
    unsigned long _slTime, _srTime;	// start time index for total time
    unsigned long _rPer, _lPer;		// The time in microseconds between revolution.
    unsigned long _sLp, _sRp;		// start markers for period.
};


#endif
