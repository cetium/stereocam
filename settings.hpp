#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <stdio.h>
#include <string>
#include <cv.h>
#include <opencv2/highgui/highgui.hpp>


#define TEST 0
#define TESTGUI 1


class Settings{

public:
	
	Settings();
	~Settings();

	bool settingsChanged();

	int state;

	static const int BEFORE_CALIBRATION;
	static const int CALIBRATION;
	static const int AFTER_CALIBRATION;
	static const int MATCH;

	void showOverlay(std::string & text, std::string & window = WIN1);

	static cv::Size CAM_SIZE;
	static std::string WIN1;
	static std::string WIN2;
	static std::string RECTIFIED;
	static std::string CORNERS;
	static std::string ALGORITHMS;

private:
	bool changed;
	
};


#endif