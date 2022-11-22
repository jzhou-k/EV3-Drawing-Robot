#include "EV3_FileIO.c";
TFileHandle fin;
const int SPOTS_PER_ROW = 136; // Number of possible dots in a row
const int ROWS = 176; // Max number of rows to draw
const float SIXTEENTH_INCH = 0.15875;
void driveXAxis(float Dist);
void paperScroller(float Dist);
int toNextX(int curX);
void returnToXHome() ;
void dotPen();

task main()
{
	SensorType[S2] = sensorEV3_Touch;
	SensorType[S3] = sensorEV3_Touch;
	returnToXHome();
	//paperScroller(0.15875); // Scroll Paper: 1/16 inch
	bool success = openReadPC(fin, "1.txt");
	if (!success)
	{
		displayString(0, "File In not working");
		wait1Msec(2000);
	}

	int curRow = 0;

	while (curRow < ROWS)
	{
		int curCol = 0;
		while (curCol < SPOTS_PER_ROW)
		{
			int goTo = toNextX(curCol);
			displayString(0, "%d --> %d", curCol, curCol+goTo);
			wait1Msec(500);
			curCol += goTo;
			if (goTo != -1)
			{
				driveXAxis(SIXTEENTH_INCH*goTo);
				dotPen();
			}
			else
			{
				curCol = SPOTS_PER_ROW;
			}
		}
		// reset X Pos
    returnToXHome();

		// Move down a row
		paperScroller(SIXTEENTH_INCH);
		curRow++;
	}
}

void returnToXHome()
{
	while(SensorValue[S2] != 1)
	{
	    motor[motorA] = -50;
	}
	motor[motorA] = 0;
	nMotorEncoder[motorA] = 0;
}
//5:3 --> 25:9 --> For every 25 deg motor encoder, 9 deg gear
// 38 mm diameter
void driveXAxis(float Dist)
{
	int initialEncoder = getMotorEncoder(motorA);
	const float GEAR_RADIUS = 1.875; // 1.9
	const float CM_TO_DEG = 180/(PI*GEAR_RADIUS);
	const float GEAR_RATIO = 25/1.0;
	float degreesToTravel = Dist*CM_TO_DEG;

	motor[motorA] = 30;

	while(nMotorEncoder(motorA) - initialEncoder < degreesToTravel*GEAR_RATIO)
	{
	}

	motor[motorA] = 0;
}

void paperScroller(float Dist)
{
	int initialEncoder = getMotorEncoder(motorD);
	const float WHEEL_RADIUS = 2.8; // May need to be updated
	const float CM_TO_DEG = 180/(PI*WHEEL_RADIUS);
	const float GEAR_RATIO = 15;
	float degreesToTravel = Dist*CM_TO_DEG;

	motor[motorD] = 30;

	while(nMotorEncoder(motorD) - initialEncoder < degreesToTravel*GEAR_RATIO)
	{
	}

	motor[motorD] = 0;
}
int toNextX(int curX)
{
	int evalValue = 0; // Value read from text file; 0 or 1
	int evalX = curX;  // Current x-position being evaluated
	bool foundDot = false;

	while (evalX < SPOTS_PER_ROW && !foundDot)
	{
		readIntPC(fin, evalValue);
		if (evalValue == 1)
		{
			foundDot = true;
	  }
	  else
	  {
	  	evalX++;
		}
	}
	if (foundDot)
	{
		return (evalX - curX);
	}
	else
	{
		return -1; // Signal no dot found in remaining row
	}
}
void dotPen()
{
	motor[motorC] = 40;

	while(!SensorValue[S3])
	{
	}

	motor[motorC] = 0;

	motor[motorC] = -40;

	while(nMotorEncoder(motorC) > 0)
	{
	}

	motor[motorC] = 0;
}
