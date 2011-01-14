#include <iostream>
#include <string>

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "cameracalibration.hpp"

using namespace std;
using namespace cv;

CameraCalibration::CameraCalibration(){
	counter = 0;
	initialized = false;
}


CameraCalibration::~CameraCalibration()
{
}


bool CameraCalibration::calibrate(vector<Mat> & camImages, Size boardSize, float squareSize, bool displayCorners, bool showRectified){

	if(camImages.size() % 2 != 0){
		cout << "Ilosc zdjec do kalibracji musi byc taka sama z obydwu kamer" << endl;
		return false;
	}

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

			//ostringstream ss;
			//ss << (k == 0) ? "left0" : "right0";
			//ss << i;
			//ss << ".jpg";
			//string str = ss.str();

			//imwrite(str, img);

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

			if(displayCorners){
				Mat cimg;
				cvtColor(img, cimg, CV_GRAY2BGR);
				drawChessboardCorners(cimg, boardSize, corners, found);
				imshow("corners", cimg);
				char c = (char)waitKey(1000);

				// wyjscie z programu, aby bylo mozliwe
				if(c == 'q' || c == 'Q')
					exit(-1);
			}else
				putchar('.');
		
			if(!found){
				cout << "Nie odnaleziono dla i: " << i << " k: " << k << endl;
				break;
			}else{
				cout << "Odnaleziono dla i: " << i << " k: " << k << endl;
			}
			

			// jesli jest to drugi obrazek z pary stereo - wiec obydwa przeszly poprawnie 
			if(k==1){
				goodImages.push_back(camImages[i*2]);
				goodImages.push_back(camImages[i*2+1]);
				j++;
			}
		}
	}

	destroyWindow("corners");

	cout << "Na " << j << " par udalo sie znalezc chessboard" << endl;
	numImages = j;
	if(numImages < 2){
		cout << "Error: za malo obrazow do przetworzenia" << endl;
		return false;
	}

	imagePoints[0].resize(numImages);
	imagePoints[1].resize(numImages);
	objectPoints.resize(numImages);

	for(i = 0; i < numImages; ++i){
		for(j = 0; j < boardSize.height; ++j)
			for(k = 0; k < boardSize.width; ++k)
				objectPoints[i].push_back(Point3f(j*squareSize, k*squareSize, 0));
	}

	cout << "Stereo kalibracja..."<<endl;

	system("pause");

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
	cout << "kalibracja zakonczona z rms error: " << rms << endl;

	putchar('.');

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
		putchar('.');
    }
    //cout << "reprojection err = " <<  err/npoints << endl;

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
    else
        cout << "Error: can not save the intrinsic parameters\n" << endl;


	//Mat R1, R2, P1, P2, Q;
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
    else
        cout << "Error: can not save the intrinsic parameters" << endl;


	bool isVerticalStereo = fabs(P2.at<double>(1, 3)) > fabs(P2.at<double>(0, 3));
	if(isVerticalStereo){
		cout << "Error: odczytalo jako pionowe ulozenie stereo" << endl;
		return false;
	}


	if( !showRectified )
        return true;

	//Precompute maps for cv::remap()
	//Mat rmap[2][2];
    initUndistortRectifyMap(cameraMatrix[0], distCoeffs[0], R1, P1, imageSize, CV_16SC2, rmap[0][0], rmap[0][1]);
    initUndistortRectifyMap(cameraMatrix[1], distCoeffs[1], R2, P2, imageSize, CV_16SC2, rmap[1][0], rmap[1][1]);
    
    Mat canvas;
    double sf;
    int w, h;

	putchar('.');

    sf = 1.0;//340./MAX(imageSize.width, imageSize.height);
    w = cvRound(imageSize.width*sf);
    h = cvRound(imageSize.height*sf);
    canvas.create(h, w*2, CV_8UC3);

    
    for( i = 0; i < numImages; i++ ){
        for( k = 0; k < 2; k++ ){

            Mat img = goodImages[i*2+k];
			Mat rimg, cimg;

			//rimg = img;
            remap(img, rimg, rmap[k][0], rmap[k][1], CV_INTER_LINEAR);

            cvtColor(rimg, cimg, CV_GRAY2BGR);

			//imshow("rect", cimg);
			//char c = (char)waitKey(5000);

			Mat canvasPart = canvas(Rect(w*k, 0, w, h));

            resize(cimg, canvasPart, canvasPart.size(), 0, 0, CV_INTER_AREA);

            Rect vroi(cvRound(validRoi[k].x*sf), cvRound(validRoi[k].y*sf),
                        cvRound(validRoi[k].width*sf), cvRound(validRoi[k].height*sf)); 
            rectangle(canvasPart, vroi, Scalar(0,0,255), 3, 8);
        }
        

        for( j = 0; j < canvas.rows; j += 16 )
            line(canvas, Point(0, j), Point(canvas.cols, j), Scalar(0, 150, 0), 1, 8);

        imshow("rectified", canvas);

        char c = (char)waitKey(5000);
        if( c == 'q' || c == 'Q' )
            break;
    }
	destroyWindow("rectified");
	return true;
}


