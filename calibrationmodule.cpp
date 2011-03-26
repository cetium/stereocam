#include "calibrationmodule.hpp"

#include <sstream>
#include <QVBoxLayout>
#include <QPushButton>

using namespace std;
using namespace cv;


CalibrationModule::CalibrationModule(){
	blackOne = Mat(Settings::CAM_SIZE, CV_8U, Scalar(0));

	counter = 0;
	samplesTime = 20;
}

CalibrationModule::~CalibrationModule(){

}

void CalibrationModule::init(Settings * sets, QDialog * dial, Size theBoardSize, int pairs){
	settings = sets;
	maxPairs = pairs;
	boardSize = theBoardSize;

	blackOne = Mat(Settings::CAM_SIZE, CV_8U, Scalar(0));
	dialog = dial;
	initWindow();
}


void CalibrationModule::initWindow(){
	dialog->setWindowTitle("Kalibracja");
	
	QVBoxLayout * layout = new QVBoxLayout(dialog);

	calibrateButton = new QPushButton("Kalibruj", dialog);
	connect(calibrateButton,	SIGNAL(clicked()),
			this,				SLOT(startCalibration()));

	loadCalibrationButton = new QPushButton("Wczytaj skalibrowane parametry", dialog);
	connect(loadCalibrationButton,	SIGNAL(clicked()),
			this,					SLOT(loadCalibration()));

	layout->addWidget(calibrateButton);
	layout->addWidget(loadCalibrationButton);

	dialog->setLayout(layout);
	dialog->move(10,20);
	dialog->setMinimumWidth(200);
	dialog->adjustSize();
}


void CalibrationModule::startCalibration(){
	settings->state = Settings::CALIBRATION;
	dialog->hide();
}

void CalibrationModule::loadCalibration(){

}



void CalibrationModule::addImages(IplImage * frame1, IplImage * frame2){

	if(counter % samplesTime == 0){

		IplImage * grey1 = cvCreateImage(cvGetSize(frame1), 8, 1);
		IplImage * grey2 = cvCreateImage(cvGetSize(frame2), 8, 1);

		cvCvtColor(frame1, grey1, CV_BGR2GRAY);
		cvCvtColor(frame2, grey2, CV_BGR2GRAY);

		imshow(Settings::WIN1, blackOne);
		imshow(Settings::WIN2, blackOne);

		camImages.push_back(grey1);
		camImages.push_back(grey2);
	}
	++counter;

	if(counter*samplesTime == maxPairs){
		if(calibrate())
			settings->state = Settings::AFTER_CALIBRATION;
		else
			clearCalibrationData();
	}
}

void CalibrationModule::clearCalibrationData(){
	camImages.clear();
	counter = 0;
	settings->state = Settings::BEFORE_CALIBRATION;
	dialog->show();
}


