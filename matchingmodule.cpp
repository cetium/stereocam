#include "matchingmodule.hpp"

#include <opencv2/highgui/highgui.hpp>

#include <QVBoxLayout>
#include <QRadioButton>


using namespace std;
using namespace cv;

MatchingModule::MatchingModule(QObject *parent)
	: QObject(parent){

	chosenAlgorithm = -1;
}

MatchingModule::~MatchingModule(){

	for(int i = 0; i < algorithms.size(); ++i){
		delete algorithms[i];
	}
	delete buttonGroup;
}


void MatchingModule::init(Settings * sets, QDialog * dialog){
	settings = sets;

	chooseDialog = dialog;

	initAlgorithms();
	initChooseWindow();
	initWindows();
}


void MatchingModule::initAlgorithms(){
	
	StereoMatching * bm = new StereoMatchingBM();
	bm->id = 0;
	bm->init();

	StereoMatching * sgbm = new StereoMatchingSGBM();
	sgbm->id = 1;
	sgbm->init();

	StereoMatching * gc = new StereoMatchingGC();
	gc->id = 2;
	gc->init();

	algorithms.push_back(bm);
	algorithms.push_back(sgbm);
	algorithms.push_back(gc);

	chosenAlgorithm = 0;
}

void MatchingModule::initChooseWindow(){

	QVBoxLayout *layout = new QVBoxLayout;
	buttonGroup = new QButtonGroup;

	for(int i = 0; i < algorithms.size(); ++i){
		QRadioButton * button = new QRadioButton(QString(algorithms[i]->getName().c_str()), chooseDialog);
		layout->addWidget(button);
		buttonGroup->addButton(button);

		if(chosenAlgorithm == i){
			button->setChecked(true);
		}
		buttonGroup->setId(button, i);
	}

	connect (	buttonGroup,	SIGNAL(buttonClicked (int)),
				this,			SLOT(algorithmChanged (int)));


	chooseDialog->setLayout(layout);
	chooseDialog->adjustSize();

	chooseDialog->move(10,50);
}


void MatchingModule::initWindows(){
	
	


}


IplImage * MatchingModule::match(IplImage * frame1, IplImage * frame2){
	Mat retImage;
	algorithms[chosenAlgorithm]->exec(Mat(frame1), Mat(frame2), retImage);
	return NULL;
}

/*********** obsluga zdarzen *************/
void MatchingModule::algorithmChanged(int id){
	chosenAlgorithm = id;
	showSettingsWindow();
}

void MatchingModule::showSettingsWindow(){
	
}