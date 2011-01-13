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


/** OPENGL elements for drawing a scene */
void initOpenGL();
void handleResize(int w, int h);
void handleKeypress(unsigned char key, int x, int y);
void handleSpecialKeypress(int key, int x, int y);
void drawScene();
void update(int value);
void handleMouse(int x, int y);
void handleMouseButtons(int button, int state, int x, int y);

Simple3DScene scene;
const int TIMER_MS = 30;

int g_lastX = -1;
int g_lastY = -1;
bool g_leftButtonPressed = false;
bool g_rightButtonPressed = false;




/** OPENCV elements for handling cameras */
void initOpenCV();
bool updateOpenCV();
void closeOpenCV();

Size CAM_SIZE(640, 480);

CameraDevice g_camLeft(1);
CameraDevice g_camRight(0);
CameraCalibration g_cameraCalibrationDevice;

bool g_calibrate = false;
bool g_hasToCalibrate = false;
bool g_showDisparity = false;
bool g_showCloud = false;

int g_counter = 1;
int g_maxPairs = 15, g_samplesTime = 20;

vector<Mat> g_camImages;
Mat g_blackOne(CAM_SIZE, CV_8U, Scalar(0));




int main( int argc, char** argv ){
	glutInit(&argc, argv);
	
	initOpenGL();
	initOpenCV();

	glutTimerFunc(TIMER_MS, update, 0);
	glutMainLoop();

	closeOpenCV();

	return 0;
}



void initOpenCV(){
	if(!g_camLeft.init(CAM_SIZE.width, CAM_SIZE.height) || !g_camRight.init(CAM_SIZE.width, CAM_SIZE.height)){
		std::cout << "Nie udalo sie zainicjowac kamer" << endl;
		system("pause");
		//exit(1);
	}

	namedWindow("g_camLeft");
	namedWindow("g_camRight");

	cout << "Obsluga programu:"<<endl;
	cout << " c - kalibracja"<<endl;
	cout << " q - wyjscie"<<endl;

	g_hasToCalibrate = !g_cameraCalibrationDevice.hasCalibrationData();
}


bool updateOpenCV(){
	char c;
	if((c = waitKey(30) ) == 'q' || c == 'Q')
		return false;
	// rozpoczynamy kalibracje
	else if(!g_calibrate && (c == 'c' || c == 'C')){
		g_calibrate = true;
	}else if(!g_calibrate && (c == 'd' || c == 'D')){
		g_showDisparity = !g_showDisparity;
		g_cameraCalibrationDevice.initDisparityImage(CAM_SIZE);
	}else if(g_showDisparity && (c == 'x' || c == 'X')){
		g_showCloud = !g_showCloud;
	}
		
	if(g_camLeft.hasNextFrame() && g_camRight.hasNextFrame()){
		IplImage * frame1 = g_camLeft.getNextFrame();
		IplImage * frame2 = g_camRight.getNextFrame();

		Mat mat1 = Mat(frame1);
		Mat mat2 = Mat(frame2);

		imshow("g_camLeft", mat1);
		imshow("g_camRight", mat2);

		// kalibracja
		if(g_calibrate){
			if(g_counter % g_samplesTime == 0){
				IplImage * grey1 = cvCreateImage(cvGetSize(frame1), 8, 1);
				IplImage * grey2 = cvCreateImage(cvGetSize(frame2), 8, 1);
				cvCvtColor(frame1, grey1, CV_BGR2GRAY);
				cvCvtColor(frame2, grey2, CV_BGR2GRAY);
				g_camImages.push_back(Mat(grey2));
				g_camImages.push_back(Mat(grey1));
				cout << g_counter/g_samplesTime << endl;
				imshow("g_camLeft", g_blackOne);
				imshow("g_camRight", g_blackOne);
			}
				
			// no to mozemy kalibrowac
			if(g_counter == g_maxPairs*g_samplesTime){
				destroyWindow("g_camLeft");
				destroyWindow("g_camRight");
				g_cameraCalibrationDevice.calibrate(g_camImages, Size(9,6), 1.0f);
				g_calibrate = false;
				g_hasToCalibrate = false;
			}
			++g_counter;
		}

		// disparity image
		if(g_showDisparity){
			Mat grey1, grey2;
			cvtColor(mat1, grey1, CV_BGR2GRAY);
			cvtColor(mat2, grey2, CV_BGR2GRAY);
			g_cameraCalibrationDevice.showDisparityImage(grey1, grey2, 1, g_showCloud);
			g_showCloud = false;
		}
	}

	return true;
}


void closeOpenCV(){
	g_camLeft.stop();
	g_camRight.stop();
}


/** OPENGL functions */


void initOpenGL(){
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(500,480);
	glutInitWindowSize(400, 400);
	
	glutCreateWindow("3d scene");
	
	scene.init(400, 400);

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutSpecialFunc(handleSpecialKeypress);
	glutMotionFunc(handleMouse);
	glutMouseFunc(handleMouseButtons);
}

void handleResize(int w, int h){
	scene.resize(w, h);
}

void drawScene(){
	scene.draw();
}

void update(int value){

	if(!updateOpenCV()){
		closeOpenCV();
		exit(0);
	}

	glutPostRedisplay();
	glutTimerFunc(TIMER_MS, update, 0);
}

