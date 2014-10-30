#ifndef _POWERUP_H
#define _POWERUP_H


#include "Object.h"
#include "Player.h"
#include <vector>

class Powerup : public Object
{
public:
	enum TYPE { HP, AMMO };

	Powerup();
	Powerup(float x, float y, int widht, int height);
	Powerup(float x, float y, int width, int height, GLuint texture, TYPE type);

	void setType(TYPE);
	void setInterval(double d);

	bool isCharged();
	double getInterval();
	
	void draw();
	void update(double delta, Player*,std::vector<Powerup>*);
	void recharge(double delta);
	bool collides(Player* player);
	

private:
	int value;
	TYPE type;
	bool charged;
	double interval;
	double time;

	void bonus_to_player(Player* p);	
};

#endif _POWERUP_H