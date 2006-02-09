
#include <QApplication>
#include <QSplashScreen>
#include <QStyle>

#include "qxstyle.h"
#include "qxmainwindow.h"


int ExecQtUI(int &argc, char *argv[]) {
	QApplication app(argc, argv);
	//QApplication::setStyle(new QxStyle);
	QApplication::setFont(QFont("courier", 12));

	// create splash screen
	QSplashScreen *splash = new QSplashScreen(QPixmap(":/splash.png"),
											Qt::WindowStaysOnTopHint);
	splash->setFont(QFont("Helvetica", 22, QFont::Bold));
	splash->show();

	// init some stuff
	splash->showMessage("Checking modules", Qt::AlignBottom|Qt::AlignHCenter);
	QApplication::processEvents();

	splash->showMessage("Loading GUI", Qt::AlignBottom|Qt::AlignCenter);
	QApplication::processEvents();

	// create mainwindow
	QxMainWindow *mainwindow = new QxMainWindow;
	mainwindow->show();
	
	splash->finish(mainwindow);
	delete splash;

	return app.exec();
}
