////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file	imageutils.cpp
///
/// @brief	Implements the imageutils methods. 
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <cxcore.h>
#include <cv.h>
#include "imageutils.hpp"



QImage ImageUtils::iplImageToQImage(const IplImage *iplImage, IplImage * copy){
	 // the individual channels for the IplImage

	IplImage * tchannel0 = cvCreateImage(cvGetSize(iplImage), IPL_DEPTH_8U, 1);
	IplImage * tchannel1 = cvCreateImage(cvGetSize(iplImage), IPL_DEPTH_8U, 1);
	IplImage * tchannel2 = cvCreateImage(cvGetSize(iplImage), IPL_DEPTH_8U, 1);
	IplImage * tchannel3 = cvCreateImage(cvGetSize(iplImage), IPL_DEPTH_8U, 1);

	// set all elements in tchannel0 (alpha channel) to 255
	cvSet(tchannel0,cvScalarAll(255),0);

	if(iplImage->nChannels == 3){
		cvSplit(iplImage, tchannel1, tchannel2, tchannel3, NULL);
		cvMerge(tchannel1, tchannel2, tchannel3, tchannel0, copy);
	}else {// == 1
		cvMerge(iplImage, iplImage, iplImage, tchannel0, copy);
	}

	// point to the image data stored in the IplImage*
	const unsigned char * data = (unsigned char *)(copy->imageData);

	// read other parameters in local variables
	int width = copy->width;
	int height = copy->height;
	int bytesPerLine = copy->widthStep;

	cvReleaseImage(&tchannel0);
	cvReleaseImage(&tchannel1);
	cvReleaseImage(&tchannel2);
	cvReleaseImage(&tchannel3);

	// imageframe is my QLabel object
	return QImage( data, width, height, bytesPerLine, QImage::Format_RGB32 );
}
