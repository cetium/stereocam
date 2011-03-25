#ifndef SKINDETECTIONPROCESSING_HPP
#define SKINDETECTIONPROCESSING_HPP

#include <cv.h>
#include "processing.hpp"


class SkinDetectionProcessing: public Processing{

public:
	SkinDetectionProcessing();
	virtual ~SkinDetectionProcessing();

	IplImage * processImage(IplImage * img);
};


#endif