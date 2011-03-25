#pragma once

#include <cv.h>
#include <cxcore.h>

#include "stereomatching.hpp"
#include "property.hpp"

class StereoMatchingBM: public StereoMatching{

public:

	cv::StereoBM bm;

	StereoMatchingBM();
	~StereoMatchingBM();
	
	virtual void init();
	virtual void exec(cv::Mat & image1, cv::Mat & image2, cv::Mat & retImage);
	virtual void applyAllParametres();


	CvRect roi1, roi2;

	const int I_minDisparity;
	const int I_numberOfDisparities;
	const int I_SADWindowSize;
	const int I_preFilterCap;
	const int I_preFilterSize;
	const int I_textureThreshold;
	const int I_uniquenessRatio;
	const int I_speckleWindowSize;
	const int I_speckleRange;
	const int I_disp12MaxDiff;
	const int I_trySmallerWindows;


private:

	
};