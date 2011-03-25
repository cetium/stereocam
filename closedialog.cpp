#include "closedialog.hpp"


CloseDialog::CloseDialog(): QDialog(0){
	end = false;
}


void CloseDialog::setMainThread(QThread * main){
	theThread = main;
}


void CloseDialog::closeEvent (QCloseEvent * event){
	theThread->deleteLater();
	event->accept();
}