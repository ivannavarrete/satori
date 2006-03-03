
#include <QApplication>
#include <QLabel>
#include <QDir>
#include <QMessageBox>
#include <boost/shared_ptr.hpp>
#include "qxmainwindow.h"
#include "satori/ui/qt/qtui.h"


QxMainWindow::QxMainWindow(QWidget *parent) : QMainWindow(parent) {
	setupUi(this);

	InitActions();
	InitStatusBar();

	//setWindowIcon(QPixmap(":/window_icon.png"));

	Test();
}


void QxMainWindow::InitActions() {
	// file menu
	action_quit->setShortcut(QKeySequence(tr("Ctrl+Q", "File|Quit")));
	connect(action_quit, SIGNAL(triggered()), this, SLOT(close()));

	connect(action_refresh, SIGNAL(triggered()), this, SLOT(TestLoadArch()));
	
	// help menu
	connect(action_about_satori, SIGNAL(triggered()), this, SLOT(About()));
	connect(action_about_qt, SIGNAL(triggered()),
			QApplication::instance(), SLOT(aboutQt()));
}


void QxMainWindow::InitStatusBar() {
	statusBar()->addWidget(new QLabel("foo bar"));
	statusBar()->addWidget(new QLabel(), 1);
	statusBar()->addWidget(new QLabel(" hello World! "));
}


void QxMainWindow::About() {
	QMessageBox::about(this, tr("About Satori"),
					tr("<h2>Satori 0.2</h2>"
					   "<p>Copyright &copy; 2006 Angulum</p>"
					   "<p>Satori is a portable and extendible "
					   "microcontroller development system.</p>"));
}


void QxMainWindow::Test() {
	//uint32_t start_addr;
	//uint32_t end_addr;
	//boost::shared_ptr<Memory> memory;

	//start_addr = 0x60;
	//end_addr = 0x25F;
	//boost::shared_ptr<CommandEngine> command_engine(new CommandEngine);
	//memory = boost::shared_ptr<Memory>(new Memory(Memory::SRAM, start_addr,
	//											end_addr, command_engine));
	//QxMemory *sram = new QxMemory(memory, this);

	//setCentralWidget(sram);
}


void QxMainWindow::TestLoadArch() {
	QDir modules_dir(QApplication::applicationDirPath() + "/../modules/arch");
	if (!modules_dir.exists()) {
		QMessageBox::warning(this, tr("Module Load Error"),
				tr("Unable to find module directory."),
				QMessageBox::Ok | QMessageBox::Default,
				QMessageBox::NoButton);
		return;
	}

	QPluginLoader loader(modules_dir.absoluteFilePath("avr/libavrqt.so"));
	QObject *module = loader.instance();
	if (module) {
		QtUi *qt_ui = qobject_cast<QtUi *>(module);
		if (qt_ui) {
			statusbar->showMessage("Module loaded", 5000);
			qt_ui->setupUi(this);
		}
	}
}


void QxMainWindow::closeEvent(QCloseEvent * /*event*/) {
	// Sure you want to quit.. Save.. etc...
}
