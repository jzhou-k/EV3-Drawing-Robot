#include "EV3_FileIO.c";
TFileHandle fin;
const int SPOTS_PER_ROW = 136; // Number of possible dots in a row
const int ROWS = 176; // Max number of rows to draw
const float SIXTEENTH_INCH = 0.15875;
const float INIT_MOVE = 5.0; // cm distance from color sensor
void driveXAxis(float Dist);
void paperScroller(float Dist);
bool checkStart();
int toNextX(int curX);
void drawFile(string fileName);
void returnToXHome() ;
void dotPen();
bool correctPosition(float Dist);
void promptUser();
bool checkFile(string fileName);
void sensorMotorInit();

task main()
{
	sensorMotorInit();
	promptUser();
	returnToXHome();
	string name[2] = {"1.txt", "2.txt"};
	drawFile(name[0]);
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
	time1[T4] = 0;
	int initialEncoder = getMotorEncoder(motorA);
	int encoderLastCheck = initialEncoder;
	const float GEAR_RADIUS = 1.875; // 1.
	const float CM_TO_DEG = 180/(PI*GEAR_RADIUS);
	const float GEAR_RATIO = 25/1.0;
	float degreesToTravel = Dist*CM_TO_DEG;

	motor[motorA] = 30;

	while(nMotorEncoder(motorA) - initialEncoder < degreesToTravel*GEAR_RATIO)
	{
		if (time1[T4] > 1500)
		{
			time1[T4] = 0;
			if(encoderLastCheck - initialEncoder < 360)
			{
				displayBigTextLine(0, "X-Axis jam. Exiting...");
				playSound(soundException);
				wait1Msec(5000);
			}
		}
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

	motor[motorD] = -30;

	while(abs(nMotorEncoder(motorD) - initialEncoder) < degreesToTravel*GEAR_RATIO)
	{
	}

	motor[motorD] = 0;
}

bool checkStart()
{
	int dotPos = 0;
	readIntPC(fin, dotPos);
	if(dotPos == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}
int toNextX(int curX)
{
	int evalValue = 0; // Value read from text file; 0 or 1
	int evalX = curX+1;  // Current x-position being evaluated

	while (evalX < SPOTS_PER_ROW)
	{
		readIntPC(fin, evalValue);
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
void dotPen()
{
	time1[T4] = 0;
	motor[motorC] = 40;

	while(!SensorValue[S3])
	{
		if (time1[T4] > 5000)
		{
			displayBigTextLine(0, "Pen dislodged. Exiting...");
			playSound(soundException);
			wait1Msec(5000);
			return; // Later add exit draw function
		}
	}

	motor[motorC] = 0;

	motor[motorC] = -40;

	while(nMotorEncoder(motorC) > 0)
	{
	}

	motor[motorC] = 0;
}

void drawFile(string fileName)
{
	bool success = openReadPC(fin, fileName);
	if (!success)
	{
		displayString(0, "File In not working");
		wait1Msec(2000);
	}
	if(!correctPosition(INIT_MOVE))
	{
		return;
	}

	int curRow = 0;

	int drawStartTime = nPgmTime;

	while (curRow < ROWS)
	{
		int curCol = 0;
		int timeElapsed = nPgmTime - drawStartTime;
		if(checkStart())
		{
			dotPen();
		}
		while (curCol < SPOTS_PER_ROW)
		{
			int goTo = toNextX(curCol);
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

			if (time1[T1] > 500)
			{
				displayBigTextLine(0, "Time Elapsed: %0.2f s", timeElapsed);
				time1[T1] = 0;
			}
		}
		if (time1[T1] > 500)
		{
			displayBigTextLine(0, "Time Elapsed: %0.2f s", timeElapsed);
			time1[T1] = 0;
		}
		// reset X Pos
    returnToXHome();

		// Move down a row
		paperScroller(SIXTEENTH_INCH);
		curRow++;
	}

	closeFilePC(fin);
}

bool checkFile(string fileName)
{
	bool foundFile = openReadPC(fin, fileName);

	closeFilePC(fin);

	return foundFile;
}

bool correctPosition(float Dist)
{
	motor[motorD] = -30;
	time1[T4] = 0;
	while(SensorValue[S1] != (int)colorWhite)
	{
		if(time1[T4] > 1000*6)
		{
			motor[motorD] = 0;
			eraseDisplay();
			displayString(5,"ERROR");
			wait1Msec(10000);
			return false;
		}
	}
	motor[motorD] = 0;
	wait1Msec(1000);
	paperScroller(Dist);
	return true;
}
void promptUser()
{
	eraseDisplay();
  displayString(2,"PLACE PAPER ");
  displayString(3,"RIGHT UNDER ");
  displayString(4,"WHEEL");
  displayString(5,"PRESS ENTER");
  displayString(6,"ONCE IN CORRECT");
  displayString(7,"POSITION");
  while(!getButtonPress(ENTER_BUTTON))
  {}
	while(getButtonPress(ENTER_BUTTON))
	{}
	eraseDisplay();
}
void sensorMotorInit()
{
	SensorType[S2] = sensorEV3_Touch;
	wait1Msec(50);
	SensorType[S3] = sensorEV3_Touch;
	wait1Msec(50);
	SensorType[S1] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S1] = modeEV3Color_Color;
	wait1Msec(50);
	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorB] = 0;
	nMotorEncoder[motorD] = 0;
}
