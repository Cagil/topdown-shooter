#ifndef _PLAYER_H
#define _PLAYER_H

class Zombie;
#include "Unit.h"
#include "BoundaryLine.h"
#include "Bullet.h"
#include "BBox.h"
#include "Zombie.h"
#include "Point.h"
#include <vector>

class Player : public Unit
{
private:
	int rank;
	int exp;
	int maxExp;
	int health;
	int MAX_HEALTH;
	int damage;
	float MAX_SPEED;
	float acceleration;
	std::vector<Bullet> ammoclip;
	int totalAmmo;
	int currentClipSize;
	int kill_count;
	bool collides;
	bool shoot;
	double firingInterval;

	Point targetPoint;
	bool moveToTarget;

	GLuint bullet_texture;
public:
	Player();
	Player(int width, int height, float x, float y, GLuint texture, float speed=0);
	Player(int width, int height, float x, float y);

	//USER CONTROLLED METHODS
	//moves the player
	void move(float magnitude, char direction);
	void moveToPoint(double d);
	void setPointToMove(float, float);
	//creates a bullet adds to the bullet list
	void fire(float mouse_x,float mouse_y);
	//clears the list, reloads in a way
	void reload();

	//UPDATE METHODS
	//updates player position & angle 
	void update(double delta, float x , float y);
	//calculates the angle
	void turn(float x, float y);
	//calculates bulelts position & set bulelts angle to players angle
	void updateBullets(double delta,float mouse_x,float mouse_y);
	//calculates time between each firing, how fast the player shoots
	void updateFiring(double delta);

	//DISPLAY METHODS
	void draw();
	void drawPlayer();
	void drawBullets();

	//GETTERS
	float getMaxSpeed();
	float getAngle();
	int getHealth();
	int getMaxHealth();
	bool isAlive();
	int getExp();
	int getRank();
	int getMaxExp();
	int getDamage();
	int getTotalAmmo();
	int getCurrentClipSize();
	int getKillCount();

	//SETTERS/MODIFIERS
	void setBulletTexture(GLuint);
	void setMaxHealth(int hp);
	void setMaxSpeed(float ms);
	void changeHealth(int dyhealth);
	void changeSpeed(float ds);
	void gainExp(int exp);
	void addAmmo(int amount);
	void rankUp();
	void changeDamage(int dyd);


	//Collision METHODS
	//checks collisions with non mobile obstacles
	bool checkCollision(std::vector<BBox>, double, char);
	//checks collisions with track edges
	bool isOutOfTrack(std::vector<BoundaryLine>* );
	//checks bullet's collision with zombies
	bool checkBulletCollision(std::vector<Zombie>*,std::vector<BBox>*);

	//if set true draws bounding box
	static bool drawLines;
};
#endif _PLAYER_H