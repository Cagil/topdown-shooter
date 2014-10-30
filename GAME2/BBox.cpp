#include "BBox.h"

#include <windows.h>
#include <gl\gl.h>			// Header file for the OpenGL32 Library
#include <gl\glu.h>

BBox::BBox(){}

BBox::BBox(float x, float y, float w, float h){
	this->left = x;
	this->bottom = y;
	this->right = x + w;
	this->top = y + h;
	this->width = w;
	this->height = h;
}

float BBox::getLeft(){
	return this->left;
}

float BBox::getRight(){
	return this->right;
}

float BBox::getTop(){
	return this->top;
}

float BBox::getBottom(){
	return this->bottom;
}

float BBox::getWidth(){
	return this->width;
}

float BBox::getHeight(){
	return this->height;
}

void BBox::setX(float x){
	this->left = x;
	this->right = this->left + this->width;
}

void BBox::setY(float y){
	this->bottom = y;
	this->top = this->bottom + this->height;
}

void BBox::draw(){
	glLineWidth(1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(this->left,this->bottom);
	glVertex2f(this->left,this->top);
	glVertex2f(this->right,this->top);
	glVertex2f(this->right,this->bottom);
	glEnd();

}

