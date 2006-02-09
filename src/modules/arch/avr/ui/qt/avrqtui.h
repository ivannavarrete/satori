
#ifndef AVRQTUI_H
#define AVRQTUI_H


#include <QtGui>
#include "satori/ui/qt/qtui.h"


class AvrQtUi : public QObject, public QtUi {
	Q_OBJECT
	Q_INTERFACES(QtUi)

public:
	AvrQtUi();
	~AvrQtUi();

	// QtUi interface
	virtual void setupUi(QMainWindow *mainwindow);

private slots:
	void TestRun();
	void TestStep();
	void TestNext();
	void TestBp();
	void TestDevice();
	void TestInfo();

private:
	void CreateActions();
	void CreateMenus();

	QMainWindow *mainwindow;
	QMenuBar *menubar;
	QStatusBar *statusbar;
	
	QMenu *menu_debug;
	QMenu *menu_avr;

	QAction *action_run;
	QAction *action_step;
	QAction *action_next;
	QAction *action_toggle_bp;
	QAction *action_device;
	QAction *action_info;
};


#endif
