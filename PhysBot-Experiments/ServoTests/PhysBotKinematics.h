#ifndef PhysBotKinematics_h
#define PhysBotKinematics_h

#include <PhysBot.h>

#define WHEEL_DIA  6.5  /* cm */
#define GEAR_RATIO  48   /* turn of motor vs turn of wheel */
#define MAG_POLES   4    /* There are 4 N-S poles on the indicator  */

/***
 *  Handler for motor kinematics.  There is much evil we need to deal with
 *  there is a lot of variation in the encoder data so we need to smooth the values
 *  When starting up the motors are not representing the speed well.  We also want
 *  want to be able to control the acceleration.
 */
class PhysBotKinematics : public PhysBotEncoder {

  public:
    PhysBotKinematics(int lPin, int rPin);

    void clearKin();        
    float intstantaneousSpeed( WHEEL wheel );
    float averageSpeed( WHEEL wheel );
    float distanceTraveled( WHEEL wheel );

//    void checkKinematics();  // Periodic check in to update variables. Place in the loop.  
                             // Uses little overhead when nothing is to be done
    static PhysBotKinematics* kinematicsObject;

    // freq. used const. pre-calc at instantiation.
    static long  PULSE_PER_REV;
    static float DIST_PER_REV;
    static float DIST_PER_PULSE;

  private:
    float rInstSpeed; // right wheel instantaneous speed
    float rAvgSpeed; // right wheel average speed

  
    unsigned long lastVelocityCheck; // A micro-second timer value;
//    long rLastStep; // last step for rightMotor
//    long rDeltaTimes[10]; // keep track of the last 5
//    long lLastStep; 

};



#endif
