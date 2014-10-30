#ifndef _MAP_H
#define _MAP_H

#include <windows.h>		// Header file for Windows
#include <gl\gl.h>			// Header file for the OpenGL32 Library
#include <gl\glu.h>	
#include <string>
#include "BoundaryLine.h"
#include <vector>



class Map{
private:
	int width;
	int height;

	GLuint bg_texture;
	
	std::vector<BBox> boundingBoxes;
	std::vector<BoundaryLine> trackLines;
public:
	//if set to TRUE, draws bounding boxes and track lines
	bool drawLines;

	//constructors
	Map();
	Map(int widht, int height);
	
	
	//getter methods
	int getWidth();
	int getHeight();
	
	std::vector<BBox> getBBoxes();
	std::vector<BoundaryLine> getTrackLines();
	
	//LINES/BBOXES SETUP
	void setupTrackLines();
	void setupBoundingBoxes();
	
	
	int getTile(float x, float y);
	
	void setBgTexture(GLuint);

	//DISPLAY METHODS
	void draw();
	void drawBackground();
	void drawTrackLines();
	void drawBoundingBoxes();
};

#endif _MAP_H