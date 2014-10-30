#include "Zombie.h"

bool Zombie::drawLines = true;

Zombie::Zombie(){}

Zombie::Zombie(int width, int height, float x, float y):Unit(width, height,x,y,0, 0){
	this->damage = 10;
	this->speed = 0.00025f;
	this->health = 15;
	this->MAX_HEALTH = 15;
	this->readyToAttack = true;
	this->attackTimer = 0;
	this->multiplier = 1.0;
	int temp_box_dim;
	if(width > height){
		temp_box_dim = width;
		this->radius = width * 0.6;
	}
	else
	{
		temp_box_dim = height;
		this->radius = height * 0.6;
	}

	this->box = BBox(x - temp_box_dim/2,y - temp_box_dim/2,temp_box_dim,temp_box_dim);
	bar = new HUDbar(this->x , this->y +3, this->health, 5,HUDbar::HEALTH);
	this->center = Point(x, y);		
}

Zombie::Zombie(int width, int height, float x, float y, GLuint texture):Unit(width, height,x,y,texture, 0){
	this->damage = 10;
	this->speed = 0.00025f;
	this->health = 15;
	this->MAX_HEALTH = 15;
	this->readyToAttack = true;
	this->attackTimer = 0;
	this->multiplier = 1.0;
	int temp_box_dim;
	if(width > height){
		temp_box_dim = width;
	}
	else{
		temp_box_dim = height;
		this->radius = height * 0.6;
	}

	this->box = BBox(x - temp_box_dim/2,y - temp_box_dim/2,temp_box_dim,temp_box_dim);
	bar = new HUDbar(this->x , this->y +3, this->health, 5,HUDbar::HEALTH);
	this->center = Point(x, y);


}

void Zombie::setAttributes(int rank){
	this->damage = 10 + (3 * rank);
	this->speed = 0.00025f + (0.00009500 * rank);
	this->health = 20 + (15 * rank);
	this->MAX_HEALTH = this->health;
	this->points = 35 + (5 * rank);
}




void Zombie::move(double delta){
	this->x += cos( this->getAngle()*( PI/180 ) )*(speed*delta)*this->multiplier;
	this->box.setX(this->x - this->width/2);
	this->y += sin( this->getAngle()*( PI/180 ) )*(speed*delta)*this->multiplier;
	this->box.setY(this->y - this->height/2);
}

//calcualtes the angle of the zombie according to the player's position
//if the player is in a different quadrant of the zombie
// different way of getting difference of position used.
void Zombie::turn(float player_x, float player_y){
	float temp_angle = 0;
	float difx = 0.0;
	float dify = 0.0;
	if(player_x < this->x){
		if(player_y < this->y){
			//3ND QUADRANT
			dify = this->y - player_y;
			difx = this->x - player_x;

			
			temp_angle = atan2(dify,difx)*(180.0f/PI);
			temp_angle = temp_angle - 180;
		}
		else if(player_y > this->y){
			//2ND QUADRANT
			dify = player_y - this->y;
			difx = this->x - player_x;

			temp_angle = atan2(dify, difx)*(180.0f/PI);
			temp_angle = -1*(temp_angle) - 180;
		}
		else{
			temp_angle = 180;//-180
		}
	}else if(player_x > this->x){
		if(player_y < this->y){
			//3RD QUADRANT
				dify = this->y - player_y;
				difx = player_x - this->x;

				temp_angle = atan2f(dify,difx)*(180.0f/PI);				
				temp_angle = -1*(temp_angle);
		}
		else if(player_y > this->y){
			//QUARTER 1 FOR ZOMBIE
			dify = player_y - this->y;
			difx = player_x - this->x;

			temp_angle = atan2f(dify,difx)*(180.0f/PI);
		}
		else{
			temp_angle = 360;//0
		}
	}
	else{
		if(player_y < this->y){
			temp_angle = 270;//0
		}else if(player_y > this->y){
			temp_angle = 90;//-270
		}
	}	
	this->setAngle(temp_angle);
}


void Zombie::update(double delta, float x, float y, std::vector<BBox>* boxes){	
	if(this->isAlive()){	
		this->chargeAttack(delta);
		BBox box;
		if(!checkCollision(boxes,x,y))
			this->turn(x, y);	
		

		this->move(delta);
		this->bar->update(this);
	}
}

