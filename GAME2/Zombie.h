#ifndef _ZOMBIE_H
#define _ZOMBIE_H


class Player;
class HUDbar;



#include "Unit.h"
#include "Player.h"
#include "HUDbar.h"
#include "BBox.h"
#include <vector>
#include "BoundaryLine.h"


class Zombie : public Unit{
private:
	int damage;
	float speed;
	int points;
	int health;
	int MAX_HEALTH;	
	
	float attackTimer;
	bool readyToAttack;
	HUDbar* bar;
	void drawZombie();
	void drawBox();		



public:
	Zombie();
	Zombie(int width,int height, float x, float y, GLuint texture);
	Zombie(int width, int height, float x, float y);

	void kill();
	void changeHealth(int amount);
	void move(double delta);
	void turn(float x, float y);
	void update(double delta,float x, float y,std::vector<BBox>*);	
	void draw();
	void drawHealthbar();
	
	int getMaxHealth();
	int getHealth();
	bool isAlive();
	bool canAttack();
	float getAttackTimer();
	int getExpPoints();
	
	
	void setAttributes(int rank);
	void chargeAttack(double delta);
	void attack(Player* p);

	//checks collision with player
	//circle to circle collison
	bool checkPlayerCollision(Player* p);
	//checks collison with rectangles
	//circle to rectangle (cirle centered on a point on the rectangle's box)
	bool checkCollision(std::vector<BBox>*,float ,float);
	bool isOutOfTrack(std::vector<BoundaryLine>*);
	//if SET TRUE draws bounding boxes of zombies
	static bool drawLines;
};


#endif _ZOMBIE_H