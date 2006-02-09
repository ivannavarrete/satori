
#include "avrqtui.h"


AvrQtUi::AvrQtUi() {
}


AvrQtUi::~AvrQtUi() {

}


/**
 *
 */
void AvrQtUi::setupUi(QMainWindow *mainwindow) {
	this->mainwindow = mainwindow;
	menubar = mainwindow->menuBar();
	statusbar = mainwindow->statusBar();

	CreateActions();
	CreateMenus();
}


/**
 *
 */
void AvrQtUi::CreateActions() {
	// Debug menu
	action_run = new QAction(tr("&Run"), mainwindow);
	action_run->setShortcut(QKeySequence(tr("F8", "Debug|Run")));
	connect(action_run, SIGNAL(triggered()), this, SLOT(TestRun()));

	action_step = new QAction(tr("&Step"), mainwindow);
	action_step->setShortcut(QKeySequence(tr("F9", "Debug|Step")));
	connect(action_step, SIGNAL(triggered()), this, SLOT(TestStep()));

	action_next = new QAction(tr("&Next"), mainwindow);
	action_next->setShortcut(QKeySequence(tr("F10", "Debug|Next")));
	connect(action_next, SIGNAL(triggered()), this, SLOT(TestNext()));

	action_toggle_bp = new QAction(tr("Breakpoint"), mainwindow);
	action_toggle_bp->setShortcut(QKeySequence(tr("F11", "Debug|Breakpoint")));
	connect(action_toggle_bp, SIGNAL(triggered()), this, SLOT(TestBp()));

	// Avr menu
	action_device = new QAction(tr("&Device"), mainwindow);
	connect(action_device, SIGNAL(triggered()), this, SLOT(TestDevice()));

	action_info = new QAction(tr("&Info"), mainwindow);
	connect(action_info, SIGNAL(triggered()), this, SLOT(TestInfo()));
}


/**
 *
 */
void AvrQtUi::CreateMenus() {
	menu_debug = new QMenu(tr("&Debug"), menubar);
	menu_debug->addAction(action_run);
	menu_debug->addAction(action_step);
	menu_debug->addAction(action_next);
	menu_debug->addSeparator();
	menu_debug->addAction(action_toggle_bp);

	menu_avr = new QMenu(tr("&Avr"), menubar);
	menu_avr->addAction(action_device);
	menu_avr->addAction(action_info);

	// insert menu before the Help menu, which should be the last in the menubar
	QAction *help_menu_action;
	foreach (help_menu_action, menubar->actions()) {
		if (help_menu_action->text() == "&Help") {
			menubar->insertMenu(help_menu_action, menu_debug);
			menubar->insertMenu(help_menu_action, menu_avr);
		}
	}

}


void AvrQtUi::TestRun() {
	statusbar->showMessage(tr("Run"), 3000);
}


void AvrQtUi::TestStep() {
	statusbar->showMessage(tr("Step"), 3000);
}


void AvrQtUi::TestNext() {
	statusbar->showMessage(tr("Next"), 3000);
}


void AvrQtUi::TestBp() {
	statusbar->showMessage(tr("Bp"), 3000);
}


void AvrQtUi::TestDevice() {
	statusbar->showMessage(tr("Device"), 3000);
}


void AvrQtUi::TestInfo() {
	statusbar->showMessage(tr("Info"), 3000);
}


Q_EXPORT_PLUGIN(AvrQtUi)
