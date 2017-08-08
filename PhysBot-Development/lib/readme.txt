Organization of this library for the BHHS PhysBot (based on the SparkFun Redbot)

|--PhysBotLib
|            |-- PhysBotLib.cpp
|            |-- PhysBotLib.h
|            |-- PhysBotEncoders.cpp
|            |-- PhysBotEncoders.h
|            |-- PhysBotMotors.cpp
|            |-- PhysBotMotors.h
|            # PhysBotLib has collective definitions used across libraries, source defines
|            #      interrupt and other low level routines
|            # Contains all the elements for the wheel encoders (depends on PhysBotMotors)
|            # Contains all the elements for the wheel power (depends on PhysBotEncoders)
|
|--PhysBotBlinker
|            |-- PhysBotBlinker.cpp
|            |-- PhysBotBlinker.h
|            # Organizes the controls for the pin D13 (LED) manipulation
|
|--PhysBotButton
|            |-- PhysBotButton.cpp
|            |-- PhysBotButton.h
|            # Organizes the interface for getting input from pin D12
|
|--PhysBotKinematics
|            |-- PhysBotKinematics.cpp
|            |-- PhysBotKinematics.h
|            # Translates the motion of the robot to measurable units
|
|--PhysBotAccel
|            |-- PhysBotAccel.cpp
|            |-- PhysBotAccel.h
|            # Provides simplified access to the on-board accelerometer
|
|--PhysBotMotorServo
|            |-- PhysBotMotorServo.cpp
|            |-- PhysBotMotorServo.h
|            # Provides motion based on feedback from encoders
|            # (move measured distance, time, turn a finite distance, etc.)
|
|--PhysBotSensorBar
|            |-- PhysBotSensorBar.cpp
|            |-- PhysBotSensorBar.h
|            |-- sx1509_registers.h
             # Encapsulates the sensor bar readings (largely unchanged from original)
