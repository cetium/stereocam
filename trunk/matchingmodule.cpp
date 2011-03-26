#include "matchingmodule.hpp"
#include "property.hpp"

#include <opencv2/highgui/highgui.hpp>

#include <QVBoxLayout>
#include <QRadioButton>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>

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

	chooseDialog = new QDialog(dialog);
	chooseDialog->setWindowTitle("Stereo dopasowanie");

	initAlgorithms();
	initChooseWindow();
	initWindows();
}


void MatchingModule::initAlgorithms(){
	
	StereoMatching * bm = new StereoMatchingBM();
	bm->id = 0;

	StereoMatching * sgbm = new StereoMatchingSGBM();
	sgbm->id = 1;

	StereoMatching * gc = new StereoMatchingGC();
	gc->id = 2;

	algorithms.push_back(bm);
	algorithms.push_back(sgbm);
	algorithms.push_back(gc);

	chosenAlgorithm = 0;
}

void MatchingModule::initChooseWindow(){

	QVBoxLayout *layout = new QVBoxLayout;
	buttonGroup = new QButtonGroup;

	for(int i = 0; i < algorithms.size(); ++i){
		QRadioButton * button = new QRadioButton(QString(algorithms[i]->getName()), chooseDialog);
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
	chooseDialog->setMinimumWidth(200);
	chooseDialog->adjustSize();

	chooseDialog->move(10,20);
}


void MatchingModule::initWindows(){
	
	for(int i = 0; i < algorithms.size(); ++i){
		QDialog * setsDialog = new QDialog(chooseDialog);
		QVBoxLayout *layout = new QVBoxLayout;

		QPushButton * buttonApply = new QPushButton("Zastosuj", chooseDialog);
		connect(buttonApply,	SIGNAL(clicked()),
				this,			SLOT(applyValues()));

		int propsCounter = algorithms[i]->properties.size();
		for(int j = 0; j < propsCounter; ++j){
			Property * prop = algorithms[i]->properties[j];
			addGuiForProperty(setsDialog, layout, prop, buttonApply);
		}
		buttonApply->setEnabled(false);
		layout->addWidget(buttonApply);
		applyButtons.push_back(buttonApply);

		setsDialog->setWindowTitle(algorithms[i]->getName());
		setsDialog->setLayout(layout);
		setsDialog->setMinimumWidth(200);
		setsDialog->adjustSize();
		setsDialog->move(10,150);
		setsDialogs.push_back(setsDialog);
	}
}

void MatchingModule::addGuiForProperty(QDialog * parent, QLayout * layout, Property * prop, QPushButton * button){
	Property::TYPE theType = prop->getType();

	switch(theType){
		case Property::INT:{
			
			QLabel * label = new QLabel(prop->getName(), parent);
			layout->addWidget(label);
			
			QSpinBox * spin = new QSpinBox(parent);
			spin->setRange(prop->minValue, prop->maxValue);
			spin->setValue(prop->getValue());
			spin->setSingleStep(prop->step);
			layout->addWidget(spin);

			connect(spin,		SIGNAL(valueChanged(int)),
					prop,		SLOT(setValueToChange(int)));

			connect(spin,		SIGNAL(valueChanged(int)),
					this,		SLOT(enableButton(int)));

			break;				   
		}
		case Property::BOOL:{
			QCheckBox * checkBox = new QCheckBox(prop->getName(), parent);
			checkBox->setChecked((bool)prop->getValue());
			layout->addWidget(checkBox);

			connect(checkBox,	SIGNAL(stateChanged(int)),
					prop,		SLOT(setValueToChange(int)));

			connect(checkBox,	SIGNAL(stateChanged(int)),
					this,		SLOT(enableButton(int)));
			
			break;	
		}
		case Property::DISCRETE:{
			
			QLabel * label = new QLabel(prop->getName(), parent);
			layout->addWidget(label);

			QComboBox * combo = new QComboBox(parent);
			int * values = prop->tableValues;
			for(int i = 0; i < prop->maxValue; ++i){
				combo->addItem(QString().setNum(values[i]));
			}
			combo->setCurrentIndex(prop->getValue());
			
			connect(combo,	SIGNAL(currentIndexChanged(int)),
					prop,	SLOT(setValueToChange(int)));

			connect(combo,		SIGNAL(currentIndexChanged(int)),
					this,		SLOT(enableButton(int)));

			layout->addWidget(combo);
			break;
		}
	}
}


IplImage * MatchingModule::match(IplImage * frame1, IplImage * frame2){
	Mat retImage;
	algorithms[chosenAlgorithm]->exec(Mat(frame1), Mat(frame2), retImage);
	return NULL;
}

/*********** obsluga zdarzen *************/
void MatchingModule::algorithmChanged(int id){
	if(id == chosenAlgorithm){
		return;
	}
	setsDialogs[chosenAlgorithm]->hide();
	chosenAlgorithm = id;
	setsDialogs[chosenAlgorithm]->show();
}



void MatchingModule::applyValues(){
	algorithms[chosenAlgorithm]->setPropertiesWereChanged();

	Mat a, b, c;
	algorithms[chosenAlgorithm]->exec(a,b,c);

	applyButtons[chosenAlgorithm]->setEnabled(false);
}


void MatchingModule::enableButton(int){
	applyButtons[chosenAlgorithm]->setEnabled(true);
}