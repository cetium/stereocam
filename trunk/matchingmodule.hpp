#ifndef MATCHINGMODULE_HPP
#define MATCHINGMODULE_HPP

#include <QDialog>
#include <QButtonGroup>
#include <QObject>
#include <QAbstractButton>

#include <cv.h>
#include <vector>

#include "stereomatching.hpp"
#include "stereomatchingbm.hpp"
#include "stereomatchingsgbm.hpp"
#include "stereomatchinggc.hpp"

#include "settings.hpp"


class MatchingModule : public QObject
{
	Q_OBJECT

public:
	MatchingModule(QObject *parent = 0);
	~MatchingModule();

	void init(Settings * sets, QDialog * dialog);
	IplImage * match(IplImage * frame1, IplImage * frame2);

private:
	void initAlgorithms();
	void initWindows();
	void initChooseWindow();

	void showSettingsWindow();

	std::vector<StereoMatching *> algorithms;
	std::vector<QDialog *> setsDialogs;

	QDialog * chooseDialog;
	QButtonGroup * buttonGroup;

	Settings * settings;
	int chosenAlgorithm;

private slots:
	void algorithmChanged(int id);
	
};

#endif // MATCHINGMODULE_HPP
