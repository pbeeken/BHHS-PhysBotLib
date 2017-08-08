/***********************************************************************
   Test5 RotaryEncoder

   Knowing where your robot is can be very important. The RedBot supports
   the use of an encoder to track the number of revolutions each wheels has
   made, so you can tell not only how far each wheel has traveled but how
   fast the wheels are turning.

   This sketch was written by SparkFun Electronics, with lots of help from
   the Arduino community. This code is completely free for any use.

   8 Oct 2013 M. Hord
   Revised, 31 Oct 2014 B. Huang
   Revised, 19 Aug 2015 P.Beeken for PhysBot branch
 ***********************************************************************/

#include <Streaming.h>
#include <PhysBotLib.h>
#include <PhysBotButton.h>

PhysBotMotors motors;
PhysBotButton button;

PhysBotEncoders encoder = PhysBotEncoders();  // initializes encoder on pins A2 and 10

int countsPerRev = 192;   // 4 pairs of N-S x 48:1 gearbox = 192 ticks per wheel rev

// variables used to store the left and right encoder counts.
int lCount;
int rCount;

unsigned long triggerTime = 0;
#define DEL_TIME	200 // milliseconds

void setup()
{
  Serial.begin(115200);

  Serial << endl << "left    right" << endl;
  Serial << "================" << endl;
  triggerTime = millis() + DEL_TIME;
}

boolean goForward = true;

void loop(void)
{
  boolean taps = button.readDoublePush();

  // wait for a button press to start driving.
  if ( taps ) {
    if ( goForward ) { // wait for button press
      encoder.clear(BOTH);  // Reset the counters.
      motors.drive(100);        // Start driving forward.
      goForward = !goForward;
    } else {
      motors.drive(-150);
      goForward = !goForward;
    }
    delay(300);
  }



  if ( millis() > triggerTime ) {
    // store the encoder counts to a variable.
    lCount = encoder.getTicks(LEFT);    // read the left motor encoder
    rCount = encoder.getTicks(RIGHT);   // read the right motor encoder

    // print out to Serial Monitor the left and right encoder counts.
    Serial << lCount << "\t" << rCount << endl;
    triggerTime = millis() + DEL_TIME;  // set up next trigger


    // if either left or right motor are more than 5 revolutions, stop
    if ((abs(lCount) >= 5 * countsPerRev) || (abs(rCount) >= 5 * countsPerRev) )  {
      motors.brake();
    }
  }


}
