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

#include <Arduino.h>
#include <PhysBotLib.h>
#include <PhysBotAccel.h>
#include <Wire.h>

// We have a bunch of stuff associated with the accelerometer here. We're going
//  to implement our own I2C functions, too, to make things easy on ourselves.
#define XL_ADDR     0x1D // I2C address of the MMA8452Q accelerometer
#define I2C_READ    0x01 // I2C read bit set
// Some values we'll load into TWCR a lot
#define START_COND  0xA4 // (1<<TWINT) | (1<<TWSTA) | (1<<TWEN)
#define STOP_COND   0x94 // (1<<TWINT) | (1<<TWSTO) | (1<<TWEN)
#define CLEAR_TWINT 0x84 // (1<<TWINT) | (1<<TWEN)
#define NEXT_BYTE   0xC4 // (1<<TWINT) | (1<<TWEA) | (1<<TWEN)

#define PULSE_SRC	0x22

PhysBotAccel::PhysBotAccel()
{
  byte buffer[2];

  x0 = 0;
  y0 = 0;
  z0 = 0;

  // This sets the bit rate of the bus; I want 100kHz. See the
  //  datasheet for details on how I came up with this value.
  TWBR = 72;

  // The very first thing we want to do is reset the accelerometer. Since
  //  we don't know what's happened since the last time we powered the
  //  thing up, the best we can do is reset it by twiddling the reset bit
  //  (bit 6) in CTRL2 register (0x2B).
  buffer[0] = 0x40;
  xlWriteBytes(0x2B, buffer, 1);

  // Accelerometer configuration- there are five registers (starting with
  //  0x2A) that must be configured for the accelerometer to operate. We'll
  //  create a byte buffer and fill it with the values we need, then push it
  //  to the accelerometer. Only the first two registers need to be fiddled
  //  with, the rest are for interrupts and things we don't need to worry
  //  about.

  // Let's set the dynamic range to max out at 8g instead of 2.
  buffer[0] = ACCEL_RESOL_8G;
  xlWriteBytes(0x0E, buffer, 1);

  // This is only needed for bump detection
  // We are going to set the HP filter to eliminate the 0 offset (we loose orientation)
  // 5   - Bypass High Pass Filter for Pulse Processing Function.
  //       0: HPF enabled for Pulse Processing, 1: HPF Bypassed for Pulse Processing
  // 4   - Enable Low Pass Filter for Pulse Processing Function.
  //       0: LPF disabled for Pulse Processing, 1: LPF Enabled for Pulse Processing
  // 1:0 - switches for
  // buffer[0] = 0x03; // 2Hz cutoff
  // xlWriteBytes(0x0F, buffer, 1);

  // Now we'll populate our buffer.
  // CTRL1 register, five settings here:
  //  7:6 - sample rate during sleep mode (leave default 50Hz)
  //  5:3 - data rate (leave default 800Hz)
  //  2   - low noise mode (set to 1 to activate)
  //  1   - 8-bit mode (leave 0 to disable)
  //  0   - active/standby (leave 0, for now)
  buffer[0] = 0x04;

  // CTRL2 register, five more settings:
  //  7   - self-test (leave 0, disabled)
  //  6   - software reset (leave 0, no reset)
  //  5   - no use
  //  4:3 - sleep mode (leave 00, normal mode)
  //  2   - sleep enable (leave 0, disable auto sleep)
  //  1:0 - active mode (set 10, hi-res oversample)
  buffer[1] = 0x02;

  // Now that we've populated our buffer, we can push it to the accelerometer.
  xlWriteBytes(0x2A, buffer, 2);

  // Now that we've set that up, we can enable the part by writing 0x05 to
  //  CTRL1.
  buffer[0] = 0x05;
  xlWriteBytes(0x2A, buffer, 1);
}

// setResolution() changes the resolution of the accelerometer
void PhysBotAccel::setResolution( ACC_RESOLUTION val )
{
  byte buffer[2];
  // The *very* first thing we need to do is disable the chip; otherwise,
  //  we can't change the register settings.
  xlReadBytes(0x2A, buffer, 1);
  buffer[0] &= 0xFE;
  xlWriteBytes(0x2A, buffer, 1);

  // Let's set the dynamic range to max out at the requested value.
  buffer[0] = (byte)val;
  xlWriteBytes(0x0E, buffer, 1);
  // Now that we've set that up, we can enable the part by writing 0x05 to
  //  CTRL1.
  buffer[0] = 0x05;
  xlWriteBytes(0x2A, buffer, 1);
}

