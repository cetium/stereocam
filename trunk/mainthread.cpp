#include "mainthread.hpp"

using namespace cv;

static Size CAM_SIZE(640, 480);

using namespace cv;
using namespace std;

MainThread::MainThread(): QThread(){
	//blackOne = Mat(Settings::CAM_SIZE, CV_8U, Scalar(0));
	blackOne = cvCreateImage(Settings::CAM_SIZE, CV_8U, 3);
	cvZero(blackOne);
}


MainThread::~MainThread(){
	cvReleaseImage(&blackOne);
	delete camLeft;
	delete camRight;
}

bool MainThread::init(CloseDialog * dialog){
	mainDialog = dialog;
	mainDialog->setMainThread(this);

#ifdef TESTGUI
	initCameras();
	initModules();
	initDialogs();
	return true;
#else
	return (initCameras() &&
			initModules() &&
			initDialogs());
#endif
}

void MainThread::run(){

	//moveToThread(this);

	while(opencvMain());
}

// initialization
bool MainThread::initCameras(){

	camLeft = new CameraDevice(0);
	camRight = new CameraDevice(1);

	if(camLeft->init(CAM_SIZE.width, CAM_SIZE.height) || 
		!camRight->init(CAM_SIZE.width, CAM_SIZE.height))
		return false;
	return true;
}


bool MainThread::initModules(){
	stereoModule.init(&settings, mainDialog);
	calibModule.init(&settings, mainDialog, Size(11, 8));
	processModule.init(&settings, mainDialog);
	return true;
}


bool MainThread::initDialogs(){
	leftImage = new ImageDialog(mainDialog, "Obraz z kamery 1", Size(640, 480), Point(220, 20));
	rightImage = new ImageDialog(mainDialog, "Obraz z kamery 2", Size(640, 480), Point(860, 20));
	return true;
}


bool MainThread::opencvMain(){
	IplImage * frame1, * frame2;

	waitKey(20);
		
#ifdef TEST
	return true;
#endif

	if(camLeft->hasNextFrame() && camRight->hasNextFrame()){
		
		frame1 = camLeft->getNextFrame();
		frame2 = camRight->getNextFrame();

		Mat mat1 = Mat(frame1);
		Mat mat2 = Mat(frame2);

		leftImage->showImage(frame1);
		rightImage->showImage(frame2);
	}

	if(settings.state == settings.BEFORE_CALIBRATION){
		return true;
	}

	if(settings.state == settings.CALIBRATION){
		// kalibracja kamer
		
		if(calibModule.addImages(frame1, frame2)){
			leftImage->showImage(blackOne);
			rightImage->showImage(blackOne);
		}
		return true;
	}
	
	IplImage * afterProcess1, * afterProcess2;

	if(settings.state == settings.MATCH){
		afterProcess1 = processModule.preMatchingProcessing(frame1);
		afterProcess2 = processModule.preMatchingProcessing(frame2);
	}

	IplImage * disparity;

	if(settings.state == settings.MATCH){

		if(afterProcess1 != NULL && afterProcess2 != NULL)
			disparity = stereoModule.match(afterProcess1, afterProcess2);
	}

	return true;
}