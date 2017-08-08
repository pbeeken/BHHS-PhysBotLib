/******************************************************************************
ReadBarOnly.ino

A sketch that demonstrates how the data comes from the sensor bar.

Marshall Taylor, SparkFun Engineering

5-27-2015

Library:
https://github.com/sparkfun/SparkFun_Line_Follower_Array_Arduino_Library
Product:
https://github.com/sparkfun/Line_Follower_Array

This sketch simply reads the data from the bar and prints it to the serial buffer.
  "SensorBar mySensorBar(SX1509_ADDRESS);" creates the sensor bar object.
  "mySensorBar.begin();" gets the bar ready.
  "mySensorBar.getDensity()" gets the number of points sensed.
  "mySensorBar.getPosition()" gets the average center of sensed points.

Resources:
sensorbar.h

Development environment specifics:
arduino > v1.6.4
hw v1.0

This code is released under the [MIT License](http://opensource.org/licenses/MIT).
Please review the LICENSE.md file included with this example. If you have any questions
or concerns with licensing, please contact techsupport@sparkfun.com.
Distributed as-is; no warranty is given.
******************************************************************************/

#include <Arduino.h>
#include <Streaming.h>
#include <PhysBotSensorBar.h>

// Uncomment one of the four lines to match your SX1509's address
//  pin selects. SX1509 breakout defaults to [0:0] (0x3E).
//const uint8_t SX1509_ADDRESS = 0x3E;  // SX1509 I2C address (00)
//const byte SX1509_ADDRESS = 0x3F;  // SX1509 I2C address (01)
//const byte SX1509_ADDRESS = 0x70;  // SX1509 I2C address (10)
//const byte SX1509_ADDRESS = 0x71;  // SX1509 I2C address (11)

PhysBotSensorBar mySensorBar(SX1509_ADDRESS);

void setup()
{
  Serial.begin(115200);  // start serial for output
  Serial << "Sensor Bar Read Test." << endl << endl;

  //For this demo, the IR will only be turned on during reads.
//  mySensorBar.setBarStrobe();
  //Other option: Command to run all the time
  mySensorBar.clearBarStrobe();

  //Default dark on light
  mySensorBar.clearInvertBits();
  //Other option: light line on dark
  //mySensorBar.setInvertBits();

  //Don't forget to call .begin() to get the bar ready.  This configures HW.
  uint8_t returnStatus = mySensorBar.begin();
  if(returnStatus)
  {
	  Serial << "sx1509 IC communication OK" << endl;
  }
  else
  {
	  Serial << "sx1509 IC communication FAILED!" << endl;
	  while(1);
  }

  Serial << endl;

}

void loop()
{
  //Get the data from the sensor bar and load it into the class members
  uint8_t rawValue = mySensorBar.getRaw();

  //Print the binary value to the serial buffer.
  Serial << "Bin value: 0b" << _BIN(rawValue) << endl;

  //Print the hex value to the serial buffer.
  Serial << "Hex value: 0x" << _HEX(rawValue) << endl;

  //Print the position and density quantities
  Serial << "Position (-127 to 127): " << mySensorBar.getPosition() << endl;
  Serial << "Density, bits detected (of 8): " << mySensorBar.getDensity() << endl;
  Serial << endl;

  //Wait 2/3 of a second, just cause we can.
  delay(666);

}
