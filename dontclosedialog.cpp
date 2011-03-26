#include "dontclosedialog.hpp"

using namespace cv;

DontCloseDialog::DontCloseDialog(QWidget * parent, char * title, Point pointXY):
QDialog(parent){
	
	setWindowTitle(title);
	move(pointXY.x, pointXY.y);
}

DontCloseDialog::~DontCloseDialog(){
	
}

void DontCloseDialog::closeEvent (QCloseEvent * e){
	e->ignore();
}