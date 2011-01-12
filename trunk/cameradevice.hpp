#pragma once

#include <cv.h>
#include <highgui.h>
#include <videoInput.h>



class CameraDevice
{
public:
	CameraDevice(int id);

	~CameraDevice();

	bool init();
	bool init(int width, int height);

	void stop();

	IplImage * getNextFrame();
	bool hasNextFrame();


	int getWidth()	{	return width;	}
	int getHeight()	{	return height;	}
	int getId()		{	return id;		}

	static videoInput vi;

private:
	
	int width;
	int height;
	int id;

	IplImage * img;

	bool initialized;
};

