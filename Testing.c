/*
    ASSUME: TOUCH SENSOR IS S2
            MOTOR A IS THE X AXIS

    This function will move the x axis motor to the begining (far left)
    of the printer, where the pen is at the first pixel position.
    The motor will reverse until it hits the touch sensor.
*/
void returnToXHome()
{
	  while(SensorValue[S2] != 1)
	  {
	      motor[motorA] = -50;
	  }
	  motor[motorA] = 0;
	  nMotorEncoder[motorA] = 0;
}

task main()
{
    SensorType[S2] = sensorEV3_Touch;
    returnToXHome();
}
