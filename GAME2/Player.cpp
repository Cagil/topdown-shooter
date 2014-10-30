#include "Player.h"

#include <windows.h>		// Header file for Windows
#include <gl\gl.h>			// Header file for the OpenGL32 Library
#include <gl\glu.h>			// Header file for the GLu32 Library
#include <math.h>

#include <iostream>
using namespace std;

bool Player::drawLines = true;

Player::Player(){}

Player::Player(int width, int height, float x, float y, GLuint texture, float speed):Unit(width, height,x,y,texture, speed){
	this->exp = 0;
	this->maxExp = 100;
	this->rank = 1;
	this->damage = 7;
	this->MAX_HEALTH = 100;
	this->health = MAX_HEALTH;
	this->totalAmmo = 75;
	this->currentClipSize = 10;
	this->speed = speed;
	this->MAX_SPEED = 0.0005f;
	this->acceleration = 0.00025f;
	this->multiplier = 1.0f;
	this->box = BBox(x - width/2,y - height/2,width,height);
	this->collides = false;
	this->shoot = true;
	this->firingInterval = 0;
	this->kill_count = 0;
	this->center = Point(x, y);
	this->bullet_texture = 0;
		this->moveToTarget = false;

}

Player::Player(int width, int height, float x, float y):Unit(width, height,x,y,0, 0){
	this->exp = 0;
	this->maxExp = 100;
	this->rank = 1;
	this->damage = 7;
	this->MAX_HEALTH = 100;
	this->health = MAX_HEALTH;
	this->totalAmmo = 75;
	this->currentClipSize = 10;
	this->speed = 0.000125f;
	this->MAX_SPEED = 0.0005f;
	this->acceleration = 0.00025f;
	this->multiplier = 1.0f;
	this->box = BBox(x - width/2,y - height/2,width,height);
	this->collides = false;
	this->shoot = true;
	this->firingInterval = 0;
	this->kill_count = 0;
	this->center = Point(x, y);
	this->bullet_texture = 0;
	this->moveToTarget = false;
}