void Zombie::draw(){
	if(this->isAlive()){
		this->drawZombie();	
		this->drawBox();
		this->drawHealthbar();
	}
}

void Zombie::drawBox(){
	if(drawLines)
		this->getBox().draw();
}

void Zombie::drawZombie(){
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
			glTexCoord2f(0.0, 1.0); glVertex2f(this->x,this->getY2());
			glTexCoord2f(1.0, 1.0); glVertex2f(this->getX2(),this->getY2());
			glTexCoord2f(1.0, 0.0); glVertex2f(this->getX2(),this->y);
		glEnd();

		glBegin(GL_QUADS);
		
		bar->draw();

		glEnd();

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);	
	glPopMatrix();	
}

void Zombie::drawHealthbar(){
	int hvalue = 0;

	hvalue = (this->health * this->width)/this->MAX_HEALTH;

	glLoadIdentity();	
	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3f(0.7f,0.7f,0.7f);
		glVertex2f(this->x - this->width/2,this->y+this->height/2+2-1);
		glVertex2f(this->x - this->width/2,this->y+this->height/2+2+5);
		glVertex2f(this->x + this->width/2, this->y+this->height/2+2 + 5);
		glVertex2f(this->x + this->width/2, this->y+this->height/2+2);
	glEnd();
	glPopMatrix();

	//bar value drawn
	glPushMatrix();	
		glBegin(GL_QUADS);
		glColor3ub(0,255,0);
			glVertex2f(this->x - this->width/2,this->y+this->height/2+2);
			glVertex2f(this->x - this->width/2,this->y+this->height/2+2+5);
			glVertex2f(this->x - this->width/2 + hvalue, this->y+this->height/2+2+5);
			glVertex2f(this->x - this->width/2 + hvalue, this->y+this->height/2+2);	
			glColor3f(1,1,1);
		glEnd();	
	glPopMatrix();

	glLineWidth(1.0f);
	glPushMatrix();
	glBegin(GL_LINE_LOOP);
		glVertex2f(this->x - this->width/2,this->y+this->height/2+2-1);
		glVertex2f(this->x - this->width/2,this->y+this->height/2+2+5);
		glVertex2f(this->x + this->width/2,this->y+this->height/2+2 + 5);
		glVertex2f(this->x + this->width/2, this->y+this->height/2+2);
	glEnd();
	glPopMatrix();
}

bool Zombie::isAlive(){
	return this->alive;
}

void Zombie::kill(){
	this->alive = false;
}

bool Zombie::canAttack(){
	return this->readyToAttack;
}


int Zombie::getHealth(){
	return this->health;
}

int Zombie::getMaxHealth(){
	return this->health;
}

float Zombie::getAttackTimer(){
	return this->attackTimer;
}

int Zombie::getExpPoints(){
	return this->points;
}

void Zombie::changeHealth(int amount){
	if((this->health + amount) <= this->MAX_HEALTH )
			this->health += amount;
	else
		this->health = MAX_HEALTH;

	if(this->health <= 0)
		this->kill();
}

void Zombie::attack(Player* p){
	if(canAttack()){
		this->multiplier = 0;
		p->changeHealth(-1*this->damage);	
		this->readyToAttack = false;
	}
}

//charging attack depending on a interval time, so wont attack every second
void Zombie::chargeAttack(double delta){
	if(!canAttack()){
		attackTimer += (float) delta;
		
		//interval time 200000.0
		if(attackTimer >= 200000.0){
			readyToAttack = true;
			attackTimer = 0.0;
		}
	}
}
//circle to circle collision  to check if it s  colliding with the player
bool Zombie::checkPlayerCollision(Player* player){
	float difx = (this->getX() - player->getX())* (this->getX() - player->getX());
	float dify = (this->getY() - player->getY()) *(this->getY() - player->getY()) ;
	float dist = difx*difx + dify*dify;
	float r2 = (this->getRadius()  + player->getRadius())*(this->getRadius() + player->getRadius());
	if(dist <= (r2))
		return true;
	return false;
}

