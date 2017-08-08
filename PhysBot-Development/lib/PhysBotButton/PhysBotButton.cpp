/****************************************************************
Implementation for PhysBot button control.

This code is beerware; if you use it, please buy me (or any other
SparkFun employee) a cold beverage next time you run into one of
us at the local.

04 Oct 2014- B. Huang, SparkFun Electronics

Code developed in Arduino 1.0.6, on an SparkFun Redbot rev02

Code extensively modified to debounce the button push and provide
some utilities to detect multiple pushes since this is our only input mode

17 Aug 2015- P. Beeken, Byram Hils High School

Tested and developed in Arduino 1.6.5 on a SparkFun RedBot ROB-13166
all of our machines also come with wheel encoders.  They are not
supplied with whiskers or xBees. We are too cheap.
****************************************************************/
#include <Arduino.h>
#include <PhysBotButton.h>

#define BUTTON_PIN 12 // pretty much hardwired

/** Constructor. Mostly for pin setup; note that it's not necessary to configure
 *  PWM pins as they will be automatically configured with the analogWrite()
 *  function is called. No need to provide an alternate pin as this one is hard wired
 *  to the CB.
 */
PhysBotButton::PhysBotButton()
{
  // Sets the "default" state of the button to be HIGH.
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

#define WAIT_TIME 50
/** readSinglePush detects a button down and waits (for a short while) for the release
 * 					call does not block for more than WAIT_TIME in ms.
 * @return {boolean} whether the button was released in the requisite time
 */
boolean PhysBotButton::readSinglePush()
{
 	if ( buttonIsDown() ) { // button is down
 		// We might have a button push.
 		delay( 2 ); // wait for it...
 		if ( buttonIsDown() ) { 	// still down...
 			unsigned long stop = millis() + WAIT_TIME;
 			while( buttonIsDown() && (millis() < stop) ); 	// wait for release but not more than WAIT_TIME
 			return !buttonIsDown(); 	// return the state of the button now in case we timed out above.
 		}
 	}
   return false;
}

/** readDoublePush detects a double button tap.  Use this as a model to detect more.
 * @return {boolean} whether the button successfully generated a double tap.
 */
boolean PhysBotButton::readDoublePush()
{
 	return readPushes() == 2;
}

#define PUSH_WAIT	300  // human reaction time is, on average, about 200ms.  300 we'll allow 50% more.
/** readPushes counts pushes of the button by waiting for successive releases within a time interval
 * @return {int} the count of button releases (taps)
 */
int PhysBotButton::readPushes()
{
	int pushCount = 0;
	unsigned long stop;

	stop = millis()+PUSH_WAIT;  	// set the stop time
	while ( millis() < stop ) {  	// as long as we haven't reached the end
		if ( readSinglePush() ) { 	// we got a push within the time zone
			++pushCount;
			stop = millis()+PUSH_WAIT;  // reset the clock
			}
		}
	return pushCount;
}

/** Raw button read state.  A pushed button returns true, while a un-pushed button is false
 * @return {boolean} the current button state
 */
boolean PhysBotButton::buttonIsDown() {
	return !digitalRead(BUTTON_PIN);  // value is pulled high until it is pressed
}
