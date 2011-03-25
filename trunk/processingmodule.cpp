#include "processingmodule.hpp"

#include <cv.h>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;


ProcessingModule::ProcessingModule(){

}
ProcessingModule::~ProcessingModule(){
	destroyWindow(Settings::ALGORITHMS);
}

void ProcessingModule::init(Settings * sets){
	settings = sets;
}

void ProcessingModule::initWindow(){
	//namedWindow(Settings::MATCH, CV_WINDOW_AUTOSIZE | CV_GUI_EXPANDED);

	
}

IplImage * ProcessingModule::preMatchingProcessing(IplImage * toProcess){

	if(preMatchingProcess.empty())
		return NULL;

	IplImage * toRet = toProcess;

	for(int i = 0; i < preMatchingProcess.size(); ++i){
		toRet = preMatchingProcess[i]->processImage(toRet);
	}
	return toRet;
}

IplImage * ProcessingModule::postMatchingProcessing(IplImage * toProcess){
	
	if(postMatchingProcess.empty())
		return NULL;
	
	IplImage * toRet = toProcess;

	for(int i = 0; i < preMatchingProcess.size(); ++i){
		toRet = preMatchingProcess[i]->processImage(toRet);
	}
	return toRet;
}