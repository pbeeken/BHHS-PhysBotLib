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

#ifndef PhysBotBlinker_h
#define PhysBotBlinker_h


/** PhysBotBlinker is a convenience class to encapsulate the one easy visual 
 * feedback we have: pin 13, the built-in LED
 **/
class PhysBotBlinker
{
	public:
		PhysBotBlinker( int pin=13 );	// Constructor with led connected to pin.
		
		void update();				// placed in loop() to periodically update actions
		void setBlinkPeriod( unsigned long period=500 );	// set the blinking period
		void blinkFor( int nTimes );	// blink the led for a finite number (blocks)
		void activate() { _active = true; }		// turn on asynchronous routines
		void deactivate() { _active = false; }	// turn off asynchronous routines

	private:
		boolean _active;
		unsigned long _change;
		unsigned long _blinkPeriod;
		boolean _ledIsOn;
		int _ledPin;
};


#endif