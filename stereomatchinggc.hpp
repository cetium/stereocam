#pragma once

#include <cv.h>
#include <cxcore.h>

#include "stereomatching.hpp"
#include "property.hpp"

class StereoMatchingGC: public StereoMatching{

public:

	CvStereoGCState* state;

	StereoMatchingGC();
	~StereoMatchingGC();
	
	virtual void init();
	virtual void exec(cv::Mat & image1, cv::Mat & image2, cv::Mat & retImage);
	virtual void applyAllParametres();


	const int I_minDisparity;
	const int I_numberOfDisparities;
	const int I_threshold;
	const int I_maxIters;
	const int I_interactionRadius;


private:

	
};