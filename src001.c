#include "EV3_FileIO.c";
TFileHandle fin;
const int SPOTS_PER_ROW = 136; // Number of possible dots in a row
const int ROWS = 176;		   // Max number of rows to draw
const float SIXTEENTH_INCH = 0.15875;
const float INIT_MOVE = 5.0; // cm distance from color sensor

// function prototypes
bool notExit = true;
void driveXAxis(float Dist);
void paperScroller(float Dist);
bool checkStart();
int toNextX(int curX);
void drawFile(string fileName);
void returnToXHome();
void dotPen();
bool correctPosition(float Dist);
void promptUser();
bool checkFile(string fileName);
void sensorMotorInit();
void checkStop();
void positionPen();

task main()
{

	sensorMotorInit();
	checkStop();
	promptUser();
	checkStop(); ///??? do you need all these checkstops? 
	returnToXHome();
	checkStop();
	string name[2] = {"1.txt", "2.txt"};
	checkStop();
	drawFile(name[0]);
	checkStop();
}


// initialize all sensor and motors 
void sensorMotorInit()
{
	SensorType[S2] = sensorEV3_Touch; // x axis touch
	wait1Msec(50);
	SensorType[S3] = sensorEV3_Touch; // pen touch
	wait1Msec(50);
	SensorType[S1] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S1] = modeEV3Color_Color;
	wait1Msec(50);
	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorB] = 0;
	nMotorEncoder[motorD] = 0;
}



// lift the pen if it is touching the page
void positionPen()
{
	nMotorEncoder[motorC] = 0
	if (SensorValue[S3])
	{
		//lift the pen up by 3 cm 
		motor[motorC] = -40;
		while (nMotorEncoder[motorC] > (1.1811/SIXTEENTH_INCH))
		{
		}
	}

		
}

void returnToXHome()
{
	while (SensorValue[S2] != 1 && notExit)
	{
		if (getButtonPress(ENTER_BUTTON))
		{
			notExit = false;
			break;
		}
		motor[motorA] = -50;
		checkStop();
	}
	motor[motorA] = 0;
	checkStop();
	nMotorEncoder[motorA] = 0;
	checkStop();
}
// 5:3 --> 25:9 --> For every 25 deg motor encoder, 9 deg gear
//  38 mm diameter
void driveXAxis(float Dist)
{
	checkStop();
	time1[T4] = 0;
	int initialEncoder = getMotorEncoder(motorA);
	int encoderLastCheck = initialEncoder;

	// should declear this as global?
	const float GEAR_RADIUS = 1.875; // 1.
	const float CM_TO_DEG = 180 / (PI * GEAR_RADIUS);
	const float GEAR_RATIO = 25 / 1.0;
	float degreesToTravel = Dist * CM_TO_DEG;

	motor[motorA] = 30;

	// what the fuck is this code
	//  keep powering motor until it gets to the distance desired
	while (nMotorEncoder(motorA) - initialEncoder < degreesToTravel * GEAR_RATIO)
	{
		// Error checking code
		if (time1[T4] > 1500)
		{
			time1[T4] = 0;
			// if encoder has not moved, plays error sound and exit sound
			if (encoderLastCheck - initialEncoder < 360)
				checkStop();

			while (nMotorEncoder(motorA) - initialEncoder < degreesToTravel * GEAR_RATIO && notExit)
			{

				// if user press enter button, exit program
				if (getButtonPress(ENTER_BUTTON))
				{
					notExit = false;
					break;
				}

				// every 1.5 sec check if encoder has not moved, plays error sound and exit sound  (is the timer value correct?)
				if (time1[T4] > 1500)
				{
					time1[T4] = 0;
					checkStop();
					encoderLastCheck = nMotorEncoder[motorA];
					if (encoderLastCheck - initialEncoder < 360 && notExit)
					{
						displayBigTextLine(0, "X-Axis jam. Exiting...");
						playSound(soundException);
						// wait1Msec(5000);
						checkStop();
					}
				}
			}

			motor[motorA] = 0;
			checkStop();
		}
	}
}
// Rolls paper by distance passed in parameter.
void paperScroller(float Dist)
{
	checkStop();
	int initialEncoder = getMotorEncoder(motorD);
	const float WHEEL_RADIUS = 2.8; // May need to be updated
	const float CM_TO_DEG = 180 / (PI * WHEEL_RADIUS);
	const float GEAR_RATIO = 15;
	float degreesToTravel = Dist * CM_TO_DEG;
	checkStop();
	motor[motorD] = -30;
	checkStop();
	while (abs(nMotorEncoder(motorD) - initialEncoder) < degreesToTravel * GEAR_RATIO && notExit)
	{
		if (getButtonPress(ENTER_BUTTON))
		{
			notExit = false;
			break;
		}
	}
	checkStop();
	motor[motorD] = 0;
	checkStop();
}

// check if the beginning of the file should be dotted
bool checkStart()
{
	checkStop();
	int dotPos = 0;
	readIntPC(fin, dotPos);

	checkStop();
	if (dotPos == 1)
	{
		checkStop();
		return true;
	}

	return false;
	checkStop();
}

// returns the unit from current position to the next dotting position
int toNextX(int curX)
{
	checkStop();
	int evalValue = 0;	  // Value read from text file; 0 or 1
	int evalX = curX + 1; // Current x-position being evaluated
	checkStop();

	while (evalX < SPOTS_PER_ROW && notExit)
	{
		if (getButtonPress(ENTER_BUTTON))
		{
			notExit = false;
			break;
		}

		// keep reading file until it finds the next dotting position
		readIntPC(fin, evalValue);
		if (evalValue == 1)
		{
			checkStop();
			return (evalX - curX);
		}
		else
		{
			checkStop();
			evalX++;
		}
		checkStop();
	}
	checkStop();
	return -1; // Signal no dot found in remaining row
}

