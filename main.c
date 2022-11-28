#include "menu/mainMenu.c"
#include "lib/ports.h"

void configureAllSensors();

task main()
{
	configureAllSensors();
	mainMenuRun();
}

void configureAllSensors()
{
	SensorType[zAxisLim] = sensorEV3_Touch;
	wait1Msec(50);
	SensorType[xAxisLim] = sensorEV3_Touch;
	wait1Msec(50);
	SensorType[paperSensor] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[paperSensor] = modeEV3Color_Color;
	wait1Msec(50);
}
