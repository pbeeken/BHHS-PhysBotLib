/****************************************************************
Declaration for PhysBotKinematics

This class extends the PhysBotEncoder to give some physical meaning to
ticks and rotations.  Values returned from these methods are in cgs units.

17 Aug 2015- P. Beeken, Byram Hils High School

Tested and developed in Arduino 1.6.5 on a SparkFun RedBot ROB-13166
all of our machines also come with wheel encoders.  They are not
supplied with whiskers or xBees. We are too cheap.
****************************************************************/
#ifndef PhysBotKinematics_h
#define PhysBotKinematics_h

#include <Arduino.h>
#include <PhysBotLib.h>

#define WHEEL_DIA  6.5  /* cm */
#define GEAR_RATIO  48   /* turn of motor vs turn of wheel */
#define MAG_POLES   4    /* There are 4 N-S poles on the indicator  */

class PhysBotKinematics : public PhysBotEncoders {

  friend class PhysBotMotorServo;  // Needs access to our calculation methods.

  public:
    PhysBotKinematics(int lPin=A2, int rPin=10);
    //PhysBotKinematics();

    void clearAll();        // reset for counters and modes
    float intstantaneousSpeed( WHEEL wheel=BOTH );
    float averageSpeed( WHEEL wheel=BOTH );
    float distanceTraveled( WHEEL wheel=BOTH );

//    void checkKinematics();  // Periodic check in to update variables. Place in the loop.
                             // Uses little overhead when nothing is to be done

    // freq. used const. pre-calc at instantiation.
    static long  PULSE_PER_REV;
    static float DIST_PER_REV;
    static float DIST_PER_PULSE;

  private:
//    static PhysBotKinematics* kinematicsObj;

    float rInstSpeed; // right wheel instantaneous speed
    float rAvgSpeed; // right wheel average speed

    unsigned long lastVelocityCheck; // A micro-second timer value;

};



#endif