void handleKeypress(unsigned char key, int x, int y){
	switch (key){
		case 27:
			exit(0);
			break;
	}
}

void handleSpecialKeypress(int key, int x, int y) {
	switch (key){
		case GLUT_KEY_LEFT : 
			scene.rotate(-2, 0);
			break;
		case GLUT_KEY_RIGHT : 
			scene.rotate(2, 0);
			break;
		case GLUT_KEY_UP : 
			scene.rotate(0, -2);
			break;
		case GLUT_KEY_DOWN : 
			scene.rotate(0, 2);
			break;
	}
}

void handleMouse(int x, int y){

	// tak w razie czego
	if(g_lastX == -1 || g_lastY == -1){
		g_lastX = x;
		g_lastY = y;
		return;
	}
	if(g_leftButtonPressed){
		scene.rotate(g_lastX - x, g_lastY - y);
	}else if(g_rightButtonPressed){
		scene.moveFlat(g_lastY - y);
	}
	g_lastX = x;
	g_lastY = y;
}

void handleMouseButtons(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON){
		g_lastX = x;
		g_lastY = y;
		g_leftButtonPressed = (state == GLUT_DOWN);
	}else if(button == GLUT_RIGHT_BUTTON){
		g_lastX = x;
		g_lastY = y;
		g_rightButtonPressed = (state == GLUT_DOWN);
	}
}




void opencvMain(){
	Size CAM_SIZE(640, 480);
	//Size CAM_SIZE(320, 240);

	CameraDevice g_camLeft(1);
	CameraDevice g_camRight(0);

	CameraCalibration g_cameraCalibrationDevice;

	if(!g_camLeft.init(CAM_SIZE.width, CAM_SIZE.height) || !g_camRight.init(CAM_SIZE.width, CAM_SIZE.height)){
		std::cout << "Nie udalo sie zainicjowac kamer" << endl;
		system("pause");
		return;
	}

	namedWindow("g_camLeft");
	namedWindow("g_camRight");

	cout << "Obsluga programu:"<<endl;
	cout << " c - kalibracja"<<endl;
	cout << " q - wyjscie"<<endl;

	char c;
	bool g_calibrate = false;
	bool g_hasToCalibrate = !g_cameraCalibrationDevice.hasCalibrationData();
	bool g_showDisparity = false;
	bool g_showCloud = false;

	int g_counter = 1;
	int g_maxPairs = 15, g_samplesTime = 20;

	vector<Mat> g_camImages;

	Mat mat1, mat2;
	Mat blackOne(CAM_SIZE, CV_8U, Scalar(0));

	CvStereoGCState * state = cvCreateStereoGCState(128, 7);

	while(true){

		if((c = waitKey(30) ) == 'q' || c == 'Q')
			break;
		// rozpoczynamy kalibracje
		else if(!g_calibrate && (c == 'c' || c == 'C')){
			g_calibrate = true;
		}else if(!g_calibrate && (c == 'd' || c == 'D')){
			g_showDisparity = !g_showDisparity;
			g_cameraCalibrationDevice.initDisparityImage(CAM_SIZE);
		}else if(g_showDisparity && (c == 'x' || c == 'X')){
			g_showCloud = !g_showCloud;
		}
		
		if(g_camLeft.hasNextFrame() && g_camRight.hasNextFrame()){
			IplImage * frame1 = g_camLeft.getNextFrame();
			IplImage * frame2 = g_camRight.getNextFrame();

			mat1 = Mat(frame1);
			mat2 = Mat(frame2);

			imshow("g_camLeft", mat1);
			imshow("g_camRight", mat2);

			// kalibracja
			if(g_calibrate){
				if(g_counter % g_samplesTime == 0){
					IplImage * grey1 = cvCreateImage(cvGetSize(frame1), 8, 1);
					IplImage * grey2 = cvCreateImage(cvGetSize(frame2), 8, 1);
					cvCvtColor(frame1, grey1, CV_BGR2GRAY);
					cvCvtColor(frame2, grey2, CV_BGR2GRAY);
					g_camImages.push_back(Mat(grey2));
					g_camImages.push_back(Mat(grey1));
					cout << g_counter/g_samplesTime << endl;
					imshow("g_camLeft", blackOne);
					imshow("g_camRight", blackOne);
				}
				
				// no to mozemy kalibrowac
				if(g_counter == g_maxPairs*g_samplesTime){
					destroyWindow("g_camLeft");
					destroyWindow("g_camRight");
					g_cameraCalibrationDevice.calibrate(g_camImages, Size(9,6), 1.0f);
					g_calibrate = false;
					g_hasToCalibrate = false;
				}
				++g_counter;
			}

			// disparity image
			if(g_showDisparity){
				Mat grey1, grey2;
				cvtColor(mat1, grey1, CV_BGR2GRAY);
				cvtColor(mat2, grey2, CV_BGR2GRAY);
				g_cameraCalibrationDevice.showDisparityImage(grey1, grey2, 1, g_showCloud);
				g_showCloud = false;
			}
		}
	}

	g_camLeft.stop();
	g_camRight.stop();
}

