
#ifndef QXMAINWINDOW_H
#define QXMAINWINDOW_H


//#include <QCloseEvent>
#include "ui_mainwindow.h"


class QxMainWindow : public QMainWindow, private Ui::MainWindow {
	Q_OBJECT

public:
	QxMainWindow(QWidget *parent=0);

protected:
	void closeEvent(QCloseEvent *event);	// implement in plugins instead

private slots:
	void About();

private:
	void InitActions();
	void InitStatusBar();
	void Test();
};


#endif
