/***
 * Kinemtics Code model
 */

#include <Streaming.h>
#include <PhysBot.h>
#include <PhysBotLineTracker.h>
#include <PhysBotKinematics.h>

PhysBotMotors   motors;
PhysBotLineTracker lineFollow( &motors );
// Tools
PhysBotButton   button;
PhysBotBlinker  led;

int buttonPress = 0;
unsigned long start = 0L;

/** NOTE to people familiar with programming in C++ and Java:
 *	Where is main?  { You know: public static void main(... or int main(... }
 *	It is hidden behind the scenes. That routine has been compiled and is ready
 *  to be linked with the two routines listed below.  The 'main' program consists
 *  simply as:
 |	int main() {
 |		setup();
 |		for(;;) {
 |			loop();
 |		}
 |		return 0;
 |	}
 *	So, the little Arduino never stops until you de-power it. Setup is called first to initialize
 *	the machine and then loop is called over and over.  You needn't put an infinite loop in loop 
 *	as it, is being called repeatedly.
 **/

//  SETUP all the initial conditions we need to run.
void setup() {
  // put your setup code here, to run once:
  Serial.begin( 57600 );
  led.activate();
  
  // Autolevel
  // Set the robot down on the line and tap the button to auto level.
  Serial << "-Set up on line, tap 'reset' then tap the Button-" << endl;
  lineFollow.autoCalibrate();

  while ( !button.readSinglePush() ); // wait for button press to continue

}


// LOOP this routine is called repeatedly to do something.
void loop() {

  led.update();  // provide a heartbeat to indicate we aren't stalled.

  if ( button.readSinglePush() ) {
    motors.brake();
    ++buttonPress;
    
    buttonPress = buttonPress % 10;
    led.blinkFor( buttonPress );

    start = millis();
  }

  lineFollow.followLine();
  update();

}


void update() {

  int spd;
  unsigned long deltaT = millis()-start;

  switch ( buttonPress ) {

    case 1: // Constant forward speed
      spd = 100;
      motors.drive( spd );
      if ( millis() > start + 2000 )
        motors.coast();
      break;

    case 2: // Constant forward speed x2
      spd = 200;
      motors.drive( spd );
      if ( millis() > start + 2000 )
        motors.coast();
      break;

    case 3: // Constant backward speed
      spd = -100;
      motors.drive( spd );
      if ( millis() > start + 2000 )
        motors.coast();
      break;

    case 4: // Constant backward speed x2
      spd = -200;
      motors.drive( spd );
      if ( millis() > start + 2000 )
        motors.coast();
      break;

    case 5: // starting from 0 constant forward acceleration
      // over 5 seconds go from 0 to 200
//      unsigned long delTime = millis() - start;
      spd = (200 * deltaT) / 5000;
      spd = constrain( spd, -250, 250);
      motors.drive( spd );
      if ( millis() > start + 5000 )
        motors.coast();
      break;

    case 6: // startig from -200 constant forward acceleration
      // over 5 seconds go from -200 to 200
//      unsigned long delTime = millis() - start;
      spd = -200 + (400 * deltaT) / 5000;
      spd = constrain( spd, -250, 250);
      motors.drive( spd );
      if ( millis() > start + 5000 )
        motors.coast();
      break;

    default:
      motors.brake();
      delay(30);  // settle the motors.
      break;
  }


}
