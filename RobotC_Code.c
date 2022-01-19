/* --------------------------- *\
	MTE 121 Project
	Parallel Parking Robot
	November 13, 2021

	Group 1
		Chelsea Dmytryk
		Dan Huynh
		Josh Blayone
		Sung-En Huang
\* --------------------------- */


// ----- Port Definitions -----
#define BACK_ULRTA_SENSOR S2
#define SIDE_ULTRA_SENSOR S3
#define GYRO_SENSOR S1
#define COLOR_SENSOR S4

#define LEFT_MOTOR motorA
#define RIGHT_MOTOR motorB
#define FLAG_MOTOR motorC

#define TIMER T1


// ----- Constants -----
const int LEFT_MOTOR_OFFSET = 0;
const int RIGHT_MOTOR_OFFSET = 0;

// Motor Speeds
const int DRIVING_SPEED = 20;
const int PARKING_SPEED = 10;
const int BANNER_SPEED = 10;
const int TURNING_SPEED = 10;

// Robot Constants
const int ROBOT_BODY_CM = 20;
const int ROBOT_WIDTH_CM = 14;
const float WHEEL_RADIUS = 2.75;

const int FORWARD = 1;
const int BACKWARD = -1;

const int RAISE = -1;
const int LOWER = 1;

// Program Settings
const int MAX_ATTEMPTS = 4;
const int MAX_TIME_FOR_ATTEMPT = 30 * 1000; // in milliseconds
const int MINIMUM_GAP_NEEDED = 20;			// in cm
const int DISTANCE_FROM_CAR = 10;			// in cm
const int BANNER_ANGLE = 90;				// in degrees
const int VALID_DISTANCE = 15;				// in cm


// ----- Robot Function Prototypes -----
void configureSensors();
void resetEncoder(tMotor selectedMotor);
void resetAllEncoders();
int getBackUltra();
int getSideUltra();
int getGyroAngle();
int getColor();
int getEncoder(tMotor selectedMotor);
void moveRobot(int speed); // Negitive speed will move robot backwards
void stopRobot();
void rotateRobot(int angle); // Negitive angle will move robot counter-clockwise
void driveDistanceCM(float distanceCM, int direction);


// ----- Non-Trivial Robot Function Prototypes -----
bool findParkingSpaceAndPark();
void waitForGreenCard();
void searchForCar(bool lookingForCar);
void parallelPark();
void parkingLineUp();
void reverseIntoSpot(float distanceToCar);
void pullForward();
void toggleBanner(int isRaised);


// ----- Main Program -----
task main()
{
	// Setup Robot
	displayString(4, "MTE 121 Project - Group 1");
	displayString(5, "Parallel Parking Robot");
	displayString(7, "Chelsea Dmytryk, Dan Huynh");
	displayString(8, "Josh Blayone, Sung-En Huang");

	configureSensors();
	resetAllEncoders();
	time1[TIMER] = 0;

	// Program
	waitForGreenCard();

	bool successfulParking = findParkingSpaceAndPark();

	if (successfulParking)
	{
		toggleBanner(RAISE);
		wait1Msec(2000);
		toggleBanner(LOWER);
	}
	else
	{
		eraseDisplay();
		displayString(5, "Unable to Find Parking Spot");
	}
	wait1Msec(1000);
}


// ----- Non-Trivial Robot Functions -----
bool findParkingSpaceAndPark()
{
	const float encToCM = (WHEEL_RADIUS * PI);
	float gapDistance = 0;

	int carCount = 0;
	bool finished = false;

	clearTimer(TIMER);

	while (carCount < MAX_ATTEMPTS && !finished)
	{
		moveRobot(DRIVING_SPEED);

		searchForCar(true);

		if (time1[TIMER] < MAX_TIME_FOR_ATTEMPT)
		{
			searchForCar(false);

			resetAllEncoders();

			searchForCar(true);

			stopRobot();
			gapDistance = (abs(getEncoder(RIGHT_MOTOR)) / 360.0) * encToCM;
		}

		if (gapDistance > MINIMUM_GAP_NEEDED)
		{
			parallelPark();
			finished = true;
		}
		else if (time1[TIMER] > MAX_TIME_FOR_ATTEMPT)
		{
			return finished;
		}
		else
		{
			carCount++;
			wait1Msec(1000);
		}
	}

	return finished;
}

void waitForGreenCard()
{
	while (getColor() != (int)colorGreen)
	{}
}

