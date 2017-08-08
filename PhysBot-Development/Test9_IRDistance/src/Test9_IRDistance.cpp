/******************************************************************************
  IRDistance Test.ino

 No need for the overhead associated with a separate object since all we do is
 read a voltage.  The tricky part is converting the voltage to an actual distance.

  This code is released under the [MIT License](http://opensource.org/licenses/MIT).
  Please review the LICENSE.md file included with this example. If you have any questions
  or concerns with licensing, please contact techsupport@sparkfun.com.
  Distributed as-is; no warranty is given.
******************************************************************************/
#include <Streaming.h>

/*****
 * distInCm converts the reading to a number in cm
 ****/
int distInCm( int val ) {
	if ( val < 20 ) return 300;
	if ( val > 500 ) return 0;  // closest we can read.

	return (13000/val) - 5;
}

/****
 * setup the Arduino
 ***/
void setup() {
	Serial.begin( 115200 );

	Serial << endl << "IR Distance Test." << endl;
	pinMode( A0, INPUT );
}

/****
 * loop the Arduino forever.
 ***/
void loop() {

	// bigger number, shorter distance.
	int dist = analogRead( A0 );

	Serial << "d: " << dist << "  " << distInCm( dist ) << endl;

	delay( 500 );

}