//circle(zombie) to rectangle collision detection
bool Zombie::checkCollision(std::vector<BBox>* boxes, float p_x, float p_y){
	BBox box;
	Point A = Point(this->getX(), this->getY());
	Point B;
	for(int i = 0; i<boxes->size();i++){
		box = boxes->at(i);

		//finds a point on the rectangles bounding box and that point acts as a circle with a radius 0 radius
		if(box.getRight() < this->getBox().getLeft())
			B.setX(box.getRight());
		else if(box.getLeft() > this->getBox().getRight())
			B.setX(box.getLeft());
		else
			B.setX(this->getX());

		if(box.getTop() < this->getBox().getBottom())
			B.setY(box.getTop());
		else if(box.getBottom() > this->getBox().getTop())
			B.setY(box.getBottom());
		else
			B.setY(this->getY());


		float difx = (B.getX() - A.getX()) * (B.getX() - A.getX());
		float dify = (B.getY() - A.getY()) * (B.getY() - A.getY());

		float dist = difx + dify;
		if(dist <= this->getRadius() * this->getRadius()){
		
		//FOLLOWS THE EDGES OF BOX TO REACH the player

			//if zombie is on below or above the box
			if((this->getBox().getBottom() > box.getTop() ||
				this->getBox().getTop()  < box.getBottom())
			){
					//IF ON TOP
				if(this->getBox().getBottom() > box.getTop()){
					if(p_x == this->getX()){
						this->angle = 0;
					}
					//if player is on the right of the zombie
					else if(p_x > this->getX()){
						//if the player is positioned higher than the zombie
						if(this->getY() < p_y)
							this->angle = 8;
						else
							this->angle = 0;
						//if player is on the left of the zombie
					}else {
						//if the player is positioned higher than the zombie
						if(this->getY() < p_y)
							this->angle = 172;
						else
							this->angle = 180;
					}
					//IF BELOW THE BOX
				}else if(this->getBox().getTop() < box.getBottom()){
					if(p_x == this->getX()){
						this->angle = 0;
					}
					//if player is on the right of the zombie
					else if(p_x > this->getX()){
						if(this->getY() > p_y)
							this->angle = 352;
						else
							this->angle = 0;
						//if player is on the left of the zombie
					}else{
						if(this->getY() > p_y)
							this->angle = 188;
						else
							this->angle = 180;
					}
				}
		//if zombie is on left or right side of the box
		}else if((this->getBox().getRight() < box.getLeft() || this->getBox().getLeft() > box.getRight())
			){
		
				float up = box.getTop() - this->getY();
				float down = this->getY() - box.getBottom();
				//IF ON LEFT SIDE
				if(this->getBox().getRight() < box.getLeft()){	
						if(p_y == this->getY() ){
							this->angle = 90;
						}
						else if(p_y > this->getY()){
							if(this->getX() > p_x)
								this->turn(this->getX() - 16,box.getTop() +32);
							else
								this->turn(this->getX(),box.getTop() +32);
						}
						else{
							if(this->getX() > p_x)
								this->turn(this->getX() - 16,box.getBottom() -32);
							else
								this->turn(this->getX(),box.getBottom() - 32);	
					}

				}
				//IF ON RIGHT SIDE 
				else if(this->getBox().getLeft() > box.getRight()){
					
						if(p_y == this->getY()){
							this->angle = 90;
						}
						else if(p_y > this->getY()){
							if(this->getX() < p_x)
								this->turn(this->getX() + 16,box.getTop() + 32);
							else
								this->turn(this->getX(),box.getTop() +32);
						}else{
							if(this->getX() < p_x)
								this->turn(this->getX() + 16,box.getBottom() - 32);
							else
								this->turn(this->getX(),box.getBottom() - 32);	
					}
				}									
			}	
			return true;
		}else
			continue;
	}
	return false;
}

//checks if zombies positions are below or under the boundary lines
bool Zombie::isOutOfTrack(std::vector<BoundaryLine>* lines){

	for(int i = 0;i<lines->size();i++){
		BoundaryLine bl = lines->at(i);
		if(this->x >= bl.getBox().getLeft() && this->x < bl.getBox().getRight() && this->y >= bl.getBox().getBottom() && this->y < bl.getBox().getTop()){
			float result = ((bl.getB().getX() - bl.getA().getX()) * (this->y - bl.getA().getY())) - ((bl.getB().getY() - bl.getA().getY()) * (this->x - bl.getA().getX()));

			if(result > 0 && bl.getTrackside() > 0 || result < 0 && bl.getTrackside() < 0){								
				return false;
			}
			else {			
				return true;
			}		
		}else
			continue;	
	}
	return false;
}


