#include "screenPt.h"

screenPt::screenPt()
{
	x = 0;
	y = 0;
}

screenPt::~screenPt() {}

void screenPt::setCoords(int xValue, int yValue)
{
	x = xValue;
	y = yValue;
}

int screenPt::getX() { return x; }
int screenPt::getY() { return y; }

void screenPt::incrementX()
{
	x++;
}

void screenPt::decrementY()
{
	y--;
}