#include "imagedialog.hpp"
#include "imageutils.hpp"

#include <QVBoxLayout>
#include <QPixmap>

using namespace cv;

ImageDialog::ImageDialog(QWidget * parent, char * title, Size size, Point pointXY):
QDialog(parent), lastFrame(NULL){
	
	QVBoxLayout * layout = new QVBoxLayout(this);
	
	frameDisplayer = new QLabel(this);
	layout->addWidget(frameDisplayer);

	setLayout(layout);
	setWindowTitle(title);
	move(pointXY.x, pointXY.y);
	setFixedSize(size.width, size.height);
	show();
}

ImageDialog::~ImageDialog(){
	
}

void ImageDialog::showImage(IplImage * frame){

	IplImage * toDisplay = cvCreateImage(cvGetSize(frame), 8, 4);

	frameDisplayer->setPixmap
		(QPixmap::fromImage(ImageUtils::iplImageToQImage(frame, toDisplay), 0));
	update();

	cvReleaseImage(&frame);

	if(lastFrame != NULL){
		cvReleaseImage(&lastFrame);
	}
	lastFrame = toDisplay;
}

void ImageDialog::showImage(Mat & image){
	IplImage * ipl = new IplImage(image);
	showImage(ipl);
}


void ImageDialog::closeEvent (QCloseEvent * e){
	e->ignore();
}