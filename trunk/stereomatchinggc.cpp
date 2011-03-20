#include "stereomatchinggc.hpp"


using namespace std;
using namespace cv;



StereoMatchingGC::StereoMatchingGC(): StereoMatching("Graph Cut"),

I_minDisparity(0),
I_numberOfDisparities(1),
I_threshold(2),
I_maxIters(3),
I_interactionRadius(4){
	state = NULL;

	state = cvCreateStereoGCState(16, 2);

	init();
}

StereoMatchingGC::~StereoMatchingGC(){
	if(state)
		cvReleaseStereoGCState(&state);
}

void StereoMatchingGC::init(){
	//0
	Property * prop = new Property("Minimum disparity", 0, Property::INT);
	prop->minValue = -50;
	prop->maxValue = 50;
	properties.push_back(prop);


	//1
	prop = new Property("Number of disparities", 2, Property::DISCRETE);
	int max = 4;
	int * values = new int[max];
	for(int i = 0; i < max; ++i)
		values[i] = 16*i;

	prop->minValue = 0;
	prop->maxValue = max-1;
	prop->tableValues = values;
	properties.push_back(prop);


	//2
	prop = new Property("Threshold", 5, Property::INT);
	prop->minValue = 0;
	prop->maxValue = 30;
	properties.push_back(prop);

	
	//3
	prop = new Property("Max iterations", 2, Property::INT);
	prop->minValue = 1;
	prop->maxValue = 7;
	properties.push_back(prop);


	//4 - for smoothness
	prop = new Property("Iteration radius", 1, Property::INT);
	prop->minValue = 1;
	prop->maxValue = 10;
	properties.push_back(prop);


	applyAllParametres();
}


void StereoMatchingGC::applyAllParametres(){
	changeProperties();

    state->minDisparity			= properties[I_minDisparity]->getValue();
	state->numberOfDisparities	= properties[I_numberOfDisparities]->getValue();
	state->Ithreshold			= properties[I_threshold]->getValue();
	state->maxIters				= properties[I_maxIters]->getValue();
	state->interactionRadius	= properties[I_interactionRadius]->getValue();
}



void StereoMatchingGC::exec(Mat & image1, Mat & image2, Mat & retImage){
	
	IplImage img1 = image1;
	IplImage img2 = image2;
	IplImage* disp1 = cvCreateImage(image1.size(), image1.depth(), image1.channels());
	IplImage* disp2 = cvCreateImage(image1.size(), image1.depth(), image1.channels());

	cvFindStereoCorrespondenceGC(&img1, &img2, disp1, disp2, state);

	Mat disp1Mat = disp1;
	disp1Mat.convertTo(retImage, CV_8U, -16);
}