void searchForCar(bool lookingForCar)
{
	if (lookingForCar)
	{
		while (getSideUltra() > VALID_DISTANCE && time1[TIMER] < MAX_TIME_FOR_ATTEMPT)
		{}
	}
	else
	{
		while (!(getSideUltra() > VALID_DISTANCE) && time1[TIMER] < MAX_TIME_FOR_ATTEMPT)
		{}
	}
	if (time1[TIMER] > MAX_TIME_FOR_ATTEMPT)
	{
		stopRobot();
	}
}

void parallelPark()
{
	float distanceToCar = getSideUltra();

	parkingLineUp();
	wait1Msec(1000);
	reverseIntoSpot(distanceToCar);
	pullForward();
}

void parkingLineUp()
{
	moveRobot(20);
	resetEncoder(LEFT_MOTOR);

	driveDistanceCM(ROBOT_BODY_CM / 3, FORWARD);
	moveRobot(0);
}

void reverseIntoSpot(float distanceToCar)
{
	rotateRobot(45);
	wait1Msec(1000);

	driveDistanceCM(ROBOT_WIDTH_CM + distanceToCar, BACKWARD);
	stopRobot();
	wait1Msec(1000);

	rotateRobot(-45);
	pullForward();
}

void pullForward()
{
	moveRobot(PARKING_SPEED);
	while (getBackUltra() < DISTANCE_FROM_CAR)
	{}
	stopRobot();
}

void toggleBanner(int isRaised)
{
	resetEncoder(FLAG_MOTOR);

	motor[FLAG_MOTOR] = isRaised * BANNER_SPEED;

	while (abs(getEncoder(FLAG_MOTOR)) < abs(BANNER_ANGLE))
	{}

	motor[FLAG_MOTOR] = 0;
}


// ----- Robot Functions -----
void moveRobot(int speed)
{
	// "-" speed because motors are backwards
	motor[LEFT_MOTOR] = -(speed + LEFT_MOTOR_OFFSET);
	motor[RIGHT_MOTOR] = -(speed + RIGHT_MOTOR_OFFSET);
}

void stopRobot()
{
	moveRobot(0);
}

void driveDistanceCM(float distanceCM, int direction)
{
	float cmToDeg = distanceCM * 180 / (PI * 2.75);
	resetAllEncoders();

	moveRobot(direction * PARKING_SPEED);

	while (abs(getEncoder(LEFT_MOTOR)) < cmToDeg)
	{}
	stopRobot();
}

void rotateRobot(int angle)
{
	resetGyro(GYRO_SENSOR);
	if (angle > 0)
	{
		motor[LEFT_MOTOR] = TURNING_SPEED;
	}
	else
	{
		motor[RIGHT_MOTOR] = TURNING_SPEED;
	}

	while (abs(getGyroAngle()) < abs(angle))
	{}
	stopRobot();
}

void configureSensors()
{
	SensorType[BACK_ULRTA_SENSOR] = sensorEV3_Ultrasonic;
	SensorType[SIDE_ULTRA_SENSOR] = sensorEV3_Ultrasonic;
	SensorType[GYRO_SENSOR] = sensorEV3_Gyro;
	SensorType[COLOR_SENSOR] = sensorEV3_Color;
	SensorMode[COLOR_SENSOR] = modeEV3Color_Color;

	wait1Msec(50);
	SensorMode[GYRO_SENSOR] = modeEV3Gyro_Calibration;
	wait1Msec(50);
	SensorMode[GYRO_SENSOR] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);

	SensorMode[COLOR_SENSOR] = modeEV3Color_Color;
	wait1Msec(50);
}

void resetEncoder(tMotor selectedMotor)
{
	nMotorEncoder[selectedMotor] = 0;
}

void resetAllEncoders()
{
	resetEncoder(LEFT_MOTOR);
	resetEncoder(RIGHT_MOTOR);
	resetEncoder(FLAG_MOTOR);
}

int getBackUltra()
{
	return SensorValue[BACK_ULRTA_SENSOR];
}

int getSideUltra()
{
	return SensorValue[SIDE_ULTRA_SENSOR];
}

int getGyroAngle()
{
	return SensorValue[GYRO_SENSOR];
}

int getColor()
{
	return SensorValue[COLOR_SENSOR];
}

int getEncoder(tMotor selectedMotor)
{
	return nMotorEncoder[selectedMotor];
}
