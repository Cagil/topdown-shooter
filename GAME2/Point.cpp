#include "Point.h"

#include <windows.h>
#include <gl\gl.h>			// Header file for the OpenGL32 Library
#include <gl\glu.h>	

Point::Point(){}

Point::Point(float x , float y){
	this->x = x;
	this->y = y;
}

float Point::getX(){
	return this->x;
}

float Point::getY(){
	return this->y;
}

void Point::setX(float x){
	this->x = x;
}

void Point::setY(float y){
	this->y = y;
}

void Point::draw(){
	
}