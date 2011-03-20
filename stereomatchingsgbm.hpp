#pragma once

#include <cv.h>
#include <cxcore.h>

#include "stereomatching.hpp"
#include "property.hpp"

class StereoMatchingSGBM: StereoMatching{

public:

	cv::StereoSGBM sgbm;

	StereoMatchingSGBM();
	~StereoMatchingSGBM();
	
	virtual void init();
	virtual void exec(cv::Mat & image1, cv::Mat & image2, cv::Mat & retImage);
	virtual void applyAllParametres();


	const int I_minDisparity;
	const int I_numberOfDisparities;
	const int I_SADWindowSize;
	const int I_preFilterCap;
	const int I_uniquenessRatio;
	const int I_speckleWindowSize;
	const int I_speckleRange;
	const int I_disp12MaxDiff;
	const int I_fullDP;


private:

	
};