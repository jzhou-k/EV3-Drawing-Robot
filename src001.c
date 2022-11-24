#include "EV3_FileIO.c";
TFileHandle fin;
const int SPOTS_PER_ROW = 136; // Number of possible dots in a row
const int ROWS = 176; // Max number of rows to draw
const float SIXTEENTH_INCH = 0.15875;
const float INIT_MOVE = 5.0; // cm distance from color sensor
bool notExit = true;
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
void checkStop();

task main()
{
		sensorMotorInit();
		checkStop();
		promptUser();
		checkStop();
		returnToXHome();
		checkStop();
		string name[2] = {"1.txt", "2.txt"};
		checkStop();
		drawFile(name[0]);
		checkStop();
}

void returnToXHome()
{
		while(SensorValue[S2] != 1 && notExit)
		{
			if(getButtonPress(ENTER_BUTTON))
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
//5:3 --> 25:9 --> For every 25 deg motor encoder, 9 deg gear
// 38 mm diameter
void driveXAxis(float Dist)
{
	time1[T4] = 0;
	checkStop();
	int initialEncoder = getMotorEncoder(motorA);
	checkStop();
	int encoderLastCheck = initialEncoder;
	checkStop();
	const float GEAR_RADIUS = 1.875; // 1.
	checkStop();
	const float CM_TO_DEG = 180/(PI*GEAR_RADIUS);
	checkStop();
	const float GEAR_RATIO = 25/1.0;
	checkStop();
	float degreesToTravel = Dist*CM_TO_DEG;
	checkStop();
	motor[motorA] = 30;
	checkStop();
	while(nMotorEncoder(motorA) - initialEncoder < degreesToTravel*GEAR_RATIO && notExit)
	{
		if(getButtonPress(ENTER_BUTTON))
		{
			notExit = false;
			break;
		}
		if (time1[T4] > 1500)
		{
			time1[T4] = 0;
			checkStop();
			encoderLastCheck = nMotorEncoder[motorA];
			if(encoderLastCheck - initialEncoder < 360 && notExit)
			{
				displayBigTextLine(0, "X-Axis jam. Exiting...");
				checkStop();
				playSound(soundException);
				checkStop();
				//wait1Msec(5000);
				checkStop();
			}
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
	const float CM_TO_DEG = 180/(PI*WHEEL_RADIUS);
	const float GEAR_RATIO = 15;
	float degreesToTravel = Dist*CM_TO_DEG;
	checkStop();
	motor[motorD] = -30;
	checkStop();
	while(abs(nMotorEncoder(motorD) - initialEncoder) < degreesToTravel*GEAR_RATIO && notExit)
	{
		if(getButtonPress(ENTER_BUTTON))
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
	checkStop();
	if(dotPos == 1)
	{
		checkStop();
		return true;
	}
	else
	{
		checkStop();
		return false;
	}
	checkStop();
}
int toNextX(int curX)
{
	checkStop();
	int evalValue = 0; // Value read from text file; 0 or 1
	int evalX = curX+1;  // Current x-position being evaluated
	checkStop();

	while (evalX < SPOTS_PER_ROW && notExit)
	{
		if(getButtonPress(ENTER_BUTTON))
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
	checkStop();
	time1[T4] = 0;
	checkStop();
	motor[motorC] = 40;
	checkStop();

	while(!SensorValue[S3] && notExit)
	{
		if(getButtonPress(ENTER_BUTTON))
		{
			notExit = false;
			break;
		}
		if (time1[T4] > 5000)
		{
			checkStop();
			displayBigTextLine(0, "Pen dislodged. Exiting...");
			checkStop();
			playSound(soundException);
			checkStop();
			wait1Msec(5000);
			checkStop();
			return; // Later add exit draw function
		}
	}
	checkStop();
	motor[motorC] = 0;
	checkStop();
	motor[motorC] = -40;
	checkStop();
	while(nMotorEncoder(motorC) > 0)
	{
		if(getButtonPress(ENTER_BUTTON))
		{
			notExit = false;
			break;
		}
		checkStop();
	}
	checkStop();
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
	if(!correctPosition(INIT_MOVE) && notExit)
	{
		checkStop();
		return;
	}
	checkStop();
	int curRow = 0;
	checkStop();
	int drawStartTime = nPgmTime;
	checkStop();
	while (curRow < ROWS && notExit)
	{
		if(getButtonPress(ENTER_BUTTON))
		{
			notExit = false;
			break;
		}
		checkStop();
		int curCol = 0;
		checkStop();
		int timeElapsed = nPgmTime - drawStartTime;
		checkStop();
		if(checkStart())
		{
			checkStop();
			dotPen();
		}
		checkStop();
		while (curCol < SPOTS_PER_ROW && notExit)
		{
			if(getButtonPress(ENTER_BUTTON))
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
				driveXAxis(SIXTEENTH_INCH*goTo);
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

bool correctPosition(float Dist)
{
	checkStop();
	motor[motorD] = -30;
	checkStop();
	time1[T4] = 0;
	checkStop();
	while(SensorValue[S1] != (int)colorWhite && notExit)
	{
		if(getButtonPress(ENTER_BUTTON))
		{
			notExit = false;
			break;
		}
		checkStop();
		if(time1[T4] > 1000*6 && notExit)
		{
			checkStop();
			motor[motorD] = 0;
			checkStop();
			eraseDisplay();
			checkStop();
			displayString(5,"ERROR");
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
    displayString(2,"PLACE PAPER ");
    displayString(3,"RIGHT UNDER ");
    displayString(4,"WHEEL");
    displayString(5,"PRESS ENTER");
    displayString(6,"ONCE IN CORRECT");
    displayString(7,"POSITION");
	checkStop();
    while(!getButtonPress(RIGHT_BUTTON) && notExit)
    {
		checkStop();
	}
	while(getButtonPress(RIGHT_BUTTON) && notExit)
    {
		checkStop();
	}
	checkStop();
	eraseDisplay();
	checkStop();
}
void sensorMotorInit()
{
	SensorType[S2] = sensorEV3_Touch; //x axis touch
	wait1Msec(50);
	SensorType[S3] = sensorEV3_Touch; //pen touch
	wait1Msec(50);
	SensorType[S1] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S1] = modeEV3Color_Color;
	wait1Msec(50);
	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorB] = 0;
	nMotorEncoder[motorD] = 0;
}

void checkStop()
{
	if(getButtonPress(ENTER_BUTTON))
	{
		notExit = false;
	}
}