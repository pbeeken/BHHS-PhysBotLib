/****************************************************************
Implementation for PhysBot wheel encoder.

This code is beerware; if you use it, please buy me (or any other
SparkFun employee) a cold beverage next time you run into one of
us at the local.

21 Jan 2014- Mike Hord, SparkFun Electronics

Code developed in Arduino 1.0.5, on an SparkFun Redbot v12.

Code modified to track speeds of the wheels.  This entails keeping
track of the time in µs of the ticks.  NOTE: the hall effect encoders
while beautifully economical are not uniform in their transitions.
We need to track every 4 pulses to insure we are talking about a single
magnetic encoder rotation.  See notes on WheelTick for more detail. I
also, perhaps unnecessarily, track timing on each wheel separately.  Issuing
BOTH to the getter functions usually returns the average which, in most
cases, is the same value.  It is also important to note that we use the
microsecond timer. The µs timer has a rollover condition of about 56 min
(from the documentation).  I don't expect anyone is running a robot for
an hour, the batteries would die long before then.

17 Aug 2015- P. Beeken, Byram Hills School District

Tested and developed in Arduino 1.6.5 on a SparkFun RedBot ROB-13166
all of our machines also come with wheel encoders.  They are not
supplied with whiskers or xBees. We are too cheap.
****************************************************************/
#include <Arduino.h>
#include <PhysBotEncoders.h>

PhysBotEncoders* PhysBotEncoders::encoderObj=0L;

/** Constructor.  lPin and rPin are set by default values to the pins that are recommended
 * for that connection.
 */
PhysBotEncoders::PhysBotEncoders(int lPin, int rPin)
{
	/** PhysBot only breaks out ten valid pins:
	 *  A0-A5 a.k.a. D14-19 (PCINT 8-13)
	 *  D3 (PCINT 19)
	 *  D9-D11 (PCINT 1-3)
	 * We'll need a whopping case statement to set up the pin change interrupts
	 *  for this; in fact, we'll need two, but I'll abstract it to a function.
	 *  A call to setPinChangeInterrupt() enables pin change interrupts for that
	 *  pin, and pin change interrupts for the group that pin is a part of.
	 */
	pinMode(lPin, INPUT_PULLUP);
	pinMode(rPin, INPUT_PULLUP);
	setPinChangeInterrupt(lPin, LENCODER);
	setPinChangeInterrupt(rPin, RENCODER);
	_lCounts = 0;
	_rCounts = 0;
	encoderObj = this;	// We want a local pointer to the class member that is
						//  instantiated in the sketch, so we can manipulate its
						//  private members with other classes.
	_lDir = 1;		// default direction to forward -- used for encoder counting
	_rDir = 1;		// default direction to forward -- used for encoder counting
	// clear the timers
	clear( BOTH );
}

/** wheelTick is a private function which records the ticks and times. Direction is set by motor
 * object.  We do have an issue with the encoders that is best handled here. The encoder disks
 * have 4 poles which trigger transitions from the Hall probes.  The problem is that they aren't
 * exactly evenly spaced which results in quite a bit of fluctuation in the results (see graphs)
 * As a result we only clock a full rotation (every 4 ticks) of the encoder wheel.  This gives a
 * much smoother graph of the instantaneous velocity.
 * @param {WHEEL} the wheel to track.
 **/
void
PhysBotEncoders::wheelTick(WHEEL wheel)
{
  switch( wheel ) {
    case LEFT:
      _lCounts += (long)_lDir;
//      if ( (_lCounts % 4) == 1 ) {   // This insures we are only clocking the whole rotation (after 1 tick)
        _lPer = micros()-_sLp;		// time for a full encoder wheel rotation.
        _sLp = micros();				// reset period start timer
//        }
      _lTime = micros();				// update current time
      break;

    case RIGHT:
      _rCounts += (long)_rDir;
//      if ( (_rCounts % 4) == 1 ) {   // This insures we are only clocking the whole rotation (after 1 tick)
        _rPer = micros()-_sRp;		// time for a full encoder wheel rotation.
        _sRp = micros();				// reset period start timer
//        }
      _rTime = micros();				// update current time
      break;

    case BOTH:	   // never happens
      break;
  }
}

/**	clear clears the encoder counts and times.  Reset
 * @param {WHEEL} the wheel to track.
 **/
void
PhysBotEncoders::clear(WHEEL wheel)
{
  switch( wheel ) {
    case LEFT:
      _lCounts = 0;
      _lTime = micros();
      _sLp = _lTime;
      _lPer = 0L;
	  _slTime = micros(); //Start time
      break;

    case RIGHT:
      _rCounts = 0;
      _rTime = micros();
      _sRp = _rTime;
      _rPer = 0L;
	  _srTime = micros(); //Start time
      break;

    case BOTH:
      clear(LEFT);
      clear(RIGHT);
      break;
  }

}

/**	getTicks reads the encoder counts for a given wheel
 * @param {WHEEL} the wheel to track.
 **/
long
PhysBotEncoders::getTicks(WHEEL wheel)
{
  switch(wheel) {
    case LEFT:
      return _lCounts;

    case RIGHT:
      return _rCounts;

    case BOTH:
      return (getTicks(LEFT)+getTicks(RIGHT))>>1; // average.
  }

  return 0;
}

/***
 * getPeriod read the time in µs for the last encoder tick.   (Inst. Speed is wheel circum/this #)
 * @param {WHEEL} the wheel to track.
 **/
unsigned long
PhysBotEncoders::getPeriod(WHEEL wheel)
{
  switch(wheel) {
    case LEFT:
  		return _lPer;

    case RIGHT:
  		return _rPer;

    case BOTH:
      return (getPeriod(LEFT)+getPeriod(RIGHT))>>1; // average.
  }
  return 0;
}

/***
 * getTime read the time in µs since the last reset (clear).  (Avg. Speed is wheel circum * rotations/this #)
 * @param {WHEEL} the wheel to track.
 **/
unsigned long
PhysBotEncoders::getTime(WHEEL wheel)
{
  switch(wheel) {
    case LEFT:
		return _lTime-_slTime;

    case RIGHT:
		return _rTime-_srTime;

    case BOTH:
      return (getTime(LEFT)+getTime(RIGHT))>>1; // average.
  }
  return 0;
}
