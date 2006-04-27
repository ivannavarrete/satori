
#ifndef SERIALTXTUI_H
#define SERIALTXTUI_H


#include <QtCore>
#include <boost/shared_ptr.hpp>
#include "serialcommandtable.h"
#include "satori/ui/txt/txtui.h"
#include "lib/command/command.h"
#include "modules/comm/commprovider.h"
#include "serial_u.h"


class SerialTxtUI : public QObject, public TxtUi, public CommProvider {
	Q_OBJECT
	Q_INTERFACES(TxtUi)
	Q_INTERFACES(CommProvider)

public:
	SerialTxtUI();

	// TxtUi interface
	virtual bool Find(Command &command, const std::string &command_name) const;
	virtual void Exec(const Command &command);

	// CommProvider interface
	virtual boost::shared_ptr<Comm> GetComm() const;

private:
	void CommandSet(const Command &command);
	void CommandHelp(const Command &command) const;

	void Message(const std::string &message="") const;
	void Error(const std::string &reason) const;

	SerialCommandTable command_table;
	boost::shared_ptr<Serial> serial;
};


#endif
