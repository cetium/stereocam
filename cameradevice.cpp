#include "cameradevice.hpp"


videoInput CameraDevice::vi;

CameraDevice::CameraDevice(int id){
	this->id = id;
	this->img = NULL;
}

CameraDevice::~CameraDevice(){
	stop();
	if(img)		cvReleaseImage(&img);
}

bool CameraDevice::init(){
	vi.setupDevice(id);

	if(!vi.isDeviceSetup(id))
		return false;

	this->width = vi.getWidth(id);
	this->height = vi.getHeight(id);

	vi.setIdealFramerate(id, 40);
	img = cvCreateImage(cvSize(width, height), 8, 3);

	return true;
}

bool CameraDevice::init(int width, int height){
	this->width = width;
	this->height = height;

	vi.setupDevice(id, width, height);

	if(!vi.isDeviceSetup(id))
		return false;

	vi.setIdealFramerate(id, 40);
	img = cvCreateImage(cvSize(width, height), 8, 3);

	return true;
}

void CameraDevice::stop(){
	vi.stopDevice(id);
}

IplImage * CameraDevice::getNextFrame(){
	vi.getPixels(id, (unsigned char *)img->imageData, false, true);
	return img;
}

bool CameraDevice::hasNextFrame(){
	return vi.isFrameNew(id);
}