#include "Map.h"

#include <windows.h>		// Header file for Windows
#include <gl\gl.h>			// Header file for the OpenGL32 Library
#include <gl\glu.h>			// Header file for the GLu32 Library
#include <math.h>
#include <sstream>
#include <fstream>
using namespace std;



Map::Map(){
	this->width = 800;
	this->height = 800;
	this->drawLines = false;
}


Map::Map(int width, int height){
	this->width = width;
	this->height = height;
	this->drawLines = false;
}

void Map::setupBoundingBoxes(){
	BBox box0 = BBox(416,352,608-416,416-352);
	BBox box1 = BBox(352,512,512-352,672-512);
	BBox box2 = BBox(608,640,672-608,768-640);
	BBox box3 = BBox(376,0,384-376,160-0);
	BBox box4 = BBox(480,0,488-480,160-0);
	BBox box5 = BBox(704,256,712-704,448-256);
	BBox box6 = BBox(512,512,608-512,520-512);
	BBox box7 = BBox(248,480,256-248,672-480);
	BBox box8 = BBox(352,768,608-352,776-768);
	BBox box9 = BBox(512,536,672-512,544-536);
	BBox box10 = BBox(768,608,776-768,808-608);
	BBox box11 = BBox(496,896,624-496,904-896);
	BBox box12 = BBox(64,824,400-64,832-824);

	boundingBoxes.push_back(box0);
	boundingBoxes.push_back(box1);
	boundingBoxes.push_back(box2);
	boundingBoxes.push_back(box3);
	boundingBoxes.push_back(box4);
	boundingBoxes.push_back(box5);
	boundingBoxes.push_back(box6);
	boundingBoxes.push_back(box7);
	boundingBoxes.push_back(box8);
	boundingBoxes.push_back(box9);
	boundingBoxes.push_back(box10);
	boundingBoxes.push_back(box11);
	boundingBoxes.push_back(box12);

}

void Map::setupTrackLines(){
	BoundaryLine l0 = BoundaryLine(384,160,384,224,-1);
	BoundaryLine l1 = BoundaryLine(384,224,512,352,-1);
	BoundaryLine l2 = BoundaryLine(480,160,544,256,1);
	BoundaryLine l3 = BoundaryLine(544,256,704,256,1);
	BoundaryLine l4 = BoundaryLine(608,512,704,448,-1);
	BoundaryLine l5 = BoundaryLine(320,416,416,416,1);
	BoundaryLine l6 = BoundaryLine(256,480,320,416,1);
	BoundaryLine l7 = BoundaryLine(256,672,352,768,-1);
	BoundaryLine l8 = BoundaryLine(672,544,768,608,1);
	BoundaryLine l9 = BoundaryLine(624,896,768,896,-1);
	BoundaryLine l10 = BoundaryLine(160,896,496,896,-1);
	BoundaryLine l11 = BoundaryLine(768,808,768,896,1);
	BoundaryLine l12 = BoundaryLine(608,832,672,768,1);
	BoundaryLine l13 = BoundaryLine(400,832,608,832,1);
	BoundaryLine l14 = BoundaryLine(64,832,64,960,-1);
	BoundaryLine l15 = BoundaryLine(160,896,160,960,1);
	trackLines.push_back(l0);
	trackLines.push_back(l1);
	trackLines.push_back(l2);
	trackLines.push_back(l3);
	trackLines.push_back(l4);
	trackLines.push_back(l5);
	trackLines.push_back(l6);
	trackLines.push_back(l7);
	trackLines.push_back(l8);
	trackLines.push_back(l9);
	trackLines.push_back(l10);
	trackLines.push_back(l11);
	trackLines.push_back(l12);
	trackLines.push_back(l13);
	trackLines.push_back(l14);
	trackLines.push_back(l15);

}




void Map::draw(){
	this->drawBackground();

	if(this->drawLines == true){
		this->drawTrackLines();
		this->drawBoundingBoxes();
	}
}

void Map::drawBackground(){	
	glLoadIdentity();
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, bg_texture);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0); glVertex2f(0,0);
				glTexCoord2f(0.0, 1.0); glVertex2f(0,this->height);
				glTexCoord2f(1.0, 1.0); glVertex2f(this->width,this->height);
				glTexCoord2f(1.0, 0.0); glVertex2f(this->width,0);
			glEnd();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	glPopMatrix();
}

void Map::drawTrackLines(){
	for(int i = 0;i<trackLines.size();i++){
		trackLines[i].draw();
	}
}

void Map::drawBoundingBoxes(){
	for(int i = 0;i<boundingBoxes.size();i++){
		boundingBoxes[i].draw();
	}
}

//gets the stony fields on the map ( yellow bits)
int Map::getTile(float x, float y){
	int temp = 0;
	int tiles[30][25] = 
			{
				{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,1},//bottom line
				{0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,0},
				{0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,0},
				{0,1,1,0,1,1,1,0,1,0,0,0,0,0,0,0,0,1,1,0,0,1,0,1,0},
				{0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,1,0,0},
				{0,1,0,1,1,1,1,1,1,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0},
				{0,0,0,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,1,0,1,0,1,0,0},
				{0,1,0,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
				{0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
				{0,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
				{0,1,1,0,0,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
				{0,0,0,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
				{0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
				{0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
				{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
				{0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0},
				{0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0},
				{0,1,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
				{0,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
				{0,1,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
				{0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
				{0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
				{0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0},
				{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0},
				{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
				{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
				{0,1,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
				{1,0,0,0,0,0,0,1,0,1,0,1,1,0,0,0,0,1,0,0,1,0,1,1,1}//top line
	 };


	int col = (int)y / 32;
	if(y < 32)
		col = 0;
	int row =(int) x / 32;
	if(x < 32)
		row = 0;
	temp = tiles[col][row];

		return temp;
	}

void Map::setBgTexture(GLuint id){
	this->bg_texture = id;
}

int Map::getHeight(){
	return this->height;
}

int Map::getWidth(){
	return this->width;
}

std::vector<BoundaryLine> Map::getTrackLines(){
	return this->trackLines;
}

std::vector<BBox> Map::getBBoxes(){
	return this->boundingBoxes;
}

