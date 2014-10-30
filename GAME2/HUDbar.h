#ifndef _HUDBAR_H
#define _HUDBAR_H

#include "Player.h"
#include "Zombie.h"

class HUDbar{

	
public:
	enum TYPE {HEALTH,EXPERIENCE};
	HUDbar();
	HUDbar(float,float,int,int,TYPE);

	float getX();
	float getY();
	int getWidth();
	int getHeight();
	int getValue();
	int getMaxValue();

	void setX(float);
	void setY(float);
	void setWidth(int);
	void setHeight(int);
	void setValue(int);
	void setMaxValue(int);

	void draw();
	void update(Player*);
	void update(Zombie*);

private:
	
	float x,y;
	int width,height;
	int value,maxvalue;

	

	int red,green,blue;
	TYPE type;
};

#endif _HUDBAR_H
