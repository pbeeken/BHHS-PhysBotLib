/***
 * Kinemtics Code model
 */

#include <Streaming.h>
#include <PhysBot.h>


PhysBotMotors   motors;
PhysBotEncoder  enc(A2, 10);
PhysBotButton   button;
PhysBotBlinker  led;

int buttonPress = 0;
unsigned long start = 0L;



void setup() {
  // put your setup code here, to run once:
  Serial.begin( 57600 );
  led.activate();
  Serial << "-Ready to Fly-" << endl;
}

void loop() {

  led.update();

  if ( button.readSinglePush() ) {
    motors.brake();
    ++buttonPress;
    
    buttonPress = buttonPress % 10;
    led.blinkFor( buttonPress );

    start = millis();
  }

  update();

}


void update() {

  int spd;
  unsigned long deltaT = millis()-start;

  switch ( buttonPress ) {

    case 1: // Constant forward speed
      spd = 100;
      motors.drive( spd );
      if ( millis() > start + 2000 )
        motors.coast();
      break;

    case 2: // Constant forward speed x2
      spd = 200;
      motors.drive( spd );
      if ( millis() > start + 2000 )
        motors.coast();
      break;

    case 3: // Constant backward speed
      spd = -100;
      motors.drive( spd );
      if ( millis() > start + 2000 )
        motors.coast();
      break;

    case 4: // Constant backward speed x2
      spd = -200;
      motors.drive( spd );
      if ( millis() > start + 2000 )
        motors.coast();
      break;

    case 5: // starting from 0 constant forward acceleration
      // over 5 seconds go from 0 to 200
//      unsigned long delTime = millis() - start;
      spd = (200 * deltaT) / 5000;
      spd = constrain( spd, -250, 250);
      motors.drive( spd );
      if ( millis() > start + 5000 )
        motors.coast();
      break;

    case 6: // startig from -200 constant forward acceleration
      // over 5 seconds go from -200 to 200
//      unsigned long delTime = millis() - start;
      spd = -200 + (400 * deltaT) / 5000;
      spd = constrain( spd, -250, 250);
      motors.drive( spd );
      if ( millis() > start + 5000 )
        motors.coast();
      break;

    default:
      motors.brake();
      break;
  }


}

// blink n times in response to a value.
void blinkNtimes( int n ) {
  pinMode( 13, OUTPUT );
  digitalWrite( 13, LOW );
  while ( n-- > 0 ) {
    digitalWrite( 13, HIGH );
    delay( 300 );
    digitalWrite( 13, LOW );
    delay( 300 );
  }

}

// Naieve heatnbeat to make sure we are alive.
void heartBeat() {
  static boolean bblink = false;
  static unsigned long bstart = 0L;
  pinMode( 13, OUTPUT );
  if ( millis() > bstart + 500 ) {  // Heartbeat
    if ( bblink ) digitalWrite( 13, HIGH );
    else         digitalWrite( 13, LOW );
    bblink = !bblink;
    bstart = millis();
  }
}

