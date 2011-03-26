#ifndef CALIBRATIONMODULE_HPP
#define CALIBRATIONMODULE_HPP

#include <cv.h>
#include <vector>

#include <QDialog>
#include <QObject>

#include "settings.hpp"
#include "cameracalibration.hpp"

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>


class CalibrationModule: public QObject{

	Q_OBJECT

public:
	CalibrationModule();
	~CalibrationModule();

	void init(Settings * sets, QDialog * dial, cv::Size theBoardSize, int pairs = 15);
	void addImages(IplImage * frame1, IplImage * frame2);

private slots:
	void startCalibration();
	void loadCalibration();

private:
	bool calibrate();
	void clearCalibrationData();
	void initWindow();

	QDialog * dialog;
	QPushButton * calibrateButton;
	QPushButton * loadCalibrationButton;

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