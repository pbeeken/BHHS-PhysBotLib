/****************************************************************
PhysBotBlinker

A convenience for setting up visual feedback, 'heart beats' etc.

17 Aug 2015- P. Beeken, Byram Hils High School

Tested and developed in Arduino 1.6.5 on a SparkFun RedBot ROB-13166
all of our machines also come with wheel encoders.  They are not
supplied with whiskers or xBees. We are too cheap.
****************************************************************/
#include <Arduino.h>
#include <PhysBotBlinker.h>

/** Constructor
 *	Sets the pin for the LED
 **/
PhysBotBlinker::PhysBotBlinker(int ledPin) {
  _ledPin = ledPin;
  pinMode(_ledPin, OUTPUT);
  _active = false;
  _blinkPeriod = 500;
  _change = millis() + _blinkPeriod;
  _ledIsOn = false;

}

/** update is to be placed in the loop() to follow through with the status changes
 *
 **/
void
PhysBotBlinker::update() {
	if ( _active )
		if ( millis() > _change ) {
			_ledIsOn = !_ledIsOn;
			digitalWrite( _ledPin, _ledIsOn );
			_change = millis() + _blinkPeriod;
		}
}

/*** setBlinkPeriod changes the blinking period (the time for the on and off semaphore)
 *	we could introduce a duty cycle parameter as well that will change the ratio of on
 *	to off period but that can be for another day.
 **/
void
PhysBotBlinker::setBlinkPeriod( unsigned long period ) {
	_blinkPeriod = period/2;
	_active = true;
  	_change = millis() + _blinkPeriod;
}

/** blinkFor is a blocking routine that blinks for a determined number of times.
 *	This can be great for giving feed back for a particular state of the robot.
 **/
void PhysBotBlinker::blinkFor( int nTimes ) {
	while( nTimes-- ) {
		digitalWrite( _ledPin, HIGH );
		delay( 300 );
		digitalWrite( _ledPin, LOW );
		delay( 300 );
	}
}
