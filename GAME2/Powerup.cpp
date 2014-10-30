#include "Powerup.h"



Powerup::Powerup(){}

Powerup::Powerup(float x, float y, int width, int height):Object(x,y,width,height,0){

	this->interval = 300000;
	this->value = 25;
	this->charged = false;
	time = 0;
	if(type == HP)
		this->value = 25;
	else if(type == AMMO)
		this->value = 15;

	this->center = Point(x + width/2, y+height/2);
}

Powerup::Powerup(float x, float y, int width, int height,GLuint texture, TYPE t):Object(x, y, width, height,texture){
	this->interval = 300000;
	
	this->charged = false;
	time = 0;
	this->type = t;

	if(type == HP)
		this->value = 25;
	else if(type == AMMO)
		this->value = 15;

	this->center = Point(x + width/2, y+height/2);
}

void Powerup::setType(TYPE type){
	this->type = type;
}

void Powerup::setInterval(double d){
	this->interval = d;
}

double Powerup::getInterval(){
	return this->interval;
}

bool Powerup::isCharged(){
	return this->charged;
}

void Powerup::draw(){
	if(isCharged()){
		glLoadIdentity();
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
}

void Powerup::update(double delta, Player* player, std::vector<Powerup>* pups){
	this->recharge(delta);

	if(this->collides(player)){
		this->interval += 100000;
		pups->clear();
	}
}

void Powerup::recharge(double delta){
	if(!isCharged()){
		time += delta;

		if(time >= this->interval){
			time = 0;
			this->charged = true;
		}
	}
	
}

bool Powerup::collides(Player* player){
	if(isCharged()){
		float difx2 = (player->getX() - this->center.getX()) * (player->getX() - this->center.getX());
		float dify2 = (player->getY() - this->center.getY()) * (player->getY() - this->center.getY());
		float rr2 = (player->getRadius() + this->getRadius()) * (player->getRadius() + this->getRadius());

		float dist = difx2 + dify2;

		if(dist <= rr2){
			bonus_to_player(player);
			this->charged = false;
			return true;
		}
	}

	return false;
}

void Powerup::bonus_to_player(Player* p){

	if(this->type == AMMO){
		p->addAmmo(this->value);
	}
	else if(this->type == HP){
		p->changeHealth(this->value);
	}
}