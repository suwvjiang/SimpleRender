#pragma once
class screenPt
{
private:
	int x, y;

public:
	screenPt();
	~screenPt();
	void setCoords(int xValue, int yValue);
	int getX();
	int getY();
	void incrementX();
	void decrementY();
};

