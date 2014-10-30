#include "Unit.h"


#include <windows.h>		// Header file for Windows
#include <gl\gl.h>			// Header file for the OpenGL32 Library
#include <gl\glu.h>			// Header file for the GLu32 Library
#include <math.h>

double Unit::PI = 3.1415926535897932384626433832795;


Unit::Unit(){
}

Unit::Unit(int width, int height, float x, float y, GLuint texture, float s) : Object(x,y,width,height,texture){
	this->speed = s;

	if(width > height)
		this->radius = width/2;
	else 
		this->radius = height/2;
}

void Unit::changeSpeed(float s){
	this->speed += s;
}

void Unit::setMultiplier(float ds){
	/*if(ds < 2.0f && ds > 0.0f)
		this->multiplier = ds;
	else*/
		//this->multiplier = 1.0f;
	this->multiplier = ds;
}

void Unit::setAngle(float angle){
	this->angle = angle;
}



void Unit::move(double delta){

		
}

bool Unit::isAlive(){
	return this->alive;
}

float Unit::getAngle(){
	return this->angle;
}

float Unit::getSpeed(){
	return this->speed;
}

float Unit::getRadius(){
	return this->radius;
}

BBox Unit::getBox(){
	return this->box;
}


void Unit::turn(float x, float y){

}

void Unit::update(double delta){
	this->move(delta);
	
}

void Unit::draw(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->texture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
		//glTranslatef(this->x,this->y,0);
		//glRotatef(angle + 90, 0,0,1);
		
		glBegin(GL_QUADS);
		glColor3f(0.5,0.5,1.0);
			glTexCoord2f(0.0, 0.0); glVertex2f(this->x,this->y);
			glTexCoord2f(0.0, 1.0); glVertex2f(this->x,this->getY2());
			glTexCoord2f(1.0, 1.0); glVertex2f(this->getX2(),this->getY2());
			glTexCoord2f(1.0, 0.0); glVertex2f(this->getX2(),this->y);
		glEnd();
	
		glDisable(GL_TEXTURE_2D);

		glDisable(GL_BLEND);
	glPopMatrix();
}
