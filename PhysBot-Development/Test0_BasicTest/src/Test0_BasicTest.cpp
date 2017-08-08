/***********************************************************************
 * Exp1_BasicTest -- RedBot Experiment 1
 *
 * Time to make sure the electronics work! To test everything out, we're
 * going to blink the LED on the board.
 *
 * This sketch was written by SparkFun Electronics, with lots of help from
 * the Arduino community.
 *
 * 23 Sept 2013 N. Seidle/M. Hord
 * 04 Oct 2014 B. Huang
 ***********************************************************************/
#include <Arduino.h>
#include <PhysBotBlinker.h>

PhysBotBlinker blinker;

// setup() function runs once at the very beginning.
void setup() {
  blinker.blinkFor( 5 );
  blinker.setBlinkPeriod( 400 );
}

// loop() function repeats over and over... forever!
void loop() {
  blinker.update();
}

/***********************************************************************
 * In Arduino, an LED is often connected to pin 13 for "debug" purposes.
 * This LED is used as an indicator to make sure that we're able to upload
 * code to the board. It's also a good indicator that your program is running.
 **********************************************************************/
