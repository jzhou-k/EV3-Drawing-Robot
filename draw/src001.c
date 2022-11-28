#pragma once
#include "../lib/EV3_FileIO.c";
#include "../lib/ports.h";

TFileHandle fin;
int timeElapsed = 0;
int drawStartTime = 0;
const float MS_TO_S = 1 / 1000.0;
const int timerInterval = 250;
const int SPOTS_PER_ROW = 136; // Number of possible dots in a row
const int ROWS = 176;		   // Max number of rows to draw
const float SIXTEENTH_INCH = 0.15875;
const float INIT_MOVE = 5.0; // cm distance from color sensor
bool notExit;
void driveXAxis(float Dist);
void paperScroller(float Dist);
bool checkStart();
int toNextX(int curX);
void drawFile(string fileName);
void returnToXHome();
void dotPen();
bool correctPosition(float Dist);
void promptUser();
//void sensorMotorInit();
void checkStop();
void resetPen();

/*
task main()
{
	sensorMotorInit();
	string name[2] = {"1.txt", "2.txt"};
	drawFile(name[0]);
}
*/

void returnToXHome()
{
	while (SensorValue[xAxisLim] != 1 && notExit)
	{
		if (getButtonPress(ENTER_BUTTON))
		{
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
	time1[T4] = 0;
	int initialEncoder = getMotorEncoder(motorA);
	int encoderLastCheck = initialEncoder;
	const float GEAR_RADIUS = 1.875;
	const float CM_TO_DEG = 180 / (PI * GEAR_RADIUS);
	const float GEAR_RATIO = 25 / 1.0;
	float degreesToTravel = Dist * CM_TO_DEG;
	motor[motorA] = 30;
	while (nMotorEncoder(motorA) - initialEncoder < degreesToTravel * GEAR_RATIO && notExit)
	{
		timeElapsed = nPgmTime - drawStartTime;
		if (getButtonPress(ENTER_BUTTON))
		{
			notExit = false;
			break;
		}
		if (time1[T4] > 1500)
		{
			time1[T4] = 0;
			checkStop();
			encoderLastCheck = getMotorEncoder(motorA);
			if (encoderLastCheck - initialEncoder < 60 && notExit)
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

		if (time1[T1] > timerInterval && notExit)
		{
			eraseDisplay();
			displayBigTextLine(0, "Time: %0.2f s", timeElapsed * MS_TO_S);
			wait1Msec(10);
			time1[T1] = 0;
		}
	}
	motor[motorA] = 0;
	checkStop();
}

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

bool checkStart()
{
	checkStop();
	int dotPos = 0;
	checkStop();
	readIntPC(fin, dotPos);
	if (dotPos == 1)
	{
		return true;
	}
	return false;
}
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
		checkStop();
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
void dotPen()
{
	nMotorEncoder[zAxis] = 0;
	checkStop();
	time1[T4] = 0;
	checkStop();
	motor[zAxis] = 100;
	checkStop();

	while (!SensorValue[zAxisLim] && notExit)
	{
		timeElapsed = nPgmTime - drawStartTime;
		if (getButtonPress(ENTER_BUTTON))
		{
			notExit = false;
			break;
		}
		if (time1[T4] > 5000)
		{
			eraseDisplay();
			motor[motorC] = 0;
			displayBigTextLine(0, "Pen broke");
			playSound(soundException);
			wait1Msec(5000);
			notExit = false;
			break;
			// add exit draw function
		}
		if (time1[T1] > timerInterval && notExit)
		{
			eraseDisplay();
			displayBigTextLine(0, "Time: %0.2f s", timeElapsed * MS_TO_S);
			wait1Msec(10);
			time1[T1] = 0;
		}
	}
	motor[zAxis] = -100;
	checkStop();
	nMotorEncoder[zAxis] = 0;
	while (abs(nMotorEncoder[zAxis]) < 6 * 360)
	{
		timeElapsed = nPgmTime - drawStartTime;
		if (getButtonPress(ENTER_BUTTON))
		{
			notExit = false;
			break;
		}
		if (time1[T1] > timerInterval && notExit)
		{
			eraseDisplay();
			displayBigTextLine(0, "Time: %0.2f s", timeElapsed * MS_TO_S);
			wait1Msec(10);
			time1[T1] = 0;
		}
	}
	checkStop();
	motor[zAxis] = 0;
	checkStop();
}
int bs = 0;
void drawFile(string fileName)
{
	notExit = true;
	checkStop();
	bool success = openReadPC(fin, fileName);
	checkStop();
	string drawingName = "";
	int numPages = 0;
	readTextPC(fin, drawingName);
	readIntPC(fin, numPages);
	readIntPC(fin, bs);
	if (!success && notExit)
	{
		checkStop();
		displayString(0, "File In not working");
		checkStop();
		wait1Msec(2000);
		checkStop();
	}
	drawStartTime = nPgmTime;

	for (int curPage = 1; curPage <= numPages; curPage++)
	{
		dotPen();
		promptUser();
		if (!correctPosition(INIT_MOVE) && notExit)
		{
			checkStop();
			return;
		}
		returnToXHome();
		int curRow = 0;
		while (curRow < ROWS && notExit)
		{
			if (getButtonPress(ENTER_BUTTON))
			{
				closeFilePC(fin);
				notExit = false;
				break;
			}
			int curCol = 0;
			if (checkStart())
			{
				dotPen();
			}
			timeElapsed = nPgmTime - drawStartTime;
			while (curCol < SPOTS_PER_ROW && notExit)
			{
				timeElapsed = nPgmTime - drawStartTime;
				if (getButtonPress(ENTER_BUTTON))
				{
					closeFilePC(fin);
					notExit = false;
					wait1Msec(5000);
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
					dotPen();
				}
				else
				{
					curCol = SPOTS_PER_ROW;
				}

				if (time1[T1] > timerInterval && notExit)
				{
					eraseDisplay();
					displayBigTextLine(0, "Time: %0.2f s", timeElapsed * MS_TO_S);
					wait1Msec(10);
					time1[T1] = 0;
				}
			}
			if (time1[T1] > timerInterval && notExit)
			{
				eraseDisplay();
				displayBigTextLine(0, "Time: %0.2f s", timeElapsed * MS_TO_S);
				wait1Msec(10);
				time1[T1] = 0;
			}
			// reset X Pos
			returnToXHome();
			// Move down a row
			paperScroller(SIXTEENTH_INCH);
			curRow++;
		}
	}
	closeFilePC(fin);
	notExit = false;
	wait1Msec(5000);
}

bool correctPosition(float Dist)
{
	motor[motorD] = -30;
	time1[T4] = 0;
	while (SensorValue[paperSensor] != (int)colorWhite && notExit)
	{
		if (getButtonPress(ENTER_BUTTON))
		{
			closeFilePC(fin);
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
	checkStop();
	return true;
}
void promptUser()
{
	checkStop();
	eraseDisplay();
	displayString(2, "PLACE PAPER ");
	displayString(3, "RIGHT UNDER ");
	displayString(4, "WHEEL");
	displayString(5, "PRESS RIGHT");
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
/*
void sensorMotorInit()
{
	SensorType[xAxisLim] = sensorEV3_Touch; // x axis touch
	wait1Msec(50);
	SensorType[zAxisLim] = sensorEV3_Touch; // pen touch
	wait1Msec(50);
	SensorType[paperSensor] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[paperSensor] = modeEV3Color_Color;
	wait1Msec(50);
	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorB] = 0;
	nMotorEncoder[motorD] = 0;
}*/

void checkStop()
{
	if (getButtonPress(ENTER_BUTTON))
	{
		closeFilePC(fin);
		notExit = false;
	}
}

void resetPen()
{
	motor[zAxis] = 60;
	while(!SensorValue(zAxisLim))
	{}
	nMotorEncoder[zAxis] = 0;
	// lift the pen up by 3 cm
	motor[motorC] = -60;
	while (nMotorEncoder[zAxis] > 4 * 360)
	{}
	motor[zAxis] = 0;
}
