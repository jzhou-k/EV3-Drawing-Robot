// not so empty robotc file
const double DPI = (1.0/16.0);
const double INCH_TO_CM = 2.54;

int distYToPoint(int currY, int nextY);
double convertUnitToCM(int unit);
double convertInchToCM(double inches);
void ink();

task main()
{



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

void ink()
{
}
