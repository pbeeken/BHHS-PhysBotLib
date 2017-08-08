/****
 * This is a crude run at developing a line follower on the SparkFun RedBot
 */
#include <Streaming.h>
#include <PhysBot.h>
#include <PhysBotLineTracker.h>

PhysBotMotors motors;
PhysBotButton button;

PhysBotLineTracker lineFollow( &motors );


int count = 0;
unsigned long startTime;


int theSpeed = 150;  // tweak these to go straight


void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  Serial <<  endl << "IR Sensor Following Test (" << __TIMESTAMP__ << ")" << endl;
  
  // Autolevel
  // Set the robot down on the line and tap the button to auto level.
  lineFollow.autoCalibrate();

  while ( !button.readSinglePush() ); // wait for button press
  startTime = millis();
  motors.drive( theSpeed ); // both speeds are the same at this point.
}



void loop() {

#ifdef DEBUG_EVAL
  if ( millis() > startTime + 200 ) { // Slow Down for debugging
#endif

  lineFollow.followLine();

#ifdef DEBUG_EVAL
    startTime = millis();
  }
#endif

}





