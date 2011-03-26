#ifndef IMAGEDIALOG_HPP
#define IMAGEDIALOG_HPP

#include <cv.h>
#include <opencv2/core/core.hpp>

#include <QDialog>
#include <QCloseEvent>
#include <QLabel>

class ImageDialog: public QDialog{

public:
	ImageDialog(QWidget * parent, char * title, cv::Size size, cv::Point pointXY);
	~ImageDialog();

	void showImage(IplImage * newImage);
	void showImage(cv::Mat & image);

protected:
	virtual void closeEvent (QCloseEvent * e);

private:
	IplImage * lastFrame;
	QLabel * frameDisplayer;
};

#endif