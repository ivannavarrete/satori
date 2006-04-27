
#include <QCoreApplication>
#include <QTimer>
#include "basetxtui.h"
#include "color.h"


int ExecTxtUI(int &argc, char *argv[]) {
	QCoreApplication app(argc, argv);

	Color::Init256();

	BaseTxtUi base_ui;
	QTimer::singleShot(0, &base_ui, SLOT(Idle()));

	return app.exec();
}
