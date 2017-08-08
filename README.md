# PhysBot
Rewrite of the RedBot library functions for the SparkFun RedBot robot. This repository serves as the codebase for our robotics project program.

First lets give ample credit to where it is due:

- Mike Hord, SparkFun Electronics and
- B. Huang, SparkFun Electronics 

who provided an excellent starting point for this codebase. Without
their effort I wouldn't have been able to put this together.  I will
definitely buy them a round or two if I ever find myself in their
neighborhood or should they find themselves in mine.

In early 2015 we started to shop around for a robotics kit we
could incorporate into our physics and engineering classes.  We
bought a few and evaluated many more and finally settled on the
RedBot kit by SparkFun. I am not saying its the best around, everyone
has to decide what works for them but for us this was just the
ticket for us. It was relatively cheap, had ample room for 'growth' (read:
tinkering) and could serve a variety of students from NooBs to
experts.

The basic kit we provide our students allows for kinematic and
motion studies as well as control functions and projects (sumobots,
mazebots and the like).  As such all of our robots have wheel
encoders (an optional add on) and have line following sensors with a
modified mount.  We have sonic ranging kits and light sensors
(robo-roaches!).  I started to tweak the code to provide a
streamlined library for our kits and ease of use for students who are
just starting out.  Pretty soon I started to make so many
modifications that I decided to break this out into its own library.

This collection of objects is, as stated, a streamlining and
amplification of the fine work by the SparkFun Folks so as to meet
our particular needs. As we develop curricular models for projects
and should you want to implement this library (or some iteration of
your own) feel free to do so.  Please include the initial
acknowledgement for the original authors above.	

_Paul Beeken, Physics_<br/>
_Byram Hills High School_

## Summary of files
*PhysBot_Experiments*		Folder containing simple examples.
<b>REAMDE.txt</b>

<b>PhysBot.h/cpp</b>				Definitions for PhysBotMotor, PhysBotMotor, PhysBotButton, and PhysiBotSensor. Code needed by everyone.  Handles interrupts and pins

<b>PhysBotButton.cpp, </b> 
<b>PhysBotBlinker.cpp</b>
<b>PhysBotEncoder.cpp</b>
<b>PhysBotMotors.cpp</b>
<b>PhysBotSensor.cpp</b>		Implementation of the definitions in PhysBot.h

<b>PhysBotMotorServo.h/cpp</b>	Code needed if you want to limit motor motion to certain distances or times. Can also servo speed but is limited by current to motors.

<b>PhysBotKinematics.h/cpp</b>	is used by PhysBotMotorServo or if you only want to monitor the motion of the robot by itself.  Provides dimensional information (speeds in cm/s etc.)

<b>PhysBotLineTracker.h/cpp</b> Implements a line following program it shows how to manage the small sensors in coordination

<b>PhysBotAccel.h/cpp</b>		Code needed if you want to incorporate the Accelerometer daughter card.

====
### Currently unused
<b>PhysBotRadio.h/cpp,	PhysBotBumper.h/cpp, PhysBotSoftwareSerial.h/cpp
PhysBotSoftwareSerial.h</b> We'll get to these later.
