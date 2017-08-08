/****************************************************************
Implementation for PhysBotKinematics

This class extends the PhysBotEncoder to give some physical meaning to
ticks and rotations.  Values returned from these methods are in cgs units.
Since we have only two physical encoders per robot there should only be one
instance of this object per executable.

17 Aug 2015- P. Beeken, Byram Hils High School

Tested and developed in Arduino 1.6.5 on a SparkFun RedBot ROB-13166
all of our machines also come with wheel encoders.  They are not
supplied with whiskers or xBees. We are too cheap.
****************************************************************/
#include <Arduino.h>
#include <PhysBotKinematics.h>

#define MEGA 1.0E6 // micro to unitary conversion

  // Thse constants are used throughout the code. We cache them here.
long PhysBotKinematics::PULSE_PER_REV = GEAR_RATIO * MAG_POLES;
float PhysBotKinematics::DIST_PER_REV = WHEEL_DIA * PI;
float PhysBotKinematics::DIST_PER_PULSE = DIST_PER_REV / PULSE_PER_REV;

// Instantiate this subclass by passing the pins to the encoder object
PhysBotKinematics::PhysBotKinematics(int lPin, int rPin) : PhysBotEncoders(lPin, rPin)
{
}

// Instantiate this subclass by using the default pins of the encoder.
// PhysBotKinematics::PhysBotKinematics() : PhysBotEncoder(A2, 10)
// {
// //	kinematicsObj = this; // set the pointer so we can get access to the public functions.
// }


/** Clear the kinematics settings, keep in case there are additional values we need
 */
void
PhysBotKinematics::clearAll()
{
  PhysBotEncoders::clear(BOTH);
}

/** return the instantaneous speed estimate.  This is based on the time of a single rotation.
 *	N.B. The encoder ISR was modified to time a full rotation.
 */
float
PhysBotKinematics::intstantaneousSpeed( WHEEL wheel )
{
	// One step moves us WHEEK_SIZE*PI / GEAR_RATIO
	return DIST_PER_PULSE * MEGA / getPeriod(wheel);
}

/** Returns the average speed for the cart based on the tick count
 * @return {float} the average speed in cm/sec (assuming the wheel circumference is in cm)
 **/
float
PhysBotKinematics::averageSpeed( WHEEL wheel )
{
	unsigned long theTime = getTime(wheel);
	if (theTime>0)
		return DIST_PER_PULSE * getTicks(wheel) * MEGA / theTime;
	else
		return 0.0;  // any fall through to this point is a problem.
}

/** Returns the distance travelled based on the tick count
 * @return {float} the distance in cm (assuming the wheel circumference is in cm)
 **/
float
PhysBotKinematics::distanceTraveled( WHEEL wheel )
{
	return DIST_PER_PULSE * getTicks(wheel);
}
