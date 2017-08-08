/******************************************************************************
  LineFollowingTest.ino
  No implementation of PID control.  This is a bang-bang servo

  This code is released under the [MIT License](http://opensource.org/licenses/MIT).
  Please review the LICENSE.md file included with this example. If you have any questions
  or concerns with licensing, please contact techsupport@sparkfun.com.
  Distributed as-is; no warranty is given.
******************************************************************************/
#include <Arduino.h>
//#include <Streaming.h>
#include <PhysBotLib.h>
#include <PhysBotSensorBar.h>

//const uint8_t SX1509_ADDRESS = 0x3E;  // SX1509 I2C address (00)

PhysBotSensorBar mySensorBar(SX1509_ADDRESS);

PhysBotMotors motors; // Instantiate the motor control object. This only needs
                      // to be done once.

#define CENTERED  (mySensorBar.getDensity()==2 && mySensorBar.getPosition()==0)
//#define REPORT
#define RUNNING_SPEED 95

//Define the states that the decision making machines uses:
enum {
  IDLE_STATE,
  FOUND_LINE,
  HUNT_LINE,
  GIVE_UP,
};

struct statusRec {
  int state = IDLE_STATE;
  int error = 0;
  int prop = 0;
  int integ = 0;
  int deriv = 0;
  unsigned long time = 0L;
};
struct statusRec gStatus;

int kP=2, kI=6, kD=6;
unsigned long startTime = 0L;

// Setup the conditions for the PID servo
void setup() {
#ifdef REPORT
  Serial.begin(115200);  // start serial for output
  Serial << endl << "Begin Line Following." << endl << endl;
#endif
  //Default: the IR will only be turned on during reads.
  //mySensorBar.setBarStrobe();
  //Other option: Command to run all the time
  mySensorBar.clearBarStrobe();

  //Default: dark on light
  mySensorBar.clearInvertBits();
  //Other option: light line on dark
  //mySensorBar.setInvertBits();
#ifdef REPORT
  Serial.setTimeout(5000);
  Serial << "Enter the 3 coefficients kP,kI,kD: ";
  int tkP = Serial.parseInt();
  int tkI = Serial.parseInt();
  int tkD = Serial.parseInt();
  kP = tkP == 0 ? kP : tkP;
  kI = tkI == 0 ? kI : tkI;
  kD = tkD == 0 ? kD : tkD;

  Serial << endl << "kP: " << kP << " kI: " << kI << " kD: " << kD << endl;
#endif
  //Don't forget to call .begin() to get the bar ready.  This configures HW.
  if ( mySensorBar.begin() )
  //  Serial << "sx1509 IC communication OK" << endl;
    delay(500);
  else {
  //  Serial << "sx1509 IC communication FAILED!" << endl;
    while (1);
    }

  // wait for robot to be centered on line.
  while( !CENTERED ) delay(50);
  //delay( 5000 );

  motors.drive(RUNNING_SPEED); // Start slow

  startTime = millis();
}

// Loop and build the correction signal
void loop() {

  struct statusRec nextStatus = gStatus;

  // read the number of sensors, decisions only make sense if I have more than
  // a few sensors activated. More than one or less than all.
  int dens = mySensorBar.getDensity();
  if ( dens > 0 && dens < 7 ) {
    // Get the position error signal
    nextStatus.state = FOUND_LINE;
    nextStatus.error = mySensorBar.getPosition();  // This is a direct error signal 0
    nextStatus.time = millis();  // record the time.

    int dt = nextStatus.time - gStatus.time; // delta time
    nextStatus.prop = nextStatus.error * kP/10;
    // we have been cheating up to now.  Since this loop has few branches the
    // ∆t has been the same.  In a more complex systtem we would need to multiply
    // the integrand by dt and divide the deriv by dt.
    nextStatus.integ = (nextStatus.integ + nextStatus.error) * kI/10;
    nextStatus.deriv = (nextStatus.error - gStatus.error) * kD/10;
    int corr = nextStatus.prop + nextStatus.integ + nextStatus.deriv;

#ifdef REPORT
    // Serial << millis() << " ";
    // Serial << " e: " << nextStatus.error;
    // Serial << " p: " << nextStatus.prop;
    // Serial << " i: " << nextStatus.integ;
    // Serial << " d: " << nextStatus.deriv;
    // Serial << " corr: " << corr;
    Serial << "{" << millis()-startTime;
    Serial << "," << nextStatus.error;
    Serial << "," << nextStatus.prop;
    Serial << "," << nextStatus.integ;
    Serial << "," << nextStatus.deriv;
    Serial << "," << corr;
#endif

    if ( abs(nextStatus.error) < 120 ) {

#ifdef REPORT
//    Serial << " T" << endl;
      Serial << ",'T'}" << endl;
#endif
      motors.turn( -corr );
    }
    else {
#ifdef REPORT
//    Serial << " P";
      Serial << ",'P'}";
#endif
      motors.drive(RUNNING_SPEED*60/100);  // Run at 60%
      motors.pivot( -corr );
      while( abs(mySensorBar.getPosition())>70 ) {
        delay(20); // wait until we find the line
#ifdef REPORT
//        Serial << ".";
#endif
      motors.drive(RUNNING_SPEED);  // restore speed
      }
#ifdef REPORT
//      Serial << "+" << endl;
#endif
    }

    gStatus = nextStatus;


  } else {

#ifdef REPORT
//    Serial << "All Stop!" << endl;
#endif
    motors.brake();
    // wait until we are lined up on the line again
    while( !CENTERED ) delay(50);
    motors.drive(RUNNING_SPEED);

  }



}