// Dots pen on page
void dotPen()
{
	checkStop();
	time1[T4] = 0;
	motor[motorC] = 40;
	checkStop();

	// while touch sensor haven't touched the paper, keep moving pen down
	while (!SensorValue[S3] && notExit)
	{

		if (getButtonPress(ENTER_BUTTON))
		{
			notExit = false;
			break;
		}

		// error checking, if this loop is excuting for more than 5 secs, assume pen is stuck and display error message
		if (time1[T4] > 5000)
		{
			checkStop();
			displayBigTextLine(0, "Pen dislodged. Exiting...");
			playSound(soundException);
			checkStop();
			wait1Msec(5000);
			checkStop();
		}
	}

	motor[motorC] = 0;
	motor[motorC] = -40;
	checkStop();

	// keep rotating the motor to left the pen up until returned to original position
	while (nMotorEncoder(motorC) > 0)
	{
		if (getButtonPress(ENTER_BUTTON))
		{
			notExit = false;
			break;
		}
		checkStop();
	}

	motor[motorC] = 0;
	checkStop();
}

void drawFile(string fileName)
{
	checkStop();
	bool success = openReadPC(fin, fileName);
	checkStop();
	if (!success && notExit)
	{
		checkStop();
		displayString(0, "File In not working");
		checkStop();
		wait1Msec(2000);
		checkStop();
	}
	if (!correctPosition(INIT_MOVE) && notExit)
	{
		checkStop();
		return;
	}
	checkStop();
	
	int curRow = 0;
	int drawStartTime = nPgmTime; //using program start time to calculate time elapsed 

	positionPen(); 

	while (curRow < ROWS && notExit)
	{
		if (getButtonPress(ENTER_BUTTON))
		{
			notExit = false;
			break;
		}
		checkStop();
		int curCol = 0;
		checkStop();
		int timeElapsed = nPgmTime - drawStartTime;
		checkStop();
		if (checkStart())
		{
			checkStop();
			dotPen();
		}
		checkStop();
		while (curCol < SPOTS_PER_ROW && notExit)
		{
			if (getButtonPress(ENTER_BUTTON))
			{
				notExit = false;
				break;
			}
			checkStop();
			int goTo = toNextX(curCol);
			checkStop();
			curCol += goTo;
			checkStop();
			if (goTo != -1 && notExit)
			{
				checkStop();
				driveXAxis(SIXTEENTH_INCH * goTo);
				checkStop();
				dotPen();
				checkStop();
			}
			else
			{
				checkStop();
				curCol = SPOTS_PER_ROW;
			}

			if (time1[T1] > 500 && notExit)
			{
				checkStop();
				displayBigTextLine(0, "Time Elapsed: %0.2f s", timeElapsed);
				checkStop();
				wait10Msec(1);
				checkStop();
				time1[T1] = 0;
				checkStop();
			}
		}
		if (time1[T1] > 500 && notExit)
		{
			checkStop();
			displayBigTextLine(0, "Time Elapsed: %0.2f s", timeElapsed);
			checkStop();
			wait10Msec(1);
			checkStop();
			time1[T1] = 0;
			checkStop();
		}
		// reset X Pos
		checkStop();
		returnToXHome();
		checkStop();
		// Move down a row
		checkStop();
		paperScroller(SIXTEENTH_INCH);
		checkStop();
		curRow++;
		checkStop();
	}
	checkStop();
	closeFilePC(fin);
	checkStop();
	notExit = false;
}

//check if file exits 
bool checkFile(string fileName)
{
	checkStop();
	bool foundFile = openReadPC(fin, fileName);
	checkStop();
	closeFilePC(fin);
	checkStop();
	return foundFile;
	checkStop();
}



//
bool correctPosition(float Dist)
{
	checkStop();
	motor[motorD] = -30;
	time1[T4] = 0;
	checkStop();

	while (SensorValue[S1] != (int)colorWhite && notExit)
	{
		if (getButtonPress(ENTER_BUTTON))
		{
			notExit = false;
			break;
		}
		checkStop();
		if (time1[T4] > 1000 * 6 && notExit)
		{
			checkStop();
			motor[motorD] = 0;
			checkStop();
			eraseDisplay();
			checkStop();
			displayString(5, "ERROR");
			checkStop();
			wait1Msec(10000);
			checkStop();
			return false;
			checkStop();
		}
	}
	checkStop();
	motor[motorD] = 0;
	checkStop();
	wait1Msec(1000);
	checkStop();
	paperScroller(Dist);
	return true;
}


//prompts the user to load the paper correctly, then waits for user to press right button to start 
//the drawing function 
void promptUser()
{
	checkStop();
	eraseDisplay();
	displayString(2, "PLACE PAPER ");
	displayString(3, "RIGHT UNDER ");
	displayString(4, "WHEEL");
	displayString(5, "PRESS ENTER");
	displayString(6, "ONCE IN CORRECT");
	displayString(7, "POSITION");
	checkStop();
	while (!getButtonPress(RIGHT_BUTTON) && notExit)
	{
		checkStop();
	}
	while (getButtonPress(RIGHT_BUTTON) && notExit)
	{
		checkStop();
	}
	checkStop();
	eraseDisplay();
	checkStop();
}


//check if user presses the enter button, then exit the program 
void checkStop()
{
	if (getButtonPress(ENTER_BUTTON))
	{
		notExit = false;
	}
}
