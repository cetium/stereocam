#include <stdio.h>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <videoInput.h>
#include <opencv2/imgproc/imgproc.hpp>


#include <GL/glut.h>

#include "cameradevice.hpp"
#include "cameracalibration.hpp"
#include "simple3dscene.hpp"

using namespace cv;
using namespace std;


void opencvMain();
void handleResize(int w, int h);
void handleKeypress(unsigned char key, int x, int y);
void handleSpecialKeypress(int key, int x, int y);
void drawScene();


Simple3DScene scene;





int main( int argc, char** argv ){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	
	glutCreateWindow("3d scene");
	
	scene.init(400, 400);

	//glutIdleFunc(drawScene);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutSpecialFunc(handleSpecialKeypress);

	glutMainLoop();

	return 0;
}


void handleResize(int w, int h){
	scene.resize(w, h);
}


void drawScene(){
	scene.draw();
}


void handleKeypress(unsigned char key, int x, int y)
{
	switch (key){
		case 27:
			exit(0);
			break;
	}
}


void handleSpecialKeypress(int key, int x, int y) {
	switch (key){
		case GLUT_KEY_LEFT : 
			//angle -= 0.01f;
			scene.orientMe(-1.0f);
			break;
		case GLUT_KEY_RIGHT : 
			scene.orientMe(1.0f);
			//orientMe(angle);
			break;
		case GLUT_KEY_UP : 
			scene.moveFlat(1);
			break;
		case GLUT_KEY_DOWN : 
			scene.moveFlat(-1);
			break;
	}
}

void opencvMain(){
	Size camSize(640, 480);
	//Size camSize(320, 240);

	CameraDevice camLeft(1);
	CameraDevice camRight(0);

	CameraCalibration cameraCalibrationDevice;

	if(!camLeft.init(camSize.width, camSize.height) || !camRight.init(camSize.width, camSize.height)){
		std::cout << "Nie udalo sie zainicjowac kamer" << endl;
		system("pause");
		return;
	}

	namedWindow("camLeft");
	namedWindow("camRight");

	cout << "Obsluga programu:"<<endl;
	cout << " c - kalibracja"<<endl;
	cout << " q - wyjscie"<<endl;

	char c;
	bool calibrate = false;
	bool hasToCalibrate = !cameraCalibrationDevice.hasCalibrationData();
	bool showDisparity = false;
	bool showCloud = false;

	int counter = 1;
	int maxPairs = 15, samplesTime = 20;

	vector<Mat> camImages;

	Mat mat1, mat2;
	Mat blackOne(camSize, CV_8U, Scalar(0));

	CvStereoGCState * state = cvCreateStereoGCState(128, 7);

	while(true){

		if((c = waitKey(30) ) == 'q' || c == 'Q')
			break;
		// rozpoczynamy kalibracje
		else if(!calibrate && (c == 'c' || c == 'C')){
			calibrate = true;
		}else if(!calibrate && (c == 'd' || c == 'D')){
			showDisparity = !showDisparity;
			cameraCalibrationDevice.initDisparityImage(camSize);
		}else if(showDisparity && (c == 'x' || c == 'X')){
			showCloud = !showCloud;
		}
		
		if(camLeft.hasNextFrame() && camRight.hasNextFrame()){
			IplImage * frame1 = camLeft.getNextFrame();
			IplImage * frame2 = camRight.getNextFrame();

			mat1 = Mat(frame1);
			mat2 = Mat(frame2);

			imshow("camLeft", mat1);
			imshow("camRight", mat2);

			// kalibracja
			if(calibrate){
				if(counter % samplesTime == 0){
					IplImage * grey1 = cvCreateImage(cvGetSize(frame1), 8, 1);
					IplImage * grey2 = cvCreateImage(cvGetSize(frame2), 8, 1);
					cvCvtColor(frame1, grey1, CV_BGR2GRAY);
					cvCvtColor(frame2, grey2, CV_BGR2GRAY);
					camImages.push_back(Mat(grey2));
					camImages.push_back(Mat(grey1));
					cout << counter/samplesTime << endl;
					imshow("camLeft", blackOne);
					imshow("camRight", blackOne);
				}
				
				// no to mozemy kalibrowac
				if(counter == maxPairs*samplesTime){
					destroyWindow("camLeft");
					destroyWindow("camRight");
					cameraCalibrationDevice.calibrate(camImages, Size(9,6), 1.0f);
					calibrate = false;
					hasToCalibrate = false;
				}
				++counter;
			}

			// disparity image
			if(showDisparity){
				Mat grey1, grey2;
				cvtColor(mat1, grey1, CV_BGR2GRAY);
				cvtColor(mat2, grey2, CV_BGR2GRAY);
				cameraCalibrationDevice.showDisparityImage(grey1, grey2, 1, showCloud);
				showCloud = false;
			}
		}
	}

	camLeft.stop();
	camRight.stop();
}