// setZero() establishes the zero points of the accelerometers.
//  This is intedned to be used in the 'setup()' phase of arduino while the
// robot is sitting still.  Note that this isn't always desirable.  We loose
// the gravity reference.  N.B. There is a way to do this in the chip itself
// but is more work than is necessary see 'AN4069 Offset Calibration Using the MMA8452Q'
void PhysBotAccel::setZero() {
	int count = 10;
	long x00=0L, y00=0L, z00=0L;
	for ( int i=0; i<count; i++ ) {
		update();
		x00 += x;
		y00 += y;
		z00 += z;
		delay( 10 );
	}
	x0 = x00/ count; y0 = y00/ count; z0 = z00/ count;
}

// read() initiates a capture of the current x, y, and z values, and stores
//  them in the appropriate class member variables.
void PhysBotAccel::update()
{
  // The first step, the easy step, is to grab the values. We'll put 'em in
  //  a byte buffer.
  byte buffer[6];
  xlReadBytes(0x01, buffer, 6);

  // Next, we need to copy the data into the member variables so they can be
  //  accessed by the user.
  x = (buffer[0]<<8 | buffer[1])>>4;
  y = (buffer[2]<<8 | buffer[3])>>4;
  z = (buffer[4]<<8 | buffer[5])>>4;

  // Adding these three calculations adds ~ 700us to this process.
  // This method takes ~856 us to run w/o them in and about 1532 us with
  // these floating point operations. (BH)
//  angleXZ = 180*atan2(x,z)/PI;
//  angleXY = 180*atan2(x,y)/PI;
//  angleYZ = 180*atan2(y,z)/PI;

}

// For bump detection, we're looking for a transient in the Z direction. The
// bump should be pretty hard, so hopefully, we'll be able to distinguish
// between a bump and a tap.
void PhysBotAccel::enableBump()
{
  byte buffer[8];

  // The *very* first thing we need to do is disable the chip; otherwise,
  //  we can't change the register settings.
  xlReadBytes(0x2A, buffer, 1);
  buffer[0] &= 0xFE;
  xlWriteBytes(0x2A, buffer, 1);

  // To enable tap detection, we need to write some data to registers
  // 0x21-0x28. See Freescale app note 4072 for more info about setting
  // this up.

  // The very first thing we'll do is enable the LPF for pulse detection.
  // This is in register 0x0F.
  buffer[0] = 0x10;
  xlWriteBytes(0x0F, buffer, 1);

  // Since tap detection and bump detection use the same system resources,
  // we need to fetch the data from the accelerometer before we can set up
  // tap.
  xlReadBytes(0x21, buffer, 8);

  // Now that we have the current settings, we can turn on z-axis tap detection
  // by fiddling with the appropriate bits.

  // 0x21 (PULSE_CFG)- We need to set bit 6 (ELE, latch events into register)
  // and bit 0 (XSPEFE, x-axis single pulse event function enable)
  buffer[0] = 0x55; //0x41;  // PBB enable all axes.

  // 0x22 (PULSE_SRC)- we'll read this to check for pulses; it's read only, so
  // we don't need to do anything with it here.
  buffer[1] |= 0x00; // just a placeholder

  // 0x23- X pulse threshold- experimentally determined to be a good value for a
  // threshold.
  buffer[2] = 30; // orig=32
  // 0x24- Y pulse threshold
  // Both of these can be ignored, and shouldn't be touched, in case they're
  // configured for something else.
  buffer[3] = 30; // placeholder orig: 0x00

  // 0x25 (PULSE_THSZ)
  buffer[4] = 30; // orig: 0x00

  // 0x26 (PULSE_TMLT)- maximum length a pulse must be to be detected as a tap.
  // The length is dependent upon three things: the sampling rate (800Hz),
  // whether Pulse_LPF is set or clear in register 0x0F (it's not), and the
  // sampling mode (Hi-res). Charts on pp34-35 of the datasheet tell us that
  // the maximum pulse length here is this register value times 0.625ms.
  buffer[5] = 25;  // maximum pulse length of 62.5ms

  // 0x27 (PULSE_LTCY)- lockout time after a pulse occurs before another one
  // will be sensed. Charts for value are on page 35 of the datasheet.
  buffer[6] = 50; // 125ms lockout period

  // 0x28 (PULSE_WIND)- window within which a second tap must occur to register
  // a double tap event. We aren't worried about double taps (yet), so let's
  // leave this unchanged.
  buffer[7] |= 0x00;  // placeholder

  // Write the values we just set up back into the accelerometer.
  xlWriteBytes(0x21, buffer, 8);

  // Now we need to put the device back into active mode.
  xlReadBytes(0x2A, buffer, 1);
  buffer[0] |= 0x01;
  xlWriteBytes(0x2A, buffer, 1);
}


