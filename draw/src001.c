#pragma once
#include "../lib/EV3_FileIO.c";
#include "../lib/ports.h";

TFileHandle fin;
int timeElapsed = 0; // Time elapsed since start of drawing
int drawStartTime = 0; // Start of drawing
const float MS_TO_S = 1 / 1000.0;
const int timerInterval = 250; // Display timer interval
const int SPOTS_PER_ROW = 136; // Number of possible dots in a row
const int ROWS = 176;		   // Max number of rows to draw
const float SIXTEENTH_INCH_CM = 0.15875; // Sixteenth of an inch in cm
const float INIT_MOVE = 5.0; // cm distance from color sensor
bool notExit; // global variable that controls exiting the drawing loop

// Function prototypes
void returnToXHome();
void driveXAxis(float Dist);
void paperScroller(float Dist);
bool checkStart();
int toNextX(int curX);
void dotPen();
void drawFile(string fileName);
bool correctPosition(float Dist);
void promptUser();
void displayElapsed();

/*
Returns the x-axis mover to the starting position
*/
void returnToXHome()
{
	while (SensorValue[xAxisLim] != 1 && notExit)
	{
		if (getButtonPress(ENTER_BUTTON))
		{
			closeFilePC(fin);
			notExit = false;
			wait1Msec(5000); // Required to ensure proper exit
			break;
		}
		motor[motorA] = -50;
	}
	motor[motorA] = 0;
	nMotorEncoder[motorA] = 0;
}

/*
Move the x-axis mover "Dist" amount, where dist is in CM
*/
void driveXAxis(float Dist)
{
	time1[T4] = 0; // For error checking
	int initialEncoder = getMotorEncoder(motorA);
	const float GEAR_RADIUS = 1.875; // The gear radius in cm
	const float CM_TO_DEG = 180 / (PI * GEAR_RADIUS);
	const float GEAR_RATIO = 25; // 25:1 gear ratio
	float degreesToTravel = Dist * CM_TO_DEG;
	motor[motorA] = 30;

	while (nMotorEncoder(motorA) - initialEncoder < degreesToTravel * GEAR_RATIO && notExit)
	{
		// Take difference to find the time elapsed since drawing started
		timeElapsed = nPgmTime - drawStartTime;

		// check for program exit
		if (getButtonPress(ENTER_BUTTON))
		{
			closeFilePC(fin);
			notExit = false;
			wait1Msec(5000);
			break;
		}

		// For checking jam after 15 seconds
		if (time1[T4] > 1500)
		{
			time1[T4] = 0;

			// Check if encoder has not changed by atleast 60 degrees
			if (nMotorEncoder(motorA) - initialEncoder < 60 && notExit)
			{
				eraseDisplay();
				displayBigTextLine(0, "X-Axis jam. Exiting...");
				wait1Msec(10);
				playSound(soundException);
				wait1Msec(5000);
				notExit = false;
				break;
			}
		}

		// For displaying draw time
		displayElapsed();
	}
	motor[motorA] = 0;
}

/*
Turns the wheels "Dist" amount (in CM) to
move paper "Dist" amount
*/
void paperScroller(float Dist)
{
	int initialEncoder = getMotorEncoder(motorD);
	const float WHEEL_RADIUS = 2.8;
	const float CM_TO_DEG = 180 / (PI * WHEEL_RADIUS);
	const float GEAR_RATIO = 15; // 15:1 gear ratio
	float degreesToTravel = Dist * CM_TO_DEG;
	motor[motorD] = -30;

	while (abs(nMotorEncoder(motorD) - initialEncoder) < degreesToTravel * GEAR_RATIO && notExit)
	{
		// check for program exit
		if (getButtonPress(ENTER_BUTTON))
		{
			closeFilePC(fin);
			notExit = false;
			wait1Msec(5000);
			break;
		}
	}
	motor[motorD] = 0;
}

/* check if current position is to be dotted
*/
bool checkStart()
{
	int dotPos = 0;
	readIntPC(fin, dotPos);
	if (dotPos == 1)
	{
		return true;
	}
	return false;
}

/* Find the next dot in the row, if it does not exit return -1
*/
int toNextX(int curX)
{
	int evalValue = 0;	  // Value read from text file; 0 or 1
	int evalX = curX + 1; // Current x-position being evaluated in file

	while (evalX < SPOTS_PER_ROW && notExit)
	{
		// check for program exit
		if (getButtonPress(ENTER_BUTTON))
		{
			closeFilePC(fin);
			notExit = false;
			wait1Msec(5000);
			break;
		}

		readIntPC(fin, evalValue);
		// if found dot to print
		if (evalValue == 1)
		{
			return (evalX - curX);
		}
		else
		{
			evalX++;
		}
	}
	return -1; // Signal no dot found in remaining row
}

