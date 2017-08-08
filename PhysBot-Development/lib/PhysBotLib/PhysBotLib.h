/****************************************************************
Core header file for all the various PhysBot functions.

- From the original RedBot.h file:
There is additional license info below regarding the use of the
SoftwareSerial library from Arduino 1.0.5; I had good and sound
reasons for creating a derivative class rather than asking users
to simply use the existing library, which are documented below.

This code is beerware; if you use it, please buy me (or any other
SparkFun employee) a cold beverage next time you run into one of
us at the local.

21 Jan 2014- Mike Hord, SparkFun Electronics
Code developed in Arduino 1.0.5, on an SparkFun Redbot v12.

Extensive modification. Some classes were left as is, many more
added. Most were adjusted and rewritten in one form or another.
Many of the lowest level access routines and interrupt service
handlers were left pretty much, as is.  Higher level routines were
modified to match our specific needs and hardware.  Most importantly:
routines that previously BLOCKED are now gone.  BLOCKING means that for
a given method, when called, would hold the full attention of the Arduino
until it finished its task, thus blocking other routines from doing their
job.  In all classes which need to perform periodic tasks look for a
method called 'update'.  This is the method that needs to be called once
per 'loop()' to check to see if something needs to be done.  If not, that
object's update method returns quickly so other object update routines can
get on with their jobs as well.  Ideally we could register these things in a
scheduler class but that is for another day.
17 Aug 2015- P. Beeken, Byram Hils High School

Tested and developed in Arduino 1.6.5 on a SparkFun RedBot ROB-13166
all of our machines also come with wheel encoders.  They are not
supplied with whiskers or xBees. We are too cheap.
****************************************************************/

#ifndef PhysBotLib_h
#define PhysBotLib_h

/** PCINT functionality aliases. Each PCINT has a value set up when the
 *  class member gets created, and the PCINT service routine will handle
 *  the choosing the appropriate response to the interrupt.
 */

#define NOT_IN_USE    0
#define WHISKER       1
#define LENCODER      2
#define RENCODER      3
#define SW_SERIAL     4

#define PCINT_A0      0
#define PCINT_A1      1
#define PCINT_A2      2
#define PCINT_A3      3
#define PCINT_A4      4
#define PCINT_A5      5
#define PCINT_3       6
#define PCINT_9       7
#define PCINT_10      8
#define PCINT_11      9


//enum WHEEL {LEFT, RIGHT, BOTH};	// Variable for which wheel you're interested in
								//  when you do things in the motor or encoder class.
//#define RESET	1024 // Special flag for anything with a speed, real speed values cannot be this high so this works as a message.

/** These three functions need to work from within multiple classes, so we keep
 *  them separate and add them as friend functions where appropriate.
 */
void setPinChangeInterrupt(int pin, int role); // The "role" of each pin is
                  //  stored in an array which is accessed in the interrupt
                  //  handler to determine what should be done on a falling edge
                  //  PC interrupt.
void pinFunctionHandler(int pinIndex); // This is the function which actually
                  //  handles the legwork after the interrupt has identified
                  //  which pin caught the interrupt.
//void brake(void); // Globally accessible motor brake. I couldn't figure out how
                  //  to set a function pointer to the PhysBotMotors class
                  //  function, and it's a small function, so I just made a
                  //  global in the library.
                  // Use a static method. PBB, will test when we get around to using whiskers
void PC0Handler(char PBTemp);
void PC1Handler(char PCTemp);
void PC2Handler(char PDTemp);

// Definitions used across many different files
enum WHEEL {LEFT, RIGHT, BOTH};	// Variable for which wheel you're interested in
								//  when you do things in the motor or encoder class.
#define RESET	1024 // Special flag for anything with a speed,

// This is a robot so the motors and their controls are central to all projects
#include <PhysBotMotors.h>
#include <PhysBotEncoders.h>


#endif
