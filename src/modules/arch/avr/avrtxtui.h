
#ifndef AVRTXTUI_H
#define AVRTXTUI_H


#include <QtCore>					// Q_EXPORT_PLUGIN macro
#include <QObject>
#include <string>
#include <boost/scoped_ptr.hpp>
#include "avrcommandtable.h"
#include "lib/command/command.h"
#include "satori/commandengine.h"
#include "satori/ui/txt/txtuiinterface.h"
#include "satori/ui/txt/memorytxtwindow.h"
#include "satori/ui/txt/statetxtwindow.h"
#include "modules/comm/commuser.h"


/**
 * This class implements a text user interface for the AVR architecture module.
 * It's job is to contain all the UI subobjects and to dispatch user commands
 * to the proper objects for execution.
 */
class AVRTxtUI : public QObject, public TxtUIInterface, public CommUser {
	Q_OBJECT
	Q_INTERFACES(TxtUIInterface)
	Q_INTERFACES(CommUser)

public:
	AVRTxtUI();
	virtual void SetComm(boost::shared_ptr<Comm> comm);
	virtual bool Find(Command &command, const std::string &command_name) const;
	virtual void Exec(const Command &command);

private:
	void CommandGetDevice() const;
	void CommandSetDevice(const Command &command);
	void CommandGetMemory(const Command &command);
	void CommandSetMemory(const Command &command);
	void CommandGetState(const Command &command);
	void CommandSetState(const Command &command);
	void CommandHelp(const Command &command) const;

	AVRCommandTable command_table;

	boost::shared_ptr<CommandEngine> command_engine;

	boost::shared_ptr<MemoryTxtWindow> sram;
	boost::shared_ptr<MemoryTxtWindow> eeprom;
	boost::shared_ptr<MemoryTxtWindow> flash;
	boost::shared_ptr<StateTxtWindow> state;
	//boost::shared_ptr<IOTxtWindow> io;
	//boost::shared_ptr<CodeTxtWindow> code;
};


#endif
