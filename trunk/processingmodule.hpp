#ifndef PROCESSINGMODULE_HPP
#define PROCESSINGMODULE_HPP

#include <cv.h>
#include <vector>

#include <QObject>
#include <QDialog>
#include <QPushButton>
#include <QFormLayout>

#include "settings.hpp"
#include "processing.hpp"
#include "dontclosedialog.hpp"


class ProcessingModule: public QObject{

	Q_OBJECT

public:
	ProcessingModule();
	~ProcessingModule();

	void init(Settings * sets, QDialog * mainDialog);
	IplImage * process(IplImage * toProcess);

	IplImage * preMatchingProcessing(IplImage * toProcess);
	IplImage * postMatchingProcessing(IplImage * toProcess);

private:
	void initProcessingObjects();
	void initWindow(QDialog * mainDialog);
	void addGuiRowForProcessing(Processing * process, QFormLayout * layout);

	std::vector<Processing *> preMatchingProcess;
	std::vector<Processing *> postMatchingProcess;

	Settings * settings;

	DontCloseDialog * dialog;
	QPushButton * applyButton;

private slots:
	void applyChanges();
	void enableButton(int);
};


#endif