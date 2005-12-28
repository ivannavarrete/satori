
#ifndef SERIALTXTUI_H
#define SERIALTXTUI_H


#include <QtCore>					// Q_EXPORT_PLUGIN macro
#include <QObject>
#include <boost/shared_ptr.hpp>
#include "serialcommandtable.h"
#include "ui/txt/txtuiinterface.h"
#include "lib/command/command.h"
#include "modules/comm/commprovider.h"
#include "serial_u.h"


class SerialTxtUI : public QObject, public TxtUIInterface, public CommProvider {
	Q_OBJECT
	Q_INTERFACES(TxtUIInterface)
	Q_INTERFACES(CommProvider)

public:
	SerialTxtUI();
	virtual boost::shared_ptr<Comm> GetComm() const;
	virtual bool Find(Command &command, const std::string &command_name) const;
	virtual void Exec(const Command &command);

private:
	void CommandSet(const Command &command);
	void CommandHelp(const Command &command) const;

	SerialCommandTable command_table;
	boost::shared_ptr<Serial> serial;
};


#endif