void CameraCalibration::initDisparityImage(Size s){
	
	stereoRectify( M1, D1, M2, D2, s, R, T, R1, R2, P1, P2, Q, -1, s, &roi1, &roi2 );

	initUndistortRectifyMap(M1, D1, R1, P1, s, CV_16SC2, rmap[0][0], rmap[0][1]);
    initUndistortRectifyMap(M2, D2, R2, P2, s, CV_16SC2, rmap[1][0], rmap[1][1]);

	bm.state->roi1 = roi1;
    bm.state->roi2 = roi2;
	bm.state->preFilterSize = 21;
    bm.state->preFilterCap = 31;
    bm.state->SADWindowSize = 11;
    bm.state->minDisparity = 0;
	bm.state->numberOfDisparities = 32;
    bm.state->textureThreshold = 1;
    bm.state->uniquenessRatio = 5;
    bm.state->speckleWindowSize = 200; // bylo 100
    bm.state->speckleRange = 32;
    bm.state->disp12MaxDiff = 2;
	
	
	sgbm.preFilterCap = 16;
    sgbm.SADWindowSize = 3;
    sgbm.P1 = 8*sgbm.SADWindowSize*sgbm.SADWindowSize;
    sgbm.P2 = 32*sgbm.SADWindowSize*sgbm.SADWindowSize;
    sgbm.minDisparity = 0;
    sgbm.numberOfDisparities = 32;
    sgbm.uniquenessRatio = 5;
    sgbm.speckleWindowSize = 200;
    sgbm.speckleRange = 32;
    sgbm.disp12MaxDiff = 2;
    sgbm.fullDP = false;
	
	initialized = true;
}


void CameraCalibration::showDisparityImage(Mat & img1, Mat & img2, int type, bool showCloud){
	
	Mat disp, disp8;
	Mat img1r, img2r;
	remap(img1, img1r, rmap[0][0], rmap[0][1], INTER_LINEAR);
	remap(img2, img2r, rmap[1][0], rmap[1][1], INTER_LINEAR);

	if(type == 0){
		//imshow("im1", img1r);
		//imshow("im2", img2r);

		bm(img1r, img2r, disp);
		disp.convertTo(disp8, CV_8U, 255.0/(bm.state->numberOfDisparities*16.));
		imshow("disparity", disp8);
	}
	else if(type == 1){

		sgbm(img1, img2, disp);
		disp.convertTo(disp8, CV_8U, 255/(sgbm.numberOfDisparities*16.));
		imshow("disparity", disp8);
	}

	if(showCloud){
		fflush(stdout);
        Mat xyz;
        reprojectImageTo3D(disp, xyz, Q, true);
		saveXYZ("cloud", xyz);
		if(XYZ.empty())		
			XYZ.release();
		XYZ = xyz.clone();
	}
}


bool CameraCalibration::hasCalibrationData(){
	FileStorage fs("intrinsics.yml", CV_STORAGE_READ);
	FileStorage fs2("extrinsics.yml", CV_STORAGE_READ);

	if(!fs.isOpened() || !fs2.isOpened()){
		return false;
	}

	fs["M1"] >> M1;
	fs["M2"] >> M2;
	fs["D1"] >> D1;
	fs["D2"] >> D2;

	fs2["R"] >> R;
	fs2["T"] >> T;
	fs2["R1"] >> R1;
	fs2["R2"] >> R2;
	fs2["P1"] >> P1;
	fs2["P2"] >> P2;
	fs2["Q"] >> Q;

	fs.release();
	fs2.release();

	return true;
}


void CameraCalibration::getIntrinsics(Mat & m1, Mat & m2, Mat & d1, Mat & d2){
	M1.copyTo(m1);
	M2.copyTo(m2);
	D1.copyTo(d1);
	D2.copyTo(d2);
}


void CameraCalibration::getExtrinsics(Mat & r, Mat & t, Mat & r1, Mat & r2, Mat & p1, Mat & p2, Mat & q){
	R.copyTo(r);
	T.copyTo(t);
	R1.copyTo(r1);
	R2.copyTo(r2);
	P1.copyTo(p1);
	P2.copyTo(p2);
	Q.copyTo(q);
}


void CameraCalibration::saveXYZ(const char* filename, const Mat& mat){
    const double max_z = 1.0e4;
	char nr[4];
	FILE* fp = fopen(string(filename).append(string(itoa(counter, nr, 10))).c_str(), "wt");

    for(int y = 0; y < mat.rows; y++){
        for(int x = 0; x < mat.cols; x++){

            Vec3f point = mat.at<Vec3f>(y, x);
            if(fabs(point[2] - max_z) < FLT_EPSILON || fabs(point[2]) > max_z) continue;
            fprintf(fp, "%f %f %f\n", point[0], point[1], point[2]);
        }
    }

	cout << "xyz saved" << endl;
    fclose(fp);
	counter++;
}