bool CalibrationModule::calibrate(){
	vector<Mat> goodImages;

	vector<vector<Point2f> > imagePoints[2];
	vector<vector<Point3f> > objectPoints;
	Size imageSize;

	const int maxScale = 1;
	int numImages = camImages.size()/2;
	int i, j, k;

	imagePoints[0].resize(numImages);
	imagePoints[1].resize(numImages);

	// for every pair of images
	for(i = j = 0; i < numImages; ++i){
	
		// for every camera
		for(k = 0; k < 2; ++k){
			
			Mat img = camImages[i*2+k];

			imageSize = img.size();
			vector<Point2f> & corners = imagePoints[k][j];

			bool found = false;
			for( int scale = 1; scale <= maxScale; scale++ ){
                Mat timg;
                if( scale == 1 )
                    timg = img;
                else
                    resize(img, timg, Size(), scale, scale);
                found = findChessboardCorners(timg, boardSize, corners, 
					CV_CALIB_CB_FILTER_QUADS | CV_CALIB_CB_NORMALIZE_IMAGE |CV_CALIB_CB_ADAPTIVE_THRESH);
                
				if( found ){
                    if( scale > 1 ){
                        Mat cornersMat(corners);
                        cornersMat *= 1./scale;
                    }
                    break;
                }
            }

			// polepszenie wyliczenia rogow
			if(found)
				cornerSubPix(img, corners, Size(11,11), Size(-1,-1), TermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 30, 0.01));

			Mat cimg;
			cvtColor(img, cimg, CV_GRAY2BGR);
			drawChessboardCorners(cimg, boardSize, corners, found);
			imshow(Settings::CORNERS, cimg);
			char c = (char)waitKey(1000);

			// wyjscie z programu, aby bylo mozliwe
			if(c == 'q' || c == 'Q')
				exit(-1);
		
			if(!found){
				break;
			}
			
			// jesli jest to drugi obrazek z pary stereo - wiec obydwa przeszly poprawnie 
			if(k==1){
				goodImages.push_back(camImages[i*2]);
				goodImages.push_back(camImages[i*2+1]);
				j++;
			}
		}
	}	
	
	float squareSize = 1.0;
	numImages = j;
	if(numImages < 2){
		waitKey(1000);
		destroyWindow(Settings::CORNERS);
		return false;
	}else{
		waitKey(1000);
		destroyWindow(Settings::CORNERS);
	}

	imagePoints[0].resize(numImages);
	imagePoints[1].resize(numImages);
	objectPoints.resize(numImages);

	for(i = 0; i < numImages; ++i){
		for(j = 0; j < boardSize.height; ++j)
			for(k = 0; k < boardSize.width; ++k)
				objectPoints[i].push_back(Point3f(j*squareSize, k*squareSize, 0));
	}

	Mat cameraMatrix[2], distCoeffs[2];
	cameraMatrix[0] = Mat::eye(3,3,CV_64F);
	cameraMatrix[1] = Mat::eye(3,3,CV_64F);

	Mat E, F;

	double rms = stereoCalibrate(objectPoints, imagePoints[0], imagePoints[1],
								cameraMatrix[0], distCoeffs[0], 
								cameraMatrix[1], distCoeffs[1],
								imageSize, R, T, E, F,
								TermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 100, 1e-5),
								CV_CALIB_FIX_ASPECT_RATIO +
								CV_CALIB_ZERO_TANGENT_DIST +
								//CV_CALIB_FIX_FOCAL_LENGTH +
								CV_CALIB_SAME_FOCAL_LENGTH +
								CV_CALIB_RATIONAL_MODEL +
								CV_CALIB_FIX_K3 + CV_CALIB_FIX_K4 + CV_CALIB_FIX_K5);

	double err = 0;
    int npoints = 0;
    vector<Vec3f> lines[2];
    for( i = 0; i < numImages; i++ ){

        int npt = (int)imagePoints[0][i].size();
        Mat imgpt[2];

        for( k = 0; k < 2; k++ ){

            imgpt[k] = Mat(imagePoints[k][i]);
            undistortPoints(imgpt[k], imgpt[k], cameraMatrix[k], distCoeffs[k], Mat(), cameraMatrix[k]);
            computeCorrespondEpilines(imgpt[k], k+1, F, lines[k]);
        }
        
		for( j = 0; j < npt; j++ ){

            double errij = fabs(imagePoints[0][i][j].x*lines[1][j][0] +
                                imagePoints[0][i][j].y*lines[1][j][1] + lines[1][j][2]) +
                           fabs(imagePoints[1][i][j].x*lines[0][j][0] +
                                imagePoints[1][i][j].y*lines[0][j][1] + lines[0][j][2]);
            err += errij;
        }
        npoints += npt;
    }

	cameraMatrix[0].copyTo(M1);
	cameraMatrix[1].copyTo(M2);
	distCoeffs[0].copyTo(D1);
	distCoeffs[1].copyTo(D2);

	// save intrinsic parameters
    FileStorage fs("intrinsics.yml", CV_STORAGE_WRITE);
    if( fs.isOpened() )
    {
        fs << "M1" << cameraMatrix[0] << "D1" << distCoeffs[0] <<
            "M2" << cameraMatrix[1] << "D2" << distCoeffs[1];
        fs.release();
    }

    Rect validRoi[2];
    
    stereoRectify(cameraMatrix[0], distCoeffs[0],
                  cameraMatrix[1], distCoeffs[1],
                  imageSize, R, T, R1, R2, P1, P2, Q,
                  1, imageSize, &validRoi[0], &validRoi[1]);
        
    fs.open("extrinsics.yml", CV_STORAGE_WRITE);
    if( fs.isOpened() ) {
        fs << "R" << R << "T" << T << "R1" << R1 << "R2" << R2 << "P1" << P1 << "P2" << P2 << "Q" << Q;
        fs.release();
    }

	bool isVerticalStereo = fabs(P2.at<double>(1, 3)) > fabs(P2.at<double>(0, 3));
	if(isVerticalStereo){
		//settings->overlay = "Blad: odczytalo jako pionowe ulozenie kamer";
		return false;
	}

    initUndistortRectifyMap(cameraMatrix[0], distCoeffs[0], R1, P1, imageSize, CV_16SC2, rmap[0][0], rmap[0][1]);
    initUndistortRectifyMap(cameraMatrix[1], distCoeffs[1], R2, P2, imageSize, CV_16SC2, rmap[1][0], rmap[1][1]);
    
    Mat canvas;
    double sf;
    int w, h;

    sf = 1.0;//340./MAX(imageSize.width, imageSize.height);
    w = cvRound(imageSize.width*sf);
    h = cvRound(imageSize.height*sf);
    canvas.create(h, w*2, CV_8UC3);

    
    for( i = 0; i < numImages; i++ ){
        for( k = 0; k < 2; k++ ){

            Mat img = goodImages[i*2+k];
			Mat rimg, cimg;

            remap(img, rimg, rmap[k][0], rmap[k][1], CV_INTER_LINEAR);

            cvtColor(rimg, cimg, CV_GRAY2BGR);

			Mat canvasPart = canvas(Rect(w*k, 0, w, h));

            resize(cimg, canvasPart, canvasPart.size(), 0, 0, CV_INTER_AREA);

            Rect vroi(cvRound(validRoi[k].x*sf), cvRound(validRoi[k].y*sf),
                        cvRound(validRoi[k].width*sf), cvRound(validRoi[k].height*sf)); 
            rectangle(canvasPart, vroi, Scalar(0,0,255), 3, 8);
        }
        
        for( j = 0; j < canvas.rows; j += 16 )
            line(canvas, Point(0, j), Point(canvas.cols, j), Scalar(0, 150, 0), 1, 8);

		imshow(Settings::RECTIFIED, canvas);

        char c = (char)waitKey(5000);
        if( c == 'q' || c == 'Q' )
            break;
    }
	destroyWindow(Settings::RECTIFIED);
	return true;
}