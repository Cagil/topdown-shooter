#include "HUDbar.h"

#include <windows.h>		// Header file for Windows
#include <gl\gl.h>			// Header file for the OpenGL32 Library
#include <gl\glu.h>			// Header file for the GLu32 Library
#include <math.h>




HUDbar::HUDbar(){}

HUDbar::HUDbar(float x, float y, int width, int height,TYPE type){
	this->x  = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->value = 0;
	this->maxvalue = 0;
	this->type = type;
}

float HUDbar::getX(){
	return this->x;
}

float HUDbar::getY(){
	return this->y;
}

int HUDbar::getWidth(){
	return this->width;
}

int HUDbar::getHeight(){
	return this->height;
}

int HUDbar::getValue(){
	return this->value;
}

int HUDbar::getMaxValue(){
	return this->maxvalue;
}

void HUDbar::setMaxValue(int x){
	this->maxvalue = x;
}

void HUDbar::setX(float x){
	this->x = x;
}

void HUDbar::setY(float y){
	this->y = y;
}

void HUDbar::setWidth(int w){
	this->width = w;
}

void HUDbar::setHeight(int h){
	this->height = h;
}

void HUDbar::setValue(int v){
	this->value = v;
}

void HUDbar::draw(){
	glLoadIdentity();	
	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3f(0.7f,0.7f,0.7f);
		glVertex2f(this->x,this->y-1);
		glVertex2f(this->x,this->y+this->height+1);
		glVertex2f(this->x + this->width, this->y + this->height);
		glVertex2f(this->x + this->width, this->y);
	glEnd();
	glPopMatrix();

	//bar value drawn
	glPushMatrix();	
		glBegin(GL_QUADS);
		glColor3ub(red,green,blue);
			glVertex2f(this->x,this->y);
			glVertex2f(this->x,this->y+this->height);
			glVertex2f(this->x + this->value, this->y + this->height);
			glVertex2f(this->x + this->value, this->y);	
			glColor3f(1,1,1);
		glEnd();	
	glPopMatrix();

	glLineWidth(1.0f);
	glPushMatrix();
	glBegin(GL_LINE_LOOP);
		glVertex2f(this->x,this->y-1);
		glVertex2f(this->x,this->y+this->height+1);
		glVertex2f(this->x + this->width, this->y + this->height);
		glVertex2f(this->x + this->width, this->y);
	glEnd();
	glPopMatrix();
}


void HUDbar::update(Player* p){

	if(this->type == HEALTH){	
		this->value = p->getHealth();
		this->maxvalue = p->getMaxHealth();
		//calculates the width bar so it represents the health accordingly
		this->value = (this->value * this->width)/this->maxvalue;
		//changing the color as the health of the player gets low
		//full hp -> 100% green
		//0 health -> 100% red
		green = (this->value * 255)/this->width;
		red = 255- green;
		blue = 0;
	}
	else if(this->type == EXPERIENCE){		
		this->maxvalue = p->getMaxExp();
		this->value = (p->getExp() * this->width)/this->maxvalue ;
		green = 0;
		red = 0;
		blue = 255;
	}
}

void HUDbar::update(Zombie* z){

	if(this->type == HEALTH){	
		this->value = z->getHealth();
		this->maxvalue = z->getMaxHealth();
		this->value = (this->value * this->width)/this->maxvalue;
	
		green = 255;
		blue = 0;
		red = 0;
	}
	
}


