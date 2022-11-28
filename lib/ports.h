#pragma once
typedef enum ports
{
	xAxis = motorA, // moves the dottor left and right
	yAxis = motorB, // moves the paper
	zAxis = motorC, // moves the dottor up and down
	xAxisLim = S2, // touch sensor on x axis
	zAxisLim = S3, // touch sensor on z axis (dotter)
	paperSensor = S1 // color sensor for the paper on y axis
};