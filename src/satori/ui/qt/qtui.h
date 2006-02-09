
#ifndef QTUI_H
#define QTUI_H


#include <QPluginLoader>


class QtUi {
public:
	virtual ~QtUi() {}

	virtual void setupUi(QMainWindow *mainwindow) = 0;
};


Q_DECLARE_INTERFACE(QtUi, "com.angulum.satori.QtUi/1.0");


#endif
