#pragma once

#include <cv.h>
#include <cxcore.h>

class Simple3DScene
{
public:
	Simple3DScene();
	~Simple3DScene();

	void init(int w, int h);
	void draw();
	void resize(int w, int h);

	void moveFlat(int direction);
	void rotate(int xdiff, int ydiff);

	void setPointCloud(cv::Mat & mat);

private:
	void drawPointCloud();


	cv::Mat cloud;
};

