#ifndef PhysBotMotorServo_h
#define PhysBotMotorServo_h

#include <PhysBot.h>
#include "PhysBotKinematics.h"

enum LIMITMODE {DONE,TIME_LIMIT,DISTANCE_LIMIT,AUTOMATIC,SERVO_ONLY};

/***
 *  Handler for motor servo
 */
class PhysBotMotorServo : public PhysBotMotors {

  public:
    PhysBotMotorServo();

    // These routines set the upper limits on the robot's actions
    void setSpeedLimit( float speedInCmPerSec ); // Set speed to servo to.
    void setDistanceLimit( float distanceInCm );    // Set time limit before braking.
    void setTimeLimit( unsigned long delTimeInMs );  // Set distance limit before braking.

    void setMode( LIMITMODE setMode );  // Override the defaults above (use with care)

    void update();  // This is the routine you put in the loop function to 
                    // check on the status of the end conditions and run the servo

    boolean isDone() { return mode == DONE; } // tests if we are done.

  private:
    LIMITMODE   mode;              // current mode
    unsigned long   timeLimit;     // time limit value
    float distanceLimit;           // distance limit value
    float speedLimit;              // speed limit for servo
    long lastTick;                 // last tick we visited update

};



#endif
