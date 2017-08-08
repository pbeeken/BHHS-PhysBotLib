/****************************************************************
Declaration of PhysBotAccel

This code is beerware; if you use it, please buy me (or any other
SparkFun employee) a cold beverage next time you run into one of
us at the local.

21 Jan 2014- Mike Hord, SparkFun Electronics

Code developed in Arduino 1.0.5, on an SparkFun Redbot v12.

Very few changes. I implemented setters and getters as well as
the resolution control.

17 Aug 2015- P. Beeken, Byram Hils High School

Tested and developed in Arduino 1.6.5 on a SparkFun RedBot ROB-13166
all of our machines also come with wheel encoders.  They are not
supplied with whiskers or xBees. We are too cheap.
****************************************************************/
#ifndef PhysBotAccel_h
#define PhysBotAccel_h

#include <stdint.h>
#include <Wire.h>

// Resolution settings
enum ACC_RESOLUTION { ACCEL_RESOL_2G=0x00, ACCEL_RESOL_4G=0x01, ACCEL_RESOL_8G=0x02  };

// Class to encapsulate all the communication with the Acceleration chip.
class PhysBotAccel
{
  public:
    PhysBotAccel();     // Constructor...doesn't do much, since we re-configure
                       //  the TWI registers on each send/receive.

    void setResolution( ACC_RESOLUTION val ); // Changes the resolution of the accelerometers
    void setZero();
    void setZero( int xRef, int yRef, int zRef ) { x0 = xRef; y0 = yRef; z0 = zRef; }
    void resetZero() { setZero(0,0,0); }

    void update();       // Puts the current readings of the accelerometer into internal storage.

    inline int getAccelX() { return (x-x0); }			// get offset acceleration
  	inline int getAccelY() { return (y-y0); }			// get offset acceleration
  	inline int getAccelZ() { return (z-z0); }			// get offset acceleration

    inline int getRawX() { return x; }					// get raw acceleration
    inline int getRawY() { return y; }					// get raw acceleration
    inline int getRawZ() { return z; }					// get raw acceleration

  	// I am not sure what these are supposed to be reporting.
  	// I think the idea was that gravity is toward earth center. Any relative
  	// variation would indicate the angle wrt gravity normal. Given the noise
  	// associated with the readings especially when they are close to 0 these
  	// raw calculations are virtually useless.
  	// If the robot is sitting level on a table the values should return the
  	//	values in [] parenthesis.  A sample of returned values is given in {}
  	inline float getAngleXZ() { return 180*atan2(x,z)/PI; } // [0], {-180, which is 0 but you have to}
  	inline float getAngleYZ() { return 180*atan2(y,z)/PI; } // [0] {0}
  	inline float getAngleXY() { return 180*atan2(x,y)/PI; } // [0] {0} Again because of variations this can be all over the map
  		// Once we are on a particular angle we can do some processing to clean these
  		// values up but I am not sure this is useful in this raw form.

    void enableBump(); // Put the accelerometer into a bump detection mode.
                       //  Useful for tap-input to the robot.
    bool checkBump(); // Check to see if a tap has occurreed since the last
                       //  time this function was called.
    void setBumpThresh(int thresh); // Adjust the threshold at which a bump
                       //  is detected. Too low and motion will set it off, too
                       //  high and it won't trigger when you want it to.
  protected:
    int x, y, z;             //  let them be read as normal variables.

    int x0, y0, z0;


	float angleXZ;
	float angleXY;
	float angleYZ;

  private:
    void xlWriteBytes(byte addr, byte *buffer, byte len); // addr is the
                       //  memory location on the device to be written to; buffer
                       //  and len are fairly self explanatory. Note that the
                       //  address in the device auto-increments after each
                       //  written, allowing consecutive registers to be written
                       //  with only one command.
    void xlReadBytes(byte addr, byte *buffer, byte len); // The same as the
                       //  write command, but with reading. Same rules apply.
};


#endif
