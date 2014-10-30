#include "Object.h"

#include <windows.h>		// Header file for Windows
#include <gl\gl.h>			// Header file for the OpenGL32 Library
#include <gl\glu.h>			// Header file for the GLu32 Library
#include <math.h>



Object::Object(){
	x = 10;
	y = 10;
	width = 50;
	height = 50;

}

Object::Object(float x,float y, int width, int height, GLuint texid){
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->setTexture(texid);
	this->center = Point(x + width/2, y + height/2);

	if(width > height)
		this->radius = width / 2;
	else
		this->radius = height / 2;
}



float Object::getX2(){
	return this->x + this->getWidth();
}

float Object::getY2(){
	return this->y + this->getHeight();
}

void Object::draw(){

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->texture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
		
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex2f(this->x,this->y);
			glTexCoord2f(0.0, 1.0); glVertex2f(this->x,this->getY2());
			glTexCoord2f(1.0, 1.0); glVertex2f(this->getX2(),this->getY2());
			glTexCoord2f(1.0, 0.0); glVertex2f(this->getX2(),this->y);
		glEnd();

		glDisable(GL_TEXTURE_2D);

		glDisable(GL_BLEND);
		
	glPopMatrix();
	
	
}



void Object::setTexture(GLuint tex){
	this->texture = tex;
}

int Object::getWidth(){
	return this->width;
}

int Object::getHeight(){
	return this->height;
}

int Object::getRadius(){
	return this->radius;	
}

float Object::getX(){
	return this->x;
}

float Object::getY(){
	return this->y;
}

Point Object::getCenter(){
	return this->center;
}

