/***********************************************************************
 * This is the best way to control the position and motion of the robot
 * without blocking.
 ***********************************************************************/
#include <Streaming.h>
#include <PhysBot.h>
#include "PhysBotKinematics.h"
#include "PhysBotMotorServo.h"

PhysBotKinematics encoder(A2, 10);
PhysBotMotorServo motors;
PhysBotButton  button;
PhysBotBlinker  led;


void setup()
{
  Serial.begin(250000); // as fast as we can!
  while (!Serial);

  driveDistance( 150.0, 120 );
  
  Serial << "Press button to begin..." << endl;
}



void loop(void)
{
  if (button.readSinglePush())   {
    motors.setSpeedLimit( 50.0 );
    motors.setDistanceLimit( 50.0 );
    encoder.clearEnc();
    motors.drive( 127 );  // All ahead 1/2
    Serial << "go.." << endl;
  }

  motors.update();

  if ( motors.isDone() )
    motors.brake();

}

//  This storage game doesn't work either. we run out of storage.
//#define STORSIZE 50
//long period[STORSIZE];
//float distan[STORSIZE];
//long theTime[STORSIZE];
//float avgSpd[STORSIZE];
//float insSpd[STORSIZE];

// Drive for a distance and stop  NO SERVO control
void driveDistance(float distance, int motorPower)
{
  long lCount = 0;
  long wCounts = 0;
  long lastCount = 0;
  unsigned long timeAccum = 0L;
 
//  Serial << endl << "begin initialize..." << endl;
//  for( int i=0; i<STORSIZE; i++ ) {
//    period[i] = 0L;
//    theTime[i] = 0L;
//    distan[i] = 0.0;
//    avgSpd[i] = 0.0;
//    insSpd[i] = 0.0;
//  }
  
  float numRev = (float) distance / PhysBotKinematics::DIST_PER_REV;
  long totalTicks = numRev * PhysBotKinematics::PULSE_PER_REV;
  
  // debug report
  Serial << endl << "driveDistance() " << distance << " cm at " << motorPower << " power." << endl;
  Serial << " nr:" << _FLOAT(numRev, 2) << "\t tt:" << totalTicks << endl;
  Serial << " const: " << _FLOAT(PhysBotKinematics::DIST_PER_REV,1) << " " << PhysBotKinematics::DIST_PER_PULSE << endl;

  encoder.clearEnc();  // clear the encoder count
  motors.drive(motorPower); // apply power

  while ( wCounts < totalTicks )
  {   // while the encoder is less than the target count -- debug print

    wCounts = encoder.getTicks(BOTH);

    if ( lastCount != wCounts /*&& wCounts < STORSIZE*/ ) { // Only if the tick value has changed since the last visit.
        // We do this because 'printing' takes too long and drops points.
      timeAccum += encoder.getPeriod(BOTH) * ( wCounts - lastCount);
      Serial << "{ " << wCounts << ", " << 
           encoder.getPeriod(BOTH) << ", " <<
           _FLOAT( encoder.distanceTraveled( BOTH ), 1) << ", " << 
           encoder.getTime(BOTH) << ", " <<
          // timeAccum << ", " <<
           _FLOAT( encoder.intstantaneousSpeed(BOTH), 1  ) << ", " <<
           _FLOAT( encoder.averageSpeed(BOTH), 1  )  << "}," << endl;

//        period[wCounts] = encoder.getPeriod(BOTH);
//        distan[wCounts] = encoder.distanceTraveled( BOTH );
//        theTime[wCounts] = encoder.getTime(BOTH);
//        insSpd[wCounts] = encoder.intstantaneousSpeed(BOTH);
//        avgSpd[wCounts] = encoder.averageSpeed(BOTH);
        }
    lastCount = wCounts;
  }
  
  // now apply "brakes" to stop the motors.
  motors.brake();

//  // debug report
//  for( int i=0; i<lastCount; i++ ) {
//       //  OUTPUT operational data
//      Serial << "{ " << i << ", " << 
//           period[i] << ", " <<
//           _FLOAT( distan[i], 1) << ", " << 
//           theTime[i] << ", " <<
//           _FLOAT( insSpd[i], 1  ) << ", " <<
//           _FLOAT( avgSpd[i], 1  )  << "}," << endl;
//  }
}

// Servo routine
void driveDistanceAtSpeed(float distance, float atSpeed )
{
  int motorPower = 75; // starting point.
  long wCounts = 0;
  long lastCount = 0;
  long rPer = 0;
  int speedErrSignal = 0;

  float numRev = (float) distance / PhysBotKinematics::DIST_PER_REV;
  long totalTicks = numRev * PhysBotKinematics::PULSE_PER_REV;
  
  // debug report
  Serial << "driveDistance() " << distance << " cm at " << motorPower << " power." << endl;
  Serial << " nr:" << _FLOAT(numRev, 2) << "\t tt:" << totalTicks << endl;
  Serial << " const: " << _FLOAT(PhysBotKinematics::DIST_PER_REV,1) << " " << PhysBotKinematics::DIST_PER_PULSE << endl;

  encoder.clearEnc(BOTH);  // clear the encoder count
  motors.drive(motorPower); // apply power

  while (wCounts < totalTicks)
  {
    wCounts = encoder.getTicks(RIGHT);
    // only if count has changed
    if ( lastCount != wCounts ) {
      // while the left encoder is less than the target count -- debug print
      // the encoder values and wait -- this is a holding loop.
      rPer = encoder.getPeriod(RIGHT);

      float vi = encoder.intstantaneousSpeed(RIGHT);
      if ( wCounts > 3 ) { // start after one revolution
      speedErrSignal = (int) (atSpeed - vi); // rate adjustment
      motorPower += speedErrSignal/2; 
      motorPower = constrain(motorPower, -240, 240);
      motors.drive(motorPower);
    }
      //  OUTPUT operational data (focus on rightside only for the moment.)
      Serial << "{ " << wCounts << ", " << 
          speedErrSignal << ", " <<
          motorPower << ", " <<
          _FLOAT( encoder.distanceTraveled( RIGHT ), 1) << ", " << 
          encoder.getTime(RIGHT) << ", " <<
          _FLOAT( encoder.intstantaneousSpeed(RIGHT), 1  ) << ", " <<
          _FLOAT( encoder.averageSpeed(RIGHT), 1  )  << "}," << endl;
//      Serial << "l:" << lCount << "\t" << lDt << "\tr:" << lCount << "\t" << lDt ;
//      Serial << "\t" << _FLOAT( encoder.distanceTraveled( RIGHT ), 1) << "\t" << encoder.getTime(RIGHT);
//      Serial << "\tvi:" << _FLOAT( encoder.intstantaneousSpeed(RIGHT), 1  );
//      Serial << "\tva:" << _FLOAT( encoder.averageSpeed(RIGHT), 1  )  << endl;
    }
    lastCount = wCounts;
  }
  // now apply "brakes" to stop the motors.
  motors.brake();
}


// Naieve heatnbeat to make sure we are alive.
void heartBeat() {
  static boolean blink = true;
  static unsigned long start = 0L;
  if ( millis() > start + 500 ) {  // Heartbeat
    if ( blink ) digitalWrite( 13, HIGH );
    else digitalWrite( 13, LOW );
    blink = ! blink;
    start = millis();
  }
}
