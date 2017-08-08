/***
 *  Handler for motor kinematics.  There is much evil we need to deal with
 *  there is a lot of variation in the encoder data so we need to smooth the values
 *  When starting up the motors are not representing the speed well.  We also want
 *  want to be able to control the acceleration.
 */

#include "PhysBotKinematics.h"
#include <Arduino.h>

#define MEGA 1.0E6

PhysBotKinematics *kinematicsObject=0;	// Create a local pointer to an instance of this
										//  class, so we can edit the counts with other
										//  library functions.

  // Thse constants are used throughout the code. We cache them here.
long PhysBotKinematics::PULSE_PER_REV = GEAR_RATIO * MAG_POLES;
float PhysBotKinematics::DIST_PER_REV = WHEEL_DIA * PI;
float PhysBotKinematics::DIST_PER_PULSE = DIST_PER_REV / PULSE_PER_REV;

// Instantiate this subclass by passing the pins to the encoder object
PhysBotKinematics::PhysBotKinematics(int lPin, int rPin) : PhysBotEncoder(lPin, rPin)
{
	kinematicsObject = this; // set the pointer so we can get access to the public functions.
}

// Clear the kinematics settings, keep in case there are additional values we need
void
PhysBotKinematics::clearKin()
{
  clearEnc(BOTH);
}

// return the instantaneous speed estimate.  This is based on the time of a single rotation.
//	N.B. The encoder ISR was modified to time a full rotation.
float
PhysBotKinematics::intstantaneousSpeed( WHEEL wheel )
{
	// One step moves us WHEEK_SIZE*PI / GEAR_RATIO
	return 3.0 * DIST_PER_PULSE * MEGA / getPeriod(wheel); // WHY 3?  4 would make sense. 3 matches experiment.
}

float
PhysBotKinematics::averageSpeed( WHEEL wheel )
{
	long theTime = getTime(wheel);
	if (theTime>0)
		return DIST_PER_PULSE * getTicks(wheel) * MEGA / theTime;
	else
		return 0.0;  // any fall through to this point is a problem.
}

float
PhysBotKinematics::distanceTraveled( WHEEL wheel )
{
	return DIST_PER_PULSE * getTicks(wheel);
}