#define ACCEL_BUMP_X    0x10
#define ACCEL_BUMP_Y    0x20
#define ACCEL_BUMP_Z    0x40

/***
 * While it is theoretically possible to determine the direction of a 'bump'
 * it is difficult in practice give the precarious mounting of the accel.
 * chip.  Any tap from almost any direction can set off the 'bump' sensor.
 * the result is that we will accept any input as a 'bump'
 **/
boolean
PhysBotAccel::checkBump()
{
  byte buffer = 0; // int mask = ACCEL_BUMP_X;

  xlReadBytes(PULSE_SRC, &buffer, 1);   // check the PULSE_SRC register to see if a
                                   //  pulse event has been registered. This
                                   //  will clear all pulse events
 // return (buffer & 0x7F);
  if ( (buffer & 0x80)==0 ) return false; // no events triggered.
  if ( (buffer & 0x70)!=0 ) return true;  // Mask for X events.
  else               return true;  // something else.

}

void PhysBotAccel::setBumpThresh(int thresh)
{
  byte buffer;
  // The *very* first thing we need to do is disable the chip; otherwise,
  //  we can't change the register settings.
  xlReadBytes(0x2A, &buffer, 1);
  buffer &= 0xFE;
  xlWriteBytes(0x2A, &buffer, 1);

  // 0x23- X pulse threshold- experimentally determined to be a good value for a
  //  threshold.
  buffer = (byte)thresh;

  // Write the value we just set up back into the accelerometer.
  xlWriteBytes(0x23, &buffer, 1);
  xlWriteBytes(0x24, &buffer, 1);
  xlWriteBytes(0x25, &buffer, 1);

  // Now we need to put the device back into active mode.
  xlReadBytes(0x2A, &buffer, 1);
  buffer |= 0x01;
  xlWriteBytes(0x2A, &buffer, 1);
}

// Private function that reads some number of bytes from the accelerometer.
void PhysBotAccel::xlReadBytes(byte addr, byte *buffer, byte len)
{
  unsigned int timeout = 0; // We're going to use this to set a timeout on the
                            //  amount of time we'll wait for the bus to become
                            //  available. The minimum period here is about 4ms
                            //  on a 16MHz device.

  // First, we need to write the address we want to read from, so issue a write
  //  with that address. That's two steps: first, the slave address:
  TWCR = START_COND;          // Send a start condition
  while (!(TWCR&(1<<TWINT))) // When TWINT is set, start is complete, and we
                              //  can initiate data transfer.
  {
    if (++timeout == 0) return; // time out if the bus is busy. In most cases,
  }                           //  "busy" means no sensor on the bus.
  timeout = 0;
  TWDR = XL_ADDR<<1;          // Load the slave address
  TWCR = CLEAR_TWINT;         // Clear TWINT to begin transmission (I know,
                              //  it LOOKS like I'm setting it, but this is
                              //  how we clear that bit. Dumb.)
  while (!(TWCR&(1<<TWINT)))  // Wait for TWINT again.
  {
    if (++timeout == 0) return; // time out if the bus is busy. In most cases,
  }                           //  "busy" means no sensor on the bus.
  timeout = 0;
  // Now, we need to send the address we want to read from:
  TWDR = addr;                // Load the slave address
  TWCR = CLEAR_TWINT;        // Clear TWINT to begin transmission (I know,
                              //  it LOOKS like I'm setting it, but this is
                              //  how we clear that bit. Dumb.)
  while (!(TWCR&(1<<TWINT)))  // Wait for TWINT again.
  {
    if (++timeout == 0) return; // time out if the bus is busy. In most cases,
  }                           //  "busy" means no sensor on the bus.
  timeout = 0;
  TWCR = STOP_COND;

  timeout = 0;

  // Now, we issue a repeated start (by doing what we just did again), and this
  //  time, we set the READ bit as well.
  TWCR = START_COND;          // Send a start condition
  while (!(TWCR&(1<<TWINT)))  // When TWINT is set, start is complete, and we
                              //  can initiate data transfer.
  {
    if (++timeout == 0) return; // time out if the bus is busy. In most cases,
  }                           //  "busy" means no sensor on the bus.
  timeout = 0;
  TWDR = (XL_ADDR<<1) | I2C_READ;  // Load the slave address and set the read bit
  TWCR = CLEAR_TWINT;        // Clear TWINT to begin transmission (I know,
                              //  it LOOKS like I'm setting it, but this is
                              //  how we clear that bit. Dumb.)
  while (!(TWCR&(1<<TWINT)))  // Wait for TWINT again.
  {
    if (++timeout == 0) return; // time out if the bus is busy. In most cases,
  }                           //  "busy" means no sensor on the bus.
  timeout = 0;

  // Now, we can fetch data from the slave by clearing TWINT, waiting, and
  //  reading the data. Rinse, repeat, as often as needed.
  for (byte i = 0; i < len; i++)
  {
    if (i == len-1) TWCR = CLEAR_TWINT; // Clear TWINT to begin transmission (I know,
                                //  it LOOKS like I'm setting it, but this is
                                //  how we clear that bit. Dumb.)
    else TWCR = NEXT_BYTE;
    while (!(TWCR&(1<<TWINT)))  // Wait for TWINT again.
  {
    if (++timeout == 0) return; // time out if the bus is busy. In most cases,
  }                           //  "busy" means no sensor on the bus.
  timeout = 0;
    buffer[i] = TWDR;           // Now our data can be fetched from TWDR.
  }
  // Now that we're done reading our data, we can transmit a stop condition.
  TWCR = STOP_COND;
}

