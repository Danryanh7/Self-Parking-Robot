# Self-Parking-Robot
Robot C code for a self parking LEGO MINDSTORM EV3 for the final project of MTE_121

## Description
The goal of our project was to program the Lego EV3 robot to drive until an open parking space is
detected, then perform a parallel park in that spot. Once the robot is turned on, it will be on standby
until the "car key" triggers the program to start. Once the robot has started driving, a 30-second timer
will be running in the background which will shut down the robot if no open spot is found within that
time frame. The robot should be checking for cars parked to the right of it and the distance between
them. If the space between two cars is sufficient for the robot to park, it should line itself up with the car
at the front of the parking space then manoeuvre itself into the spot. The robot should be rotating,
reversing, straightening itself out, then moving forward if it is too close to the car behind it. If the robot
does a parallel park, it should raise the banner upon completion.

## Non-Trivial Function Descriptions
### findParkingSpaceAndPark
The primary objective of "findParkingSpaceAndPark" is to locate a suitable parking space for the robot
under several constraints. Once a parking space is found, the "parallelPark" function is called which
satisfies its secondary objective. This function initiates by declaring several variables that contain; the
gap distance between two cars (float gapDistance), the number of failed parallel parking attempts (int
carCount), and a boolean that determines if the parallel parking attempt was successful (bool finished)
A constant integer is also declared which helps convert angular encoder measurements to centimetres
(const int encToCM).

A while loop is then initialized which loops if the number of parallel parking attempts is less than the
predefined constant of max attempts and if "finished" has a value of "false". The
motors are then turned on and the "searchForCar" function is called with the parameter "true" to look
for the first car of which the robot will parallel park in front. Once the first car is
located, the "searchForCar" function is called with the parameter "false" such that an empty space may
be searched for. The motor encoders are then reset such that the gap distance
between the two vehicles may be measured and the "searchForCar" function is called once more with
the parameter "true". Once the second car is sensed, the motors are turned off
and the gap distance is set equal to the absolute value of the encoder value of motorA multiplied by
"encToCM".

A number of end conditions are then checked. If gapDistance is greater than the predefined constant
"MINIMUM_GAP_NEEDED", the parallelPark function is called and "finished" is set
to "true". If "TIMER" becomes greater than the constant
"MAX TIME FOR ATTEMPTS" "finished" is returned with the value "false". This exits the process of
searching for a parking space if the robot "runs out of fuel" in this fictitious scenario without interfering
with the maximum attempt counter. If both preceding statements are false, "carCount" is incremented
and "finished" is returned with value "false"

### waitForGreenCard
This function waits for the presence of the green "key" before the robot begins to search for a parallel
parking spot. A while loop loops infinitely whilst the colour green is not sensed by
the colour sensor

### searchForCar
The "searchForCar" function is called to both actively look for a car, or to search for empty space by
inverting the search condition. This function accepts a boolean parameter "lookingForCar". If
"lookingForCar" is true, a while loop is declared which loops until the ultrasonic sensor located on the
side of the robot returns a value that is less than the constant integer "VALID_DISTANCE". The value of
"TIMER" must also be less than the "MAX_TIME_FOR_ATTEMPT" to stay within the while loop
Essentially, this function is receiving the value of the side ultrasonic sensor. Once the value of this
particular sensor is less than the value "VALID_DISTANCE", a car is found. If "lookingForCar" is false, a
while loop is declared which loops until the side ultrasonic sensor returns a value that is greater than
"VALID DISTANCE. Once again, the value of "TIMER" must be less than the "MAX_TIME_FOR_ATTEMPT'
to stay within the loop. If "lookingForCar" is false, the function effectively operates as the reverse of
how it would operate if "lookingForCar" was true. That is to say, if "lookingForCar" is false, the function
searches for empty space beyond the value of "VALID_DISTANCE" instead of a car. Once the value of the
side ultrasonic sensor is greater than "VALID_DISTANCE", empty space is found
The value of "TIMER" is then checked. If the value of "TIMER" is greater than
"MAX TIME FOR ATTEMPT", the motors are turned off.

### parallelPark
"parallelPark" calls several functions (parkingLineUp, reverselntoSpot, pullForward), to simulate parallel
parking the vehicle.

### parkingLineUp:
"parkingLine Up" is a function that drives the robot forwards a distance equal to one-third of the robot
body (measured in centimetres) such that the robot is positioned to reverse into the parking spot when
the "reverselntoSpot" function is called. The function turns on the motors, resets the encoder of motorA
and calls the "driveDistanceCM" function. The parameters passed to the "driveDistanceCM" function are
the distance one desires to move the robot (one-third of the robot body), and the direction (forwards).

### reverselntoSpot
"reverselntoSpot" is the function that facilitates the parallel parking action of the robot. This function
takes a parameter "distanceToCar" which measures the distance between the adjacent side of the robot
to the second car detected. The function first rotates the robot clockwise 45 degrees and drives the
robot backwards a distance equal to the sum of the robot width and "distanceToCar" in centimetres, to
position the robot between two cars. The robot will then rotate counterclockwise 45 degrees such that
the robot will be positioned parallel to the cars it is parking between. The function then calls the
"pullForward" function, which drives the robot forwards such that the robot is an appropriate distance
away from the rear car.

### pullForward
"pullForward" is called within the "reverselntoSpot" function to position the robot an appropriate
distance away from the rear car. This distance was determined to be 5 centimetres, which was given the
constant "DISTANCE FROM CAR". The function turns on the motors, and drives forwards until the rear
ultrasonic sensor returns a value greater than "DISTANCE FROM CAR". If the rear ultrasonic sensor
already returns a value greater than "DISTANCE_FROM _CAR", the robot will not move forwards as was
already an appropriate distance away from the rear car. Finally, the function turns off the motors.

### toggleBanner
"toggleBanner" is responsible for both raising the successful completion flag, as well as lowering the
flag. The function accepts an integer parameter called "isRaised" which dictates whether "toggleBanner"
raises or lowers the flag. The parameter "isRaised" can be a value of "1', or "-1'. The "FLAG_MOTOR"
encoder is reset, where "FLAG_ MOTOR" is the defined term given to the motor attached to the flag
shaft. Power is then provided to "FLAG_MOTOR", with its value being the product of "isRaised" and the
constant integer "BANNER_SPEED". In this case, "BANNER_SPEED" is the constant integer determined to
be the speed in which the banner is raised. By multiplying "isRaised" by "BANNER _SPEED", both
clockwise and counterclockwise rotations are able to be facilitated without using decisions. In both
cases, "FLAG_MOTOR" is rotated until the absolute value of the encoder value of the "FLAG_MOTOR" is
equal to the constant "BANNER _ANGLE" which was defined to be 90 degrees. By calling "toggleBanner"
twice in succession, one can raise the flag and lower the flag consecutively by altering the value of the
parameter of the function.
