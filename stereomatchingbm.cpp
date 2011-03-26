#include "stereomatchingbm.hpp"


using namespace std;
using namespace cv;



StereoMatchingBM::StereoMatchingBM(): StereoMatching("Block Matching"),

I_minDisparity(0),
I_numberOfDisparities(1),
I_SADWindowSize(2),
I_preFilterCap(3),
I_preFilterSize(4),
I_textureThreshold(5),
I_uniquenessRatio(6),
I_speckleWindowSize(7),
I_speckleRange(8),
I_disp12MaxDiff(9),
I_trySmallerWindows(10){

	init();
}

StereoMatchingBM::~StereoMatchingBM(){
	
}

void StereoMatchingBM::init(){
	//0
	Property * prop = new Property("Minimum disparity", 0, Property::INT);
	prop->minValue = -50;
	prop->maxValue = 50;
	prop->step = 5;
	properties.push_back(prop);


	//1
	Property * prop2 = new Property("Number of disparities", 2, Property::DISCRETE);
	int max = 4;
	int * values = new int[max];
	for(int i = 0; i < max; ++i)
		values[i] = 16*i;

	prop2->minValue = 0;
	prop2->maxValue = max;
	prop2->tableValues = values;
	properties.push_back(prop2);


	//2
	Property * prop3 = new Property("SAD window size", 6, Property::DISCRETE);
	max = 8;
	int defValues[8] = {1, 3, 5, 7, 9, 11, 21, 31};

	int * values2 = new int[max];
	for(int i = 0; i < max; ++i)
		values2[i] = defValues[i];

	prop3->minValue = 0;
	prop3->maxValue = max;
	prop3->tableValues = values2;
	properties.push_back(prop3);

	
	//3
	prop = new Property("PrefilterCap", 31, Property::INT);
	prop->minValue = 0;
	prop->maxValue = 50;
	properties.push_back(prop);


	//4
	prop = new Property("Prefilter Size", 11, Property::INT);
	prop->minValue = 0;
	prop->maxValue = 50;
	properties.push_back(prop);


	//5
	prop = new Property("Texture Threshold", 5, Property::INT);
	prop->minValue = 0;
	prop->maxValue = 30;
	properties.push_back(prop);


	//6
	prop = new Property("UniquenessRatio", 1, Property::INT);
	prop->minValue = 0;
	prop->maxValue = 35;
	properties.push_back(prop);


	//7
	prop = new Property("Speckle window size", 400, Property::INT);
	prop->minValue = 0;
	prop->maxValue = 500;
	prop->step = 50;
	properties.push_back(prop);


	//8
	prop = new Property("Speckle range", 1, Property::DISCRETE);
	max = 4;
	int * values3 = new int[max];
	for(int i = 0; i < max; ++i)
		values3[i] = 16*i;

	prop->minValue = 0;
	prop->maxValue = max;
	prop->tableValues = values3;
	properties.push_back(prop);


	//9
	prop = new Property("Disparity max difference", 2, Property::INT);
	prop->minValue = -1;
	prop->maxValue = 20;
	properties.push_back(prop);


	//10
	prop = new Property("Try smaller windows", 0, Property::BOOL);
	prop->minValue = 0;
	prop->maxValue = 1;
	properties.push_back(prop);


	applyAllParametres();
}


void StereoMatchingBM::applyAllParametres(){
	changeProperties();

	bm.state->preFilterCap			= properties[I_preFilterCap]->getValue();
	bm.state->preFilterSize 		= properties[I_preFilterSize]->getValue();
	bm.state->SADWindowSize			= properties[I_SADWindowSize]->tableValues[properties[I_SADWindowSize]->getValue()];
    bm.state->minDisparity			= properties[I_minDisparity]->getValue();
	bm.state->numberOfDisparities	= properties[I_numberOfDisparities]->tableValues[properties[I_numberOfDisparities]->getValue()];
    bm.state->uniquenessRatio		= properties[I_uniquenessRatio]->getValue();
	bm.state->textureThreshold		= properties[I_textureThreshold]->getValue();
    bm.state->speckleWindowSize		= properties[I_speckleWindowSize]->getValue();
	bm.state->speckleRange			= properties[I_speckleRange]->tableValues[properties[I_speckleRange]->getValue()];
    bm.state->disp12MaxDiff			= properties[I_disp12MaxDiff]->getValue();
    bm.state->trySmallerWindows		= (bool)properties[I_trySmallerWindows]->getValue();
}



void StereoMatchingBM::exec(Mat & image1, Mat & image2, Mat & retImage){
	if(propertiesChanged){
		applyAllParametres();
		propertiesChanged = false;
	}

	Mat disp;

	bm(image1, image2, disp);
	disp.convertTo(retImage, CV_8U, 255/(bm.state->numberOfDisparities*16.));
}