#ifndef PROCESSING_HPP
#define PROCESSING_HPP

#include <cv.h>

class Processing{

public:

	Processing();
	virtual ~Processing();

	virtual IplImage * processImage(IplImage * img) = 0;
};


#endif