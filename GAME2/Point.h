#ifndef _POINT_H
#define _POINT_H


class Point{
private:
	float x, y;

public:
	Point();
	Point(float, float);

	float getX();
	float getY();
	void setX(float);
	void setY(float);

	void draw();
};

#endif _POINT_H