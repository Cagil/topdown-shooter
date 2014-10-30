#ifndef _BULLET_H
#define _BULLET_H

#include <windows.h>		// Header file for Windows
#include <gl\gl.h>			// Header file for the OpenGL32 Library
#include <gl\glu.h>

class Bullet{
private:
	float x,y;
	float speed;
	float damage;
	float angle;
	bool fired;
	double PI;
	GLuint texture;
	int width, height,radius;
public:
	//CONSTRUCTORS
	Bullet();
	Bullet(float x, float y,float a,float d);

	//OTHER METHODS
	void draw();
	void update(double delta);
	void kill();
	//SETTERS
	void setSpeed(float x);
	void setDamage(float x);
	void setX(float x);
	void setY(float y);
	void setAngle(float o);
	void setTexture(GLuint);
	
	//GETTERS
	float getX();
	float getY();
	float getSpeed();
	float getDamage();
	float getAngle();
	int getRadius();
	int getWidth();
	int getHeight();
	bool isFired();

	
	//STATIC
	static int MAX_X_BOUND;
	static int MAX_Y_BOUND;

};


#endif _BULLET_H