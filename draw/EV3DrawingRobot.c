// not so empty robotc file
const double DPI = (1.0/16.0);
const double INCH_TO_CM = 2.54;

int distYToPoint(int currY, int nextY);
double convertUnitToCM(int unit);
double convertInchToCM(double inches);
void ink(int inkSpeed);
void configureAllSensors();


task main()
{
	configureAllSensors();
	int inkSpeed = 15;
	ink(inkSpeed);

}

//enters unit num
int distYToPoint(int currY, int nextY)
{
		return nextY - currY;
}

double convertUnitToCM(int unit)
{
	return convertInchToCM(unit * DPI);
}

double convertInchToCM(double inches)
{
	return inches * INCH_TO_CM;
}

//moves pen down and as pen touches paper, moves back up original distance
void ink(int inkSpeed)
{
	//depends on motor
	int travelDistance = nMotorEncoder[motorA];
	motor[motorA] = inkSpeed;
	while(SensorValue[S1] != 1)
	{}
	motor[motorA] = -inkSpeed;
	while(nMotorEncoder[motorA] > travelDistance)
	{}
}

void configureAllSensors()
{
	SensorType[S1] = sensorEV3_Touch;
}
