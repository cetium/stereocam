#ifndef MATCHINGMODULE_HPP
#define MATCHINGMODULE_HPP

#include <QDialog>
#include <QButtonGroup>
#include <QObject>
#include <QAbstractButton>
#include <QLayout>
#include <QPushButton>

#include <cv.h>
#include <vector>

#include "stereomatching.hpp"
#include "stereomatchingbm.hpp"
#include "stereomatchingsgbm.hpp"
#include "stereomatchinggc.hpp"

#include "settings.hpp"
#include "property.hpp"


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

	void addGuiForProperty(QDialog * parent, QLayout * layout, Property * prop, QPushButton * button);

	std::vector<StereoMatching *> algorithms;
	std::vector<QDialog *> setsDialogs;
	std::vector<QPushButton *> applyButtons;

	QDialog * chooseDialog;
	QButtonGroup * buttonGroup;

	Settings * settings;
	int chosenAlgorithm;

private slots:
	void algorithmChanged(int id);
	void applyValues();
	void enableButton(int);
};

#endif // MATCHINGMODULE_HPP
