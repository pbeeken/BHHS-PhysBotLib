/****************************************************************
Declaration for PhysBotMotorServo

Declaration for sensorbar.  SparkFun provided a fine library but we
we want something that fits our overall scheme

27 Aug 2016- P. Beeken, Byram Hils High School

Tested and developed in Arduino 1.6.5 on a SparkFun RedBot ROB-13166
all of our machines also come with wheel encoders.  They are not
supplied with whiskers or xBees. We are too cheap.
****************************************************************/
#ifndef PHYSBOTSENSORBAR_H
#define PHYSBOTSENSORBAR_H

#include <stdint.h>
#include <Wire.h>


#define RECEIVE_TIMEOUT_VALUE 1000	// Timeout for I2C receive

// These are used for setting LED driver to linear or log mode:
#define LINEAR		0
#define LOGARITHMIC	1

// These are used for clock config:
#define INTERNAL_CLOCK	2
#define EXTERNAL_CLOCK	1

const uint8_t SX1509_ADDRESS = 0x3E;  // SX1509 I2C address (00)
//const uint8_t SX1509_ADDRESS = 0x3F;  // SX1509 I2C address (01)
//const uint8_t SX1509_ADDRESS = 0x70;  // SX1509 I2C address (10)
//const uint8_t SX1509_ADDRESS = 0x71;  // SX1509 I2C address (11)

class PhysBotSensorBar
{
  public:
    //New functions for bar specific operation
    PhysBotSensorBar( uint8_t address, uint8_t resetPin = 255, uint8_t interruptPin = 255, uint8_t oscillatorPin = 255 );
    uint8_t begin( void );
    uint8_t getRaw( void );
    int8_t getPosition( void );
    uint8_t getDensity( void );
	void setBarStrobe( void );
	void clearBarStrobe( void );
	void setInvertBits( void ); //Dark (no reflection) = 1, position returns center of dark
	void clearInvertBits( void ); //Light (reflection) = 1, position returns center of light

	//Functions pulled from the SX1509 driver
    void reset( void );
	void debounceConfig( uint8_t configValue );
    void debounceEnable( uint8_t pin );
    void enableInterrupt( uint8_t pin, uint8_t riseFall );
    unsigned int interruptSource( void );
    void configClock( uint8_t oscSource = 2, uint8_t oscPinFunction = 0, uint8_t oscFreqOut = 0, uint8_t oscDivider = 1 );


  private:
    //Holding variables
    uint8_t lastBarRawValue;
	uint8_t lastBarPositionValue;

	//Settings
	uint8_t deviceAddress; // I2C Address of SX1509
	uint8_t barStrobe; // 0 = always on, 1 = power saving by IR LED strobe
	uint8_t invertBits; // 1 = invert

    // Pin definitions:
    uint8_t pinInterrupt;
    uint8_t pinOscillator;
    uint8_t pinReset;

    // Read Functions:
    void scan( void );
    uint8_t readByte(uint8_t registerAddress);
    unsigned int readWord(uint8_t registerAddress);
    void readBytes(uint8_t firstRegisterAddress, uint8_t * destination, uint8_t length);
    // Write functions:
    void writeByte(uint8_t registerAddress, uint8_t writeValue);
    void writeWord(uint8_t registerAddress, unsigned int writeValue);
    void writeBytes(uint8_t firstRegisterAddress, uint8_t * writeArray, uint8_t length);
};

//****************************************************************************//
//
//  Circular Buffer
//
//****************************************************************************//

//Class CircularBuffer is int16_t
//Does not care about over-running real data ( if request is outside length's bounds ).
//For example, the underlying machine writes [48], [49], [0], [1] ...

class CircularBuffer
{
public:
    CircularBuffer( uint16_t inputSize );
	~CircularBuffer();
    int16_t getElement( uint16_t ); //zero is the push location
    void pushElement( int16_t );
    int16_t averageLast( uint16_t );
    uint16_t recordLength( void );
private:
	uint16_t cBufferSize;
    int16_t *cBufferData;
    int16_t cBufferLastPtr;
    uint8_t cBufferElementsUsed;
};



#endif // SENSORBAR_H
