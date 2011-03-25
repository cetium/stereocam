#ifndef CLOSEDIALOG_HPP
#define CLOSEDIALOG_HPP

#include <QObject>
#include <QDialog>
#include <QCloseEvent>
#include <QWidget>
#include <QThread>

class CloseDialog: public QDialog{

	Q_OBJECT

public:
	CloseDialog();
	void setMainThread(QThread * main);

protected:
	void closeEvent (QCloseEvent * event);

private:
	bool end;
	QThread * theThread;
};


#endif