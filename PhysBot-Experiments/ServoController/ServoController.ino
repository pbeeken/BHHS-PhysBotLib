#include <Streaming.h>
#include <Servo.h>
#include <PhysBot.h>

int pos = 0;

// This is tuned for the cheap TowerPro SG90 type servos.  Don't push them too hard. They freak out.
const int MAX = 2100;
const int MIN = 700;
const int WAIT = 40;
// Here is the tuning for the Parallax 360 servo. This one has a midpoint which sets speed 0 while walking 200us 
// in either direction causes the wheel to spin at a rate proportional to the difference from the stop point.
const int CENTER_STOP = 1536;  // anywhere from 1528 to about 1540
const int MINMAX = 180;        // µSec on either side for one way or another (larger differnce from CENTER is higher speed)
// More sophisticated servo


Servo leftServo;
PhysBotButton button;


void setup() {
  Serial.begin( 56700 );
  // put your setup code here, to run once:
  leftServo.attach( 3 );
  exploreLimits( CENTER_STOP-15, CENTER_STOP+15 );

  Serial << ".......HOLD......... " << endl;
  leftServo.writeMicroseconds( CENTER_STOP );

  delay( 2000 );
  
  Serial << ".......HOLD......... " << endl;
  exploreLimits( CENTER_STOP-MINMAX, CENTER_STOP+MINMAX );

  delay( 2000 );

  leftServo.writeMicroseconds( CENTER_STOP );

}

void exploreLimits( int min, int max ) {
  
  Serial << "Explore Limits:" << endl;
  int jmp = (max-min)/30;
  if (jmp == 0) jmp =1;

  if ( max>min ) {
      
      for( int i=min; i<max; i+=jmp ) {
        Serial << "Pulse width: " << i << endl;
        leftServo.writeMicroseconds( i );
        delay( 600 );
      }
  } else {
      for( int i=min; i>max; i+=jmp ) {
        Serial << "Pulse width: " << i << endl;
        leftServo.writeMicroseconds( i );
        delay( 600 );
      }
    
  }
  leftServo.write(0);
}


void loop() {

//  if( button.readSinglePush() ) {
//    // Extrema test
//    leftServo.write(MIN);
//    delay(3000);
//    leftServo.write(MAX);
//    delay(3000);
//    leftServo.write( (MIN+MAX)/2 );
//  }
//
//  if ( button.readDoublePush() ) {
//    // put your main code here, to run repeatedly:
//    for (pos = MIN; pos <= MAX; pos += 10) { // goes from 0 degrees to 180 degrees
//      // in steps of 1 degree
//      leftServo.write(pos);              // tell servo to go to position in variable 'pos'
//      delay(WAIT);                       // waits 200ms for the servo to reach the position
//    }
//    for (pos = MAX; pos >= MIN; pos -= 10) { // goes from 180 degrees to 0 degrees
//      leftServo.write(pos);              // tell servo to go to position in variable 'pos'
//      delay(WAIT);                       // waits 200ms for the servo to reach the position
//    }
//  }


}
