#ifndef _UNIT_H
#define _UNIT_H



#include <windows.h>
#include <gl\gl.h>
#include "Object.h"
#include "BBox.h"



class Unit : public Object{
protected:
	float radius;
	float angle;
	float speed,multiplier;
	static double PI;
	bool alive;
	BBox box;

public:
	Unit();
	Unit(int width, int height, float x, float y, GLuint texture, float speed=2.0);
	virtual void move(double delta);
	virtual void update(double delta);
	virtual void turn(float x, float y);

	virtual void draw();

	float getAngle();
	float getSpeed();
	BBox getBox();
	float getRadius();
	virtual bool isAlive(); 

	virtual void changeSpeed(float ds);

	void setAngle(float a);
	void setMultiplier(float s);
};


#endif _UNIT_H