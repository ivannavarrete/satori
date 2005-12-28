
#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>

#include <QPluginLoader>

#include <QStyle>
#include <QWindowsStyle>
#include <QPlastiqueStyle>
#include <QScrollBar>
#include <QHBoxLayout>

#include <QPushButton>
#include <QLabel>

#include "widgets/widgetgallery.h"
#include "widgets/qxmemory.h"
#include "../../memory.h"
#include "../../commandengine.h"
#include "../../modules/comm/commprovider.h"

#include "qxstyle.h"


/*
int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	app.setStyle(new QPlastiqueStyle);
	app.setStyle(new QxStyle);

	WidgetGallery widget_gallery;
	widget_gallery.show();

	return app.exec();
}
*/


int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	QApplication::setStyle(new QxStyle);
	QApplication::setFont(QFont("courier", 12));
	QApplication::setKeyboardInputInterval(200);

	QSplashScreen *splash = new QSplashScreen(QPixmap("splash.png"),
										Qt::WindowStaysOnTopHint);
	splash->setFont(QFont("Helvetica", 22, QFont::Bold));
	splash->show();

	// init some stuff
	splash->showMessage("Initializing GUI", Qt::AlignBottom|Qt::AlignHCenter);
	QApplication::processEvents();

	splash->showMessage("Checking modules", Qt::AlignBottom|Qt::AlignHCenter);
	QApplication::processEvents();

	splash->showMessage("Loading GUI", Qt::AlignBottom|Qt::AlignCenter);
	QApplication::processEvents();


	QPluginLoader loader("../../modules/comm/serial/libserial.so");
	QObject *module = loader.instance();
	CommProvider *comm_provider = qobject_cast<CommProvider *>(module);
	
	boost::shared_ptr<CommandEngine> command_engine(new CommandEngine);
	command_engine->SetComm(comm_provider->GetComm());
	
	uint32_t start_addr;
	uint32_t end_addr;
	boost::shared_ptr<Memory> memory;
	
	start_addr = 0x60;
	end_addr = 0x25F;
	memory = boost::shared_ptr<Memory>(new Memory(Memory::SRAM, start_addr,
												end_addr, command_engine));
	QxMemory *sram = new QxMemory(memory);

	//start_addr = 0x0;
	//end_addr = 0x1FF;
	//memory = boost::shared_ptr<Memory>(new Memory(Memory::EEPROM, start_addr,
	//											end_addr, command_engine));
	//QxMemory *eeprom = new QxMemory(memory);

	//start_addr = 0x0;
	//end_addr = 0xFFF;
	//memory = boost::shared_ptr<Memory>(new Memory(Memory::FLASH, start_addr,
	//											end_addr, command_engine));
	//QxMemory *flash = new QxMemory(memory);

	sram->show();
	//eeprom->show();
	//flash->show();

	splash->finish(sram);
	delete splash;

	return app.exec();
}