void Player::drawPlayer(){
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->texture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
		glTranslatef(x,y,0);
		glRotatef(angle - 90, 0,0,1);
		glTranslatef(-(x + this->getWidth()/2),-(y+this->getHeight()/2),0);
		
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

void Player::draw(){
	//draws bullets
	glPushMatrix();
		this->drawBullets();
	glPopMatrix();

	//draws players
	glPushMatrix();
		this->drawPlayer();
	glPopMatrix();

	//draws bounding box,
	//debug purposes ONLY
	if(drawLines == true){
		glPushMatrix();
			this->box.draw();
		glPopMatrix();
	}
}


bool Player::checkCollision(std::vector<BBox> boxes, double delta, char direction){
	float temp = this->multiplier*(this->speed*delta);

	for(int i = 0;i<boxes.size();i++){

		switch(direction){
		case 'U':
			if(this->getBox().getBottom() + temp < boxes.at(i).getTop() &&
				this->getBox().getTop()  + temp > boxes.at(i).getBottom() &&
				this->getBox().getRight() > boxes.at(i).getLeft() &&
				this->getBox().getLeft() < boxes.at(i).getRight()){
		
				this->collides = true;			
			}else{
				this->collides = false;
			}
			this->move(temp,direction);
			break;
		case 'D':
			if(this->getBox().getBottom() - temp < boxes.at(i).getTop() &&
				this->getBox().getTop() - temp > boxes.at(i).getBottom() &&
				this->getBox().getRight() > boxes.at(i).getLeft() &&
				this->getBox().getLeft() < boxes.at(i).getRight()){
		
		this->collides = true;
	
			}else{
				this->collides = false;
			
			}
			this->move(temp,direction);
			break;
		case 'L':
			if(this->getBox().getBottom() < boxes.at(i).getTop() &&
				this->getBox().getTop() > boxes.at(i).getBottom() &&
				this->getBox().getRight() - temp > boxes.at(i).getLeft() &&
				this->getBox().getLeft()  - temp < boxes.at(i).getRight()){
		
				this->collides = true;
	
			}else{
				this->collides = false;
			
			}
			this->move(temp,direction);
			break;
		case 'R':
			if(this->getBox().getBottom() < boxes.at(i).getTop() &&
				this->getBox().getTop()  > boxes.at(i).getBottom() &&
				this->getBox().getRight() + temp > boxes.at(i).getLeft() &&
				this->getBox().getLeft()  + temp< boxes.at(i).getRight()){
		
				this->collides = true;
		
			}else{
				this->collides = false;

			}
			this->move(temp,direction);
			break;
		}					
	}
				
	return this->collides;
}
//checks if players positions are below or under the boundary lines
bool Player::isOutOfTrack(vector<BoundaryLine>* lines){

	for(int i = 0;i<lines->size();i++){
		BoundaryLine bl = lines->at(i);
		if(this->x >= bl.getBox().getLeft() && this->x < bl.getBox().getRight() && this->y >= bl.getBox().getBottom() && this->y < bl.getBox().getTop()){
			float result = ((bl.getB().getX() - bl.getA().getX()) * (this->y - bl.getA().getY())) - ((bl.getB().getY() - bl.getA().getY()) * (this->x - bl.getA().getX()));

			//if the result is positive or negative like the boundaryLine's track side value
			//trackside values are pre defined (+1 or -1) if result being under is negative and trackside is -1 
			//then it means player is on the track.
			if(result > 0 && bl.getTrackside() > 0 || result < 0 && bl.getTrackside() < 0){
				//sets speed multiplier to 0.65 
				this->multiplier = 0.650;
				return false;
			}
			else {
				//sets speed multiplier to 0.50
				this->multiplier = 0.50;
				return true;
			}		
		}else
			continue;	
	}
	return false;
}



//moves the player towards the direction given if not moves back in the opposite direction 
//U - UP D - DOWN L - LEFT - R - RIGHT
void Player::move(float move_dy, char direction){
	
	switch(direction){
			case 'U':
				if(!this->collides){
					this->y += move_dy;
					this->box.setY(this->y - this->height/2);
				}
				else{
					this->y -= 0.65;
					this->box.setY(this->y - this->height/2);
				}
				break;
			case 'D':
				if(!this->collides){
					this->y -= move_dy;
					this->box.setY(this->y - this->height/2);
				}else{
					this->y += 0.65;
					this->box.setY(this->y - this->height/2);
				}
				break;
			case 'L':
				if(!this->collides){
					this->x -= move_dy;
					this->box.setX(this->x - this->width/2);
				}
				else{
					this->x +=0.65;
					this->box.setX(this->x - this->width/2);
				}
				break;
			case 'R':
				if(!this->collides){
					this->x += move_dy;
					this->box.setX(this->x - this->width/2);
				}
				else{
					this->x -= 0.65;
					this->box.setX(this->x - this->width/2);
				}
				break;
		}
	
}

void Player::moveToPoint(double delta){
	if(moveToTarget){
		float xint = this->getX() - this->targetPoint.getX();
		float yint = this->getY() - this->targetPoint.getY();

		float movespeed = this->multiplier*(delta * this->speed);

		if(xint < 0){
			this->x += movespeed;
		}
		else if(xint > 0){
			this->x -= movespeed;
		}

		if(yint < 0){
			this->y += movespeed;
		}else if(yint > 0){
			this->y -= movespeed;
		}


	}
}

void Player::setPointToMove(float mouse_x, float mouse_y){
	targetPoint.setX(mouse_x);
	targetPoint.setY(mouse_y);

	if(this->targetPoint.getX() != this->center.getX() || this->targetPoint.getY() != this->center.getY()){
		
		this->moveToTarget  = true;

	}else
		this->moveToTarget = false;


}

//calculates and gets the angle between the mouse coordinates and the middle of the 
//middle of the map also the player's positions 
void Player::turn(float mouse_x, float mouse_y){		
		float dify = mouse_y - 240;
		float difx = mouse_x - 240;
		
		this->setAngle(atan2f(dify,difx)*(180.0f/PI));
}

void Player::update(double delta, float x, float y){
	this->updateFiring(delta);
	this->updateBullets(delta, x, y);
	
	if(this->targetPoint.getX() == this->center.getX() && this->targetPoint.getY() == this->center.getY()){
		this->moveToTarget = false;
	}


	this->turn(x, y);

	
	this->moveToPoint(delta);
}

void Player::changeSpeed(float ds){
	if(ds > 0){
		if(ds < this->MAX_SPEED)
			this->speed  = ds;
		else
			this->speed = this->MAX_SPEED;
	}
}

float Player::getMaxSpeed(){
	return this->MAX_SPEED;
}

bool Player::isAlive(){
	return true;

}

float Player::getAngle(){
	return this->angle;
	
}

void Player::updateFiring(double delta){
	if(!this->shoot){
		this->firingInterval += delta;
		
	}

	if(this->firingInterval > 10000){
		this->shoot = true;
		this->firingInterval = 0;
	}

}

//BULLET RELATED METHODS
void Player::updateBullets(double delta,float mouse_x,float mouse_y){
	for(int i = 0;i<ammoclip.size();i++){
		if(ammoclip.at(i).isFired() == true){
			ammoclip.at(i).update(delta);		
		}			
	}
}

void Player::reload(){
	if(this->totalAmmo > 0 ){
		if(this->currentClipSize == 0){
			ammoclip.clear();
			if(this->totalAmmo > 10){
				currentClipSize = 10;
				this->totalAmmo -= 10;
			}else{	
				this->currentClipSize = this->totalAmmo;
				this->totalAmmo = 0;
			}
			this->shoot = true;			
		}
		
	}
}

void Player::drawBullets(){
	for(int i = 0;i<ammoclip.size();i++){
		ammoclip.at(i).draw();
	}
}

void Player::fire(float mouse_x, float mouse_y){
	if(this->shoot){
		if(this->currentClipSize > 0){
			Bullet b = Bullet(this->x, this->y,this->angle,this->getDamage());
			b.setTexture(this->bullet_texture);
			this->ammoclip.push_back(b);
			this->currentClipSize--;
		}
		this->shoot = false;
	}
	
}

void Player::addAmmo(int amount){
	this->totalAmmo += amount;
}

int Player::getCurrentClipSize(){
	return this->currentClipSize;
}

int Player::getTotalAmmo(){
	return this->totalAmmo;
}

//BULLET END

int Player::getDamage(){
	return this->damage;
}

void Player::changeDamage(int dyd){
	this->damage += dyd;
}

void Player::gainExp(int amount){
	this->exp += amount;

	if(this->exp >= this->maxExp){
		this->rankUp();
	}
}

void Player::rankUp(){
	this->rank++;
	
	this->changeDamage(+2);
	this->exp = this->exp - this->maxExp;
	if(this->rank < 5){
		this->maxExp = this->rank * 100;
	}
	else if(this->rank > 5){
		this->maxExp = this->rank* 300;
	}
	else{
		this->maxExp = this->rank* 500;
	}
}

int Player::getExp(){
	return this->exp;
}

int Player::getMaxExp(){
	return this->maxExp;
}

int Player::getRank(){
	return this->rank;
}

int Player::getKillCount(){
	return this->kill_count;
}

//HEALTH METHODS
int Player::getHealth(){
	return this->health;
}

int Player::getMaxHealth(){
	return this->MAX_HEALTH;
}

void Player::changeHealth(int x){
	if((this->health + x) <= this->MAX_HEALTH)
		this->health += x;
	else
		this->health = this->MAX_HEALTH;

	if(this->health <= 0)
		this->alive = false;
}

void Player::setMaxHealth(int hp){
	this->MAX_HEALTH = hp;
	int difhp = this->getHealth() - this->MAX_HEALTH;

	this->changeHealth(difhp);
}

//HEALTH END

void Player::setBulletTexture(GLuint bullet_tex){
	this->bullet_texture = bullet_tex;
}

bool Player::checkBulletCollision(std::vector<Zombie>* zombies, std::vector<BBox>* boxes){
	
	//checks if bullet collides with any zombies if so damages them n if they are dead
	//players gets reward and kills the zombies.
	//or and in any case destoryes the bullet colliding.
	for(int i = 0;i<zombies->size();i++){
		if(!zombies->at(i).isAlive())
			continue;
		for(int k = 0;k<this->ammoclip.size();k++){
			if(this->ammoclip.at(k).isFired()){
				float difx = this->ammoclip.at(k).getX() - zombies->at(i).getX();
				float dify = this->ammoclip.at(k).getY() - zombies->at(i).getY();

				float r = ammoclip.at(k).getRadius() + zombies->at(i).getRadius();
				r = r*r;
				float dist = difx*difx + dify*dify;

				if(dist <= r){				
					zombies->at(i).changeHealth(-1*(this->ammoclip.at(k).getDamage()));
					this->ammoclip.at(k).kill();
					if(zombies->at(i).getHealth() <= 0){
						zombies->at(i).kill();
						this->gainExp(zombies->at(i).getExpPoints());
						this->kill_count++;
					}
					return true;
				}else
					continue;
			}else
				continue;
		}
	}

	//checks if the bullet collides with any non moving obstacles
	for(int i = 0;i<boxes->size();i++){
		for(int k = 0;k<this->ammoclip.size();k++){
			if(this->ammoclip.at(k).isFired()){
				if(boxes->at(i).getLeft() <= this->ammoclip.at(k).getX() &&
					boxes->at(i).getRight() >= this->ammoclip.at(k).getX() &&
					boxes->at(i).getBottom() <= this->ammoclip.at(k).getY() &&
					boxes->at(i).getTop() >= this->ammoclip.at(k).getY()){
						this->ammoclip.at(k).kill();
						return true;
				}else
					continue;
			}else
				continue;
		}
	}
	return false;
}




