
#ifndef AVRTXTUI_H
#define AVRTXTUI_H


#include <QtCore>
#include <string>
#include "avrcommandtable.h"
#include "../../avrdevice.h"
#include "lib/command/command.h"
#include "satori/ui/txt/txtui.h"
#include "satori/ui/txt/memorytxtwindow.h"
#include "satori/ui/txt/statetxtwindow.h"
#include "avrcodetxtwindow.h"
#include "modules/comm/commuser.h"


/**
 * This class implements a text user interface for the AVR architecture module.
 * It's job is to contain all the UI view objects and to dispatch user commands
 * to the proper objects for execution.
 */
class AvrTxtUi : public QObject, public TxtUi, public CommUser {
	Q_OBJECT
	Q_INTERFACES(TxtUi)
	Q_INTERFACES(CommUser)

public:
	virtual void SetComm(boost::shared_ptr<Comm> comm);
	virtual bool Find(Command &command, const std::string &command_name) const;
	virtual void Exec(const Command &command);

private:
	void CommandGetDevice() const;
	void CommandSetDevice(const Command &command);
	void CommandGetCode(const Command &command);
	void CommandGetMemory(const Command &command);
	void CommandSetMemory(const Command &command);
	void CommandGetState(const Command &command);
	void CommandSetState(const Command &command);
	void CommandHelp(const Command &command) const;

	bool DeviceLoaded() const;

	AVRCommandTable command_table;

	// It is a little ugly to keep a reference here to the Comm object that a
	// device uses for communication. We do it anyway because it is possible
	// for this UI to be loaded but without containing any device. In this way
	// a device can be loaded later and use the latest set Comm object.
	boost::shared_ptr<Comm> comm;
	boost::shared_ptr<AvrDevice> avr_device;

	boost::shared_ptr<MemoryTxtWindow> sram;
	boost::shared_ptr<MemoryTxtWindow> eeprom;
	boost::shared_ptr<MemoryTxtWindow> flash;
	boost::shared_ptr<StateTxtWindow> state;
	//boost::shared_ptr<IOTxtWindow> io;
	boost::shared_ptr<CodeTxtWindow> code;
};


#endif