/*
Moves the pen down until the touch sensor is
triggered, at which point the pen will retract
5/16 of an inch
*/
void dotPen()
{
	nMotorEncoder[zAxis] = 0;
	time1[T4] = 0;
	motor[zAxis] = 60;

	// while touch sensor not pressed
	while (!SensorValue[zAxisLim] && notExit)
	{
		timeElapsed = nPgmTime - drawStartTime;

		// check for program exit
		if (getButtonPress(ENTER_BUTTON))
		{
			closeFilePC(fin);
			notExit = false;
			wait1Msec(5000);
			break;
		}
		// If after 5 seconds it is unable to complete the task
		if (time1[T4] > 5000)
		{
			eraseDisplay();
			motor[motorC] = 0;
			displayBigTextLine(0, "Pen broke");
			playSound(soundException);
			wait1Msec(5000);
			notExit = false;
			break;
		}

		// For drawing time display
		displayElapsed();
	}
	motor[zAxis] = -60;
	nMotorEncoder[zAxis] = 0;
	// For retracting 5/16 of an inch since one
	// rotation is 1/16 of an inch
	while (abs(nMotorEncoder[zAxis]) < 5 * 360)
	{
		timeElapsed = nPgmTime - drawStartTime;

		// Check to exit program
		if (getButtonPress(ENTER_BUTTON))
		{
			closeFilePC(fin);
			notExit = false;
			wait1Msec(5000);
			break;
		}
		displayElapsed();
	}
	motor[zAxis] = 0;
}

int temp = 0; // unused temporary holder
/*
Loops through each row and column of the drawing
Calls all the functions required to draw
*/
void drawFile(string fileName)
{
	notExit = true;
	bool success = openReadPC(fin, fileName);
	string drawingName = "";
	int numPages = 0;
	readTextPC(fin, drawingName);
	readIntPC(fin, numPages);
	readIntPC(fin, temp); // Required to ensure file is read properly

	if (!success && notExit)
	{
		displayString(0, "File In not working");
		wait1Msec(2000);
	}

	// Mark the time when the drawing starts
	drawStartTime = nPgmTime;

	// For each page required
	for (int curPage = 1; curPage <= numPages; curPage++)
	{
		dotPen();
		promptUser();
		if (!correctPosition(INIT_MOVE) && notExit)
		{
			return;
		}
		returnToXHome();
		int curRow = 0;

		// For each row in a page
		while (curRow < ROWS && notExit)
		{

			// Check to exit program
			if (getButtonPress(ENTER_BUTTON))
			{
				closeFilePC(fin);
				notExit = false;
				wait1Msec(5000);
				break;
			}

			int curCol = 0;
			if (checkStart())
			{
				dotPen();
			}
			// Find the time passed since start
			timeElapsed = nPgmTime - drawStartTime;
			// For each column in a row
			while (curCol < SPOTS_PER_ROW && notExit)
			{
				timeElapsed = nPgmTime - drawStartTime;

				// Check to exit program
				if (getButtonPress(ENTER_BUTTON))
				{
					closeFilePC(fin);
					notExit = false;
					wait1Msec(5000);
					break;
				}

				int goTo = toNextX(curCol);
				curCol += goTo;
				if (goTo != -1 && notExit)
				{
					driveXAxis(SIXTEENTH_INCH_CM * goTo);
					dotPen();
				}
				else
				{
					curCol = SPOTS_PER_ROW;
				}

				displayElapsed();
			}
			displayElapsed();
			// reset X Pos
			returnToXHome();
			// Move down a row
			paperScroller(SIXTEENTH_INCH_CM);
			curRow++;
		}
	}
	closeFilePC(fin);
	notExit = false;
	// To ensure EV3 exits the program safely
	wait1Msec(5000);
}

/*
Checks if paper is properly placed in position
*/
bool correctPosition(float Dist)
{
	motor[motorD] = -30;
	time1[T4] = 0;
	while (SensorValue[paperSensor] != (int)colorWhite && notExit)
	{
		// Check for exit drawing
		if (getButtonPress(ENTER_BUTTON))
		{
			closeFilePC(fin);
			notExit = false;
			wait1Msec(5000);
			break;
		}

		// If after 6 seconds the sensor is unable to detect thepaper
		if (time1[T4] > 1000 * 6 && notExit)
		{
			motor[motorD] = 0;
			eraseDisplay();
			displayString(5, "ERROR");
			playSound(soundException);
			wait1Msec(10000);
			return false;
		}
	}
	motor[motorD] = 0;
	wait1Msec(1000);
	paperScroller(Dist);
	return true;
}

// Tell user to put paper under wheels
void promptUser()
{
	eraseDisplay();
	displayString(2, "PLACE PAPER ");
	displayString(3, "RIGHT UNDER ");
	displayString(4, "WHEEL");
	displayString(5, "PRESS RIGHT");
	displayString(6, "ONCE IN CORRECT");
	displayString(7, "POSITION");
	while (!getButtonPress(RIGHT_BUTTON) && notExit)
	{
	}
	while (getButtonPress(RIGHT_BUTTON) && notExit)
	{
	}
	eraseDisplay();
}

void displayElapsed()
{
	if (time1[T1] > timerInterval && notExit)
		{
			eraseDisplay();
			displayBigTextLine(0, "Time: %0.2f s", timeElapsed * MS_TO_S);
			wait1Msec(10);
			time1[T1] = 0;
		}
}
