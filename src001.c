#include "EV3_FileIO.c";
TFileHandle fin;
const int SPOTS_PER_ROW = 136; // Number of possible dots in a row
const float SIXTEENTH_INCH = 0.15875;
void driveXAxis(float Dist);
void paperScroller(float Dist);
int toNextX(int curX);
task main()
{
	//paperScroller(0.15875); // Scroll Paper: 1/16 inch
	bool success = openReadPC(fin, "test_inputs.txt");
	if (!success)
	{
		displayString(0, "File In not working");
		wait1Msec(2000);
	}

	int curCol = 0;
	int goTo = toNextX(curCol);
	displayString(0, "GoTo: %f", goTo);
	wait1Msec(5000);
	curCol += goTo;
	driveXAxis(SIXTEENTH_INCH*goTo);
}

//5:3 --> 25:9 --> For every 25 deg motor encoder, 9 deg gear
// 38 mm diameter
void driveXAxis(float Dist)
{
	//Dist *= (1/0.925);
	nMotorEncoder(motorA) = 0;
	const float GEAR_RADIUS = 1.875; // 1.9
	const float CM_TO_DEG = 180/(PI*GEAR_RADIUS);
	const float GEAR_RATIO = 25/1.0;
	float degreesToTravel = Dist*CM_TO_DEG;

	motor[motorA] = 10;

	while(nMotorEncoder(motorA) < degreesToTravel*GEAR_RATIO)
	{
	}

	motor[motorA] = 0;
}

void paperScroller(float Dist)
{
	nMotorEncoder(motorA) = 0;
	const float WHEEL_RADIUS = 2.8; // May need to be updated
	const float CM_TO_DEG = 180/(PI*WHEEL_RADIUS);
	const float GEAR_RATIO = 15;
	float degreesToTravel = Dist*CM_TO_DEG;

	motor[motorA] = 30;

	while(nMotorEncoder(motorA) < degreesToTravel*GEAR_RATIO)
	{
	}

	motor[motorA] = 0;
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

	return (evalX - curX);
}
