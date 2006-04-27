
#ifndef BASETXTUI_H
#define BASETXTUI_H


#include <QObject>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/tokenizer.hpp>
#include "txtui.h"
#include "basecommandtable.h"
#include "lib/command/command.h"
#include "modules/comm/commuser.h"
#include "modules/comm/commprovider.h"


/**
 * The architecture-independent text user interface. It contains the command
 * loop (the idle loop) and handles general commands. The class extends QObject
 * to be able to have an active idle loop while the application is in it's
 * event handle loop. To be able to start the idle loop we must fire it from
 * a timer, which only works on a QObject.
 *
 * @Bug Modules are not automatically unloaded it seems.
 */
class BaseTxtUi : public QObject, public TxtUi {
	Q_OBJECT

public:
	BaseTxtUi() : arch_ui(0), comm_user(0), comm_ui(0), comm_provider(0) {}

	virtual bool Find(Command &command, const std::string &command_name) const;
	virtual void Exec(const Command &command);

public slots:
	void Idle();

private:
	void CommandQuit() const;
	void CommandShowModule() const;
	void CommandLoadModule(const Command &command);
	void CommandClearScreen() const;
	void CommandHelp(const Command &command) const;
	void CommandUnknown() const;
	
	void Message(const std::string &message="") const;
	void SyntaxError(const std::string &reason="") const;
	void Error(const std::string &reason) const;

	BaseCommandTable command_table;

	TxtUi *arch_ui;
	CommUser *comm_user;

	TxtUi *comm_ui;
	CommProvider *comm_provider;
};


#endif
