#include "EV3_FileIO.c";
TFileHandle fin;
const int SPOTS_PER_ROW = 136; // Number of possible dots in a row
const int ROWS = 176;		   // Max number of rows to draw
const float SIXTEENTH_INCH = 0.15875;
const float INIT_MOVE = 5.0; // cm distance from color sensor

<<<<<<< HEAD
// function prototypes
bool notExit = true;
=======

//function prototypes  
>>>>>>> parent of 905b4dc (Merge branch 'main' of https://github.com/jzhou-k/EV3-Drawing-Robot)
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
<<<<<<< HEAD
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

		
=======

task main()
{
	sensorMotorInit();
	promptUser();
	returnToXHome();
	string name[2] = {"1.txt", "2.txt"};
	drawFile(name[0]);
>>>>>>> parent of 905b4dc (Merge branch 'main' of https://github.com/jzhou-k/EV3-Drawing-Robot)
}

void returnToXHome()
{
<<<<<<< HEAD
	while (SensorValue[S2] != 1 && notExit)
	{
		if (getButtonPress(ENTER_BUTTON))
		{
			notExit = false;
			break;
		}
		motor[motorA] = -50;
		checkStop();
=======
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

	//keep powering motor until it gets to the distance desired 
	while(nMotorEncoder(motorA) - initialEncoder < degreesToTravel*GEAR_RATIO)
	{	
		//Error checking code 
		if (time1[T4] > 1500)
		{
			time1[T4] = 0;
			//if encoder has not moved, plays error sound and exit sound 
			if(encoderLastCheck - initialEncoder < 360)
			{
				displayBigTextLine(0, "X-Axis jam. Exiting...");
				playSound(soundException);
				wait1Msec(5000);
			}
		}
>>>>>>> parent of 905b4dc (Merge branch 'main' of https://github.com/jzhou-k/EV3-Drawing-Robot)
	}

	motor[motorA] = 0;
<<<<<<< HEAD
	checkStop();
	nMotorEncoder[motorA] = 0;
	checkStop();
=======
>>>>>>> parent of 905b4dc (Merge branch 'main' of https://github.com/jzhou-k/EV3-Drawing-Robot)
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
	int initialEncoder = getMotorEncoder(motorD);
	const float WHEEL_RADIUS = 2.8; // May need to be updated
	const float CM_TO_DEG = 180 / (PI * WHEEL_RADIUS);
	const float GEAR_RATIO = 15;
<<<<<<< HEAD
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
=======
	float degreesToTravel = Dist*CM_TO_DEG;

	motor[motorD] = -30;
	
	while(abs(nMotorEncoder(motorD) - initialEncoder) < degreesToTravel*GEAR_RATIO)
	{
>>>>>>> parent of 905b4dc (Merge branch 'main' of https://github.com/jzhou-k/EV3-Drawing-Robot)
	}

	motor[motorD] = 0;
}

// check if the beginning of the file should be dotted
bool checkStart()
{
<<<<<<< HEAD
	checkStop();
	int dotPos = 0;
	readIntPC(fin, dotPos);

	checkStop();
	if (dotPos == 1)
=======
	readIntPC(fin, 0);
	if(dotPos == 1)
>>>>>>> parent of 905b4dc (Merge branch 'main' of https://github.com/jzhou-k/EV3-Drawing-Robot)
	{
		return true;
	}
<<<<<<< HEAD

=======
		
>>>>>>> parent of 905b4dc (Merge branch 'main' of https://github.com/jzhou-k/EV3-Drawing-Robot)
	return false;

}

// returns the unit from current position to the next dotting position
int toNextX(int curX)
{
<<<<<<< HEAD
	checkStop();
	int evalValue = 0;	  // Value read from text file; 0 or 1
	int evalX = curX + 1; // Current x-position being evaluated
	checkStop();
=======
	int evalValue = 0; // Value read from text file; 0 or 1
	int evalX = curX+1;  // Current x-position being evaluated
>>>>>>> parent of 905b4dc (Merge branch 'main' of https://github.com/jzhou-k/EV3-Drawing-Robot)

	while (evalX < SPOTS_PER_ROW)
	{
<<<<<<< HEAD
		if (getButtonPress(ENTER_BUTTON))
		{
			notExit = false;
			break;
		}

		// keep reading file until it finds the next dotting position
=======
>>>>>>> parent of 905b4dc (Merge branch 'main' of https://github.com/jzhou-k/EV3-Drawing-Robot)
		readIntPC(fin, evalValue);
		if (evalValue == 1)
		{
			return (evalX - curX);
	  	}
	  	else
	  	{
	  		evalX++;
		}
<<<<<<< HEAD
		else
		{
			checkStop();
			evalX++;
		}
		checkStop();
=======
>>>>>>> parent of 905b4dc (Merge branch 'main' of https://github.com/jzhou-k/EV3-Drawing-Robot)
	}

	return -1; // Signal no dot found in remaining row
}

// Dots pen on page
void dotPen()
{
	time1[T4] = 0;
	motor[motorC] = 40;

<<<<<<< HEAD
	// while touch sensor haven't touched the paper, keep moving pen down
	while (!SensorValue[S3] && notExit)
	{

		if (getButtonPress(ENTER_BUTTON))
		{
			notExit = false;
			break;
		}

		// error checking, if this loop is excuting for more than 5 secs, assume pen is stuck and display error message
=======
	while(!SensorValue[S3])
	{
>>>>>>> parent of 905b4dc (Merge branch 'main' of https://github.com/jzhou-k/EV3-Drawing-Robot)
		if (time1[T4] > 5000)
		{
			displayBigTextLine(0, "Pen dislodged. Exiting...");
			playSound(soundException);
			wait1Msec(5000);
<<<<<<< HEAD
			checkStop();
=======
			return; // Later add exit draw function
>>>>>>> parent of 905b4dc (Merge branch 'main' of https://github.com/jzhou-k/EV3-Drawing-Robot)
		}
	}

	motor[motorC] = 0;
<<<<<<< HEAD
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
=======

	motor[motorC] = -40;

	while(nMotorEncoder(motorC) > 0)
	{
>>>>>>> parent of 905b4dc (Merge branch 'main' of https://github.com/jzhou-k/EV3-Drawing-Robot)
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
<<<<<<< HEAD
	if (!correctPosition(INIT_MOVE) && notExit)
=======
	if(!correctPosition(INIT_MOVE))
>>>>>>> parent of 905b4dc (Merge branch 'main' of https://github.com/jzhou-k/EV3-Drawing-Robot)
	{
		return;
	}
<<<<<<< HEAD
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
=======

	int curRow = 0;

	int drawStartTime = nPgmTime;

	while (curRow < ROWS)
	{
>>>>>>> parent of 905b4dc (Merge branch 'main' of https://github.com/jzhou-k/EV3-Drawing-Robot)
		int curCol = 0;
		int timeElapsed = nPgmTime - drawStartTime;
<<<<<<< HEAD
		checkStop();
		if (checkStart())
=======
		if(checkStart())
>>>>>>> parent of 905b4dc (Merge branch 'main' of https://github.com/jzhou-k/EV3-Drawing-Robot)
		{
			dotPen();
		}
		while (curCol < SPOTS_PER_ROW)
		{
<<<<<<< HEAD
			if (getButtonPress(ENTER_BUTTON))
			{
				notExit = false;
				break;
			}
			checkStop();
=======
>>>>>>> parent of 905b4dc (Merge branch 'main' of https://github.com/jzhou-k/EV3-Drawing-Robot)
			int goTo = toNextX(curCol);
			curCol += goTo;
			if (goTo != -1)
			{
<<<<<<< HEAD
				checkStop();
				driveXAxis(SIXTEENTH_INCH * goTo);
				checkStop();
=======
				driveXAxis(SIXTEENTH_INCH*goTo);
>>>>>>> parent of 905b4dc (Merge branch 'main' of https://github.com/jzhou-k/EV3-Drawing-Robot)
				dotPen();
			}
			else
			{
				curCol = SPOTS_PER_ROW;
			}

			if (time1[T1] > 500)
			{
				wait1Msec(5);
				displayBigTextLine(0, "Time Elapsed: %0.2f s", timeElapsed);
				time1[T1] = 0;
			}
		}
		if (time1[T1] > 500)
		{
			wait1Msec(5);
			displayBigTextLine(0, "Time Elapsed: %0.2f s", timeElapsed);
			time1[T1] = 0;
		}
		// reset X Pos
<<<<<<< HEAD
		checkStop();
		returnToXHome();
		checkStop();
=======
    returnToXHome();

>>>>>>> parent of 905b4dc (Merge branch 'main' of https://github.com/jzhou-k/EV3-Drawing-Robot)
		// Move down a row
		paperScroller(SIXTEENTH_INCH);
		curRow++;
	}

	closeFilePC(fin);
}

//check if file exits 
bool checkFile(string fileName)
{
	bool foundFile = openReadPC(fin, fileName);

	closeFilePC(fin);

	return foundFile;
}



//
bool correctPosition(float Dist)
{
	motor[motorD] = -30;
	time1[T4] = 0;
<<<<<<< HEAD
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
=======
	while(SensorValue[S1] != (int)colorWhite)
	{
		if(time1[T4] > 1000*6)
		{
>>>>>>> parent of 905b4dc (Merge branch 'main' of https://github.com/jzhou-k/EV3-Drawing-Robot)
			motor[motorD] = 0;
			eraseDisplay();
<<<<<<< HEAD
			checkStop();
			displayString(5, "ERROR");
			checkStop();
=======
			displayString(5,"ERROR");
>>>>>>> parent of 905b4dc (Merge branch 'main' of https://github.com/jzhou-k/EV3-Drawing-Robot)
			wait1Msec(10000);
			return false;
		}
	}
	motor[motorD] = 0;
	wait1Msec(1000);
	paperScroller(Dist);
	return true;
}


//prompts the user to load the paper correctly, then waits for user to press right button to start 
//the drawing function 
void promptUser()
{
<<<<<<< HEAD
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
=======
>>>>>>> parent of 905b4dc (Merge branch 'main' of https://github.com/jzhou-k/EV3-Drawing-Robot)
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


<<<<<<< HEAD
//check if user presses the enter button, then exit the program 
void checkStop()
{
	if (getButtonPress(ENTER_BUTTON))
	{
		notExit = false;
	}
=======

//initialize all 
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
>>>>>>> parent of 905b4dc (Merge branch 'main' of https://github.com/jzhou-k/EV3-Drawing-Robot)
}
