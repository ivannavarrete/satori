
#include <QCoreApplication>
#include <QTimer>
#include "ui/txt/basetxtui.h"


int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);

	BaseTxtUI base_ui;
	QTimer::singleShot(0, &base_ui, SLOT(Idle()));

	return app.exec();
}
