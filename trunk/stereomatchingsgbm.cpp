#include "stereomatchingsgbm.hpp"


using namespace std;
using namespace cv;



StereoMatchingSGBM::StereoMatchingSGBM(): StereoMatching("Semi-Global Block Matching"),

I_minDisparity(0),
I_numberOfDisparities(1),
I_SADWindowSize(2),
I_preFilterCap(3),
I_uniquenessRatio(4),
I_speckleWindowSize(5),
I_speckleRange(6),
I_disp12MaxDiff(7),
I_fullDP(8){

	init();
}

StereoMatchingSGBM::~StereoMatchingSGBM(){
	
}

void StereoMatchingSGBM::init(){
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
	prop = new Property("SAD window size", 5, Property::DISCRETE);
	max = 8;
	int defValues[8] = {1, 3, 5, 7, 9, 11, 21, 31};

	int * values2 = new int[max];
	for(int i = 0; i < max; ++i)
		values2[i] = defValues[i];

	prop->minValue = 0;
	prop->maxValue = max-1;
	prop->tableValues = values2;
	properties.push_back(prop);

	
	//3
	prop = new Property("PrefilterCap", 31, Property::INT);
	prop->minValue = 0;
	prop->maxValue = 50;
	properties.push_back(prop);


	//4
	prop = new Property("UniquenessRatio", 1, Property::INT);
	prop->minValue = 0;
	prop->maxValue = 35;
	properties.push_back(prop);


	//5
	prop = new Property("Speckle window size", 400, Property::INT);
	prop->minValue = 0;
	prop->maxValue = 500;
	properties.push_back(prop);


	//6
	prop = new Property("Speckle range", 16, Property::DISCRETE);
	max = 4;
	int * values3 = new int[max];
	for(int i = 0; i < max; ++i)
		values3[i] = 16*i;

	prop->minValue = 0;
	prop->maxValue = max-1;
	prop->tableValues = values;
	properties.push_back(prop);


	//7
	prop = new Property("Disparity max difference", 2, Property::INT);
	prop->minValue = -1;
	prop->maxValue = 20;
	properties.push_back(prop);


	//8
	prop = new Property("Use full DP", 0, Property::BOOL);
	properties.push_back(prop);


	applyAllParametres();
}


void StereoMatchingSGBM::applyAllParametres(){
	changeProperties();

	sgbm.preFilterCap			= properties[I_preFilterCap]->getValue();
    sgbm.SADWindowSize			= properties[I_SADWindowSize]->getValue();
    sgbm.P1						= 16*sgbm.SADWindowSize*sgbm.SADWindowSize;
    sgbm.P2						= 64*sgbm.SADWindowSize*sgbm.SADWindowSize;
    sgbm.minDisparity			= properties[I_minDisparity]->getValue();
	sgbm.numberOfDisparities	= properties[I_numberOfDisparities]->getValue();
    sgbm.uniquenessRatio		= properties[I_uniquenessRatio]->getValue();
    sgbm.speckleWindowSize		= properties[I_speckleWindowSize]->getValue();
    sgbm.speckleRange			= properties[I_speckleRange]->getValue();
    sgbm.disp12MaxDiff			= properties[I_disp12MaxDiff]->getValue();
    sgbm.fullDP					= (bool)properties[I_fullDP]->getValue();
}



void StereoMatchingSGBM::exec(Mat & image1, Mat & image2, Mat & retImage){
	Mat disp;

	sgbm(image1, image2, disp);
	disp.convertTo(retImage, CV_8U, 255/(sgbm.numberOfDisparities*16.));
}