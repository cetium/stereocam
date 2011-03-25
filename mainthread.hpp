#ifndef MAINTHREAD_HPP
#define MAINTHREAD_HPP

#include <QThread>
#include <QDialog>
#include <QObject>

#include <stdio.h>
#include <iostream>
#include <string>

#include <cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "cameradevice.hpp"
#include "matchingmodule.hpp"
#include "calibrationmodule.hpp"
#include "processingmodule.hpp"
#include "closedialog.hpp"


class MainThread: public QThread{

	Q_OBJECT

public:
	MainThread();
	~MainThread();

	bool init(CloseDialog * dialog);

protected:
	virtual void run();

private:
	
	bool initCameras();
	bool initModules();
	bool initDialogs();

	bool opencvMain();

	CloseDialog * mainDialog;
	QDialog * images;

	// kamery
	CameraDevice * camLeft;
	CameraDevice * camRight;

	// moduly
	MatchingModule			stereoModule;
	CalibrationModule		calibModule;
	ProcessingModule		processModule;

	// ustawienia
	Settings settings;

};



#endif