#include "processingmodule.hpp"

#include <cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

#include <QFormLayout>
#include <QLabel>
#include <QCheckBox>


using namespace std;
using namespace cv;


ProcessingModule::ProcessingModule(){

}
ProcessingModule::~ProcessingModule(){
	
}

void ProcessingModule::init(Settings * sets, QDialog * mainDialog){
	settings = sets;
	initProcessingObjects();
	initWindow(mainDialog);
}


void ProcessingModule::initProcessingObjects(){

}


void ProcessingModule::initWindow(QDialog * mainDialog){
	
	dialog = new DontCloseDialog(mainDialog, "Przetwarzanie obrazow", Point(250, 520));
	dialog->setMinimumWidth(300);
	
	QFormLayout * layout = new QFormLayout(dialog);

	QLabel * label1 = new QLabel("Przed stereo dopasowaniem", dialog);
	layout->addRow(label1, new QLabel("", dialog));
	for(int i = 0; i < preMatchingProcess.size(); ++i){
		addGuiRowForProcessing(preMatchingProcess[i], layout);
	}

	QLabel * label2 = new QLabel("Po stereo dopasowaniu", dialog);
	layout->addRow(label2, new QLabel("", dialog));
	for(int i = 0; i < postMatchingProcess.size(); ++i){
		addGuiRowForProcessing(postMatchingProcess[i], layout);
	}

	// do zatwierdzenia
	applyButton = new QPushButton("Zastosuj", dialog);
	applyButton->setEnabled(false);

	connect(applyButton,	SIGNAL(clicked()),
			this,			SLOT(applyChanges()));

	layout->addRow(new QLabel("", dialog), applyButton);

	dialog->setLayout(layout);
	dialog->adjustSize();
}


void ProcessingModule::addGuiRowForProcessing(Processing * process, QFormLayout * layout){

}


void ProcessingModule::applyChanges(){
	
}


void ProcessingModule::enableButton(int){

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