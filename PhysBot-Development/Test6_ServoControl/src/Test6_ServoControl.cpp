/***********************************************************************
 * Test6_ServoControl -- PhysBot Experiment 7.2
 *
 * The kinematics object provides an essential conversion of the encoders
 * to real world values. Here we can use those values to limit the motion
 * of the robot.
 *
 * This sketch was originally written by SparkFun Electronics, with lots of
 * help from the Arduino community. This code is completely free for any use.
 * It was modified extensively to test and demonstrate the kinematics and servo
 * objects.
 *
 * 8 Oct 2013 M. Hord
 * Revised, 31 Oct 2014 B. Huang
 * Rewritten, 19 Aug 2015 P. Beeken
 ***********************************************************************/
#include <Streaming.h>
#include <PhysBotLib.h>
#include <PhysBotBlinker.h>
#include <PhysBotButton.h>
#include <PhysBotMotorServo.h> // pulls in PhysBotKinematics

//PhysBotKinematics sensors;
PhysBotMotorServo servo;

PhysBotBlinker led;
PhysBotButton button;

void setup()
{
  Serial.begin(115200);
  led.activate();  // Activate the heartbeat
  Serial << endl << "Servo Control Test. Press the button to begin..." << endl;
}


int runCount = 0;
float	startLoc;
unsigned long startTime;

void loop(void)
{
  if ( button.readSinglePush() ) { // wait for button press
  	++runCount;
  	if ( runCount == 1 ) {
		servo.setDistanceLimit( 75. );  // drive 75 cm
		servo.drive( 150, BOTH );
		Serial << "Drive! " << _FLOAT(servo.getDistanceLimit(),1) << " cm at " << servo.getSpeed() << endl;
		}
	if ( runCount == 2 ) {
		 servo.setTimeLimit( 2000 );  // drive for 2 seconds
		 servo.drive( 180, BOTH );
		 Serial << "Drive! " << _FLOAT(servo.getTimeLimit()/1000.0,1) << " s at " << servo.getSpeed() << endl;
		}
	if ( runCount == 3 ) {
		 servo.setDistanceLimit( 50. );  // drive for 50 cm
		 servo.drive( 80, BOTH );
		 Serial << "Drive! " << _FLOAT(servo.getDistanceLimit(),1) << " cm at " << servo.getSpeed() << endl;
		}
	if ( runCount == 4 ) {
		 servo.setTimeLimit( 1000 );  // drive for 1 seconds
		 servo.drive( 200, BOTH );
		 Serial << "Drive! " << _FLOAT(servo.getTimeLimit()/1000.0,1) << " s at " << servo.getSpeed() << endl;
		}
	runCount *= 10; // flag the count
  }

  servo.update();  // update servo feed
  led.update();		// heartbeat

  if ( servo.isDone() && runCount > 9 ) {
  	servo.brake();
  	float delDist = servo.distanceTraveled( BOTH );
  	float delTime = servo.getTime(BOTH)/1000000.;
  	Serial << "I drove for " << _FLOAT( delDist, 1 ) << " cm taking " << _FLOAT(delTime,3) << " s."<<endl;
	runCount = runCount / 10; // restore the original count
	runCount %= 4;  // truncate the count
  	}


}
