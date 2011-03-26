#include <QtGui/QApplication>
#include <QTextCodec>

#include "mainthread.hpp"
#include "closedialog.hpp"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	CloseDialog dial;

	MainThread mainThread;
	mainThread.init(&dial);
	mainThread.start();

	return dial.exec();
}