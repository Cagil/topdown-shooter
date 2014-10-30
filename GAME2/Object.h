#ifndef _OBJECT_H
#define _OBJECT_H


#include <windows.h>
//#include <gl\gl.h>	
#include <gl/GLU.h>
#include <string>
#include "Point.h"

class Object{

protected:
	float x,y;
	int width,height;
	Point center;
	int radius;

	GLuint texture;
public:
	Object();
	Object(float,float,int,int, GLuint);


	//GETTER METHODS
	int getWidth();
	int getHeight();
	int getRadius();
	float getX();
	float getY();
	float getX2();
	float getY2();
	Point getCenter();

	virtual void setTexture(GLuint tex);
	
	virtual void draw();
};


#endif _OBJECT_H