#ifndef DONTCLOSEDIALOG_HPP
#define DONTCLOSEDIALOG_HPP


#include <QDialog>
#include <QCloseEvent>

#include <opencv2/core/core.hpp>

class DontCloseDialog: public QDialog{

public:
	DontCloseDialog(QWidget * parent, char * title, cv::Point pointXY);
	~DontCloseDialog();

protected:
	virtual void closeEvent (QCloseEvent * e);
};

#endif