void PhysBotAccel::xlWriteBytes(byte addr, byte *buffer, byte len)
{
  unsigned int timeout = 0; // We're going to use this to set a timeout on the
                            //  amount of time we'll wait for the bus to become
                            //  available. The minimum period here is about 4ms
                            //  on a 16MHz device.

  // First, we need to write the address we want to read from, so issue a write
  //  with that address. That's two steps: first, the slave address:
  TWCR = START_COND;          // Send a start condition
  while (!(TWCR&(1<<TWINT))) // When TWINT is set, start is complete, and we
                              //  can initiate data transfer.
  {
    if (++timeout == 0) return; // time out if the bus is busy. In most cases,
  }                           //  "busy" means no sensor on the bus.
  timeout = 0;
  TWDR = XL_ADDR<<1;          // Load the slave address
  TWCR = CLEAR_TWINT;         // Clear TWINT to begin transmission (I know,
                              //  it LOOKS like I'm setting it, but this is
                              //  how we clear that bit. Dumb.)
  while (!(TWCR&(1<<TWINT)))  // Wait for TWINT again.
  {
    if (++timeout == 0) return; // time out if the bus is busy. In most cases,
  }                           //  "busy" means no sensor on the bus.
  timeout = 0;
  // Now, we need to send the address we want to read from:
  TWDR = addr;                // Load the slave address
  TWCR |= CLEAR_TWINT;         // Clear TWINT to begin transmission (I know,
                              //  it LOOKS like I'm setting it, but this is
                              //  how we clear that bit. Dumb.)
  while (!(TWCR&(1<<TWINT)))  // Wait for TWINT again.
  {
    if (++timeout == 0) return; // time out if the bus is busy. In most cases,
  }                           //  "busy" means no sensor on the bus.
  timeout = 0;

  // Now, we can send data to the slave by putting data into TWDR, clearing
  //  TWINT, and waiting for TWINT. Rinse, repeat, as often as needed.
  for (byte i = 0; i < len; i++)
  {
    TWDR = buffer[i];           // Now our data can be sent to TWDR.
    TWCR |= CLEAR_TWINT;        // Clear TWINT to begin transmission (I know,
                                //  it LOOKS like I'm setting it, but this is
                                //  how we clear that bit. Dumb.)
    while (!(TWCR&(1<<TWINT)))  // Wait for TWINT again.
  {
    if (++timeout == 0) return; // time out if the bus is busy. In most cases,
  }                           //  "busy" means no sensor on the bus.
  timeout = 0;
  }
  // Now that we're done writing our data, we can transmit a stop condition.
  TWCR = STOP_COND;
}
