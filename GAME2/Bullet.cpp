#include "Bullet.h"


#include <iostream>
using namespace std;

int Bullet::MAX_X_BOUND = 0;
int Bullet::MAX_Y_BOUND = 0;

Bullet::Bullet(){}

Bullet::Bullet(float x, float y,float angle, float damage){
	this->x = x;
	this->y = y;
	this->speed = 0.005f;
	this->damage = damage;
	this->fired = true;
	this->angle = angle;
	this->PI= 3.1415926535897932384626433832795;
	this->width = 2;
	this->height = 2;
	this->texture = 0;
	if(width > height)
		this->radius = width/2;
	else 
		this->radius = height/2;
}


float Bullet::getDamage(){
	return this->damage;
}

float Bullet::getSpeed(){
	return this->speed;
}

float Bullet::getX(){
	return this->x;
}

float Bullet::getY(){
	return this->y;
}

float Bullet::getAngle(){
	return this->angle;
}

int Bullet::getRadius(){
	return this->radius;
}

int Bullet::getWidth(){
	return this->width;
}

int Bullet::getHeight(){
	return this->height;
}

bool Bullet::isFired(){
	return this->fired;
}

void Bullet::setAngle(float o){
	this->angle = o;
}

void Bullet::setDamage(float dmg){
	this->damage = dmg;
}

void Bullet::setSpeed(float dys){
	this->speed = dys;
}

void Bullet::setX(float x){
	this->x  = x;
}

void Bullet::setY(float y){
	this->y = y;
}

void Bullet::setTexture(GLuint tex){
	this->texture = tex;
}

void Bullet::kill(){
	this->fired = false;
}

void Bullet::draw(){
	if(fired == true){

	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->texture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
		
		glTranslatef(x,y,0);
        glRotatef(angle + 90, 0,0,1);
        glTranslatef(-(x + this->getWidth()/2),-(y+this->getHeight()/2),0);
			
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex2f(this->x,this->y);
			glTexCoord2f(0.0, 1.0); glVertex2f(this->x,this->y +  this->getHeight());
			glTexCoord2f(1.0, 1.0); glVertex2f(this->x + this->getWidth(),this->y + this->getHeight());
			glTexCoord2f(1.0, 0.0); glVertex2f(this->x + this->getWidth(),this->y);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);	
	glPopMatrix();

	}
}

void Bullet::update(double delta){
		this->x += cos( this->angle*( PI/180 ) )*(speed*delta);
		this->y += sin( this->angle*( PI/180 ) )*(speed*delta);

		//BOUNDARIES OF THE MAP if collides with boundaries of the map gets set to false.
		if(x >= MAX_X_BOUND || x <= 0)
			fired = false;
		if(y >= MAX_Y_BOUND || y <= 0)
			fired = false;
	
}