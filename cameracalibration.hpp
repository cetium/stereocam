#pragma once

#include <vector>
#include <cv.h>
#include <cxcore.h>

#include "cameradevice.hpp"


class CameraCalibration
{
public:
	CameraCalibration();
	~CameraCalibration();

	bool calibrate(std::vector<cv::Mat> & camImages, cv::Size boardSize, float squareSize, bool displayCorners = true, bool showRectified = true);
	bool hasCalibrationData();
	void getIntrinsics(cv::Mat & m1, cv::Mat & m2, cv::Mat & d1, cv::Mat & d2);
	void getExtrinsics(cv::Mat & r, cv::Mat & t, cv::Mat & r1, cv::Mat & r2, cv::Mat & p1, cv::Mat & p2, cv::Mat & q);
	
	void showDisparityImage(cv::Mat & img1, cv::Mat & img2, int type = 0, bool showCloud = false);
	void initDisparityImage(cv::Size s);

	cv::Mat & getPointCloud(){	return XYZ; }

	bool isInitialized(){	return initialized; }

private:
	void saveXYZ(const char* filename, const cv::Mat& mat);

	cv::Mat M1, M2, D1, D2;
	cv::Mat R, T, R1, R2, P1, P2, Q;

	cv::Mat rmap[2][2];
	cv::StereoBM bm;
	cv::StereoSGBM sgbm;

	cv::Rect roi1, roi2;

	cv::Mat XYZ;

	int counter;

	bool initialized;
};

