#ifndef CALIBRATIONMODULE_HPP
#define CALIBRATIONMODULE_HPP

#include <cv.h>
#include <vector>

#include "settings.hpp"
#include "cameracalibration.hpp"

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


class CalibrationModule{

public:
	CalibrationModule();
	~CalibrationModule();

	void init(Settings * sets, cv::Size theBoardSize, int pairs = 15);
	void addImages(IplImage * frame1, IplImage * frame2);

private:
	bool calibrate();
	void clearCalibrationData();

	Settings * settings;
	int maxPairs;

	std::vector<cv::Mat> camImages;
	cv::Size boardSize;

	cv::Mat M1, M2, D1, D2;
	cv::Mat R, T, R1, R2, P1, P2, Q;

	cv::Mat rmap[2][2];

	cv::Mat blackOne;

	int counter;
	int samplesTime;
};


#endif