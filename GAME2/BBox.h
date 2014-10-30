#ifndef _BBOX_H
#define _BBOX_H

class BBox{
private:
	float left,right,bottom,top;
	float width,height;
public:
	BBox();
	BBox(float, float, float,float);

	float getLeft();
	float getRight();
	float getTop();
	float getBottom();
	float getWidth();
	float getHeight();

	void setX(float x);
	void setY(float y);

	void draw();
};

#endif _BBOX_H