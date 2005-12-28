
#ifndef PICTXTUI_H
#define PICTXTUI_H


#include <QtCore>					// Q_EXPORT_PLUGIN macro
#include <QObject>
#include "piccommandtable.h"
#include "../txtuiinterface.h"
#include "../command.h"


class PICTxtUI : public QObject, public TxtUIInterface {
	Q_OBJECT
	Q_INTERFACES(TxtUIInterface)

public:
	PICTxtUI();

	virtual bool Find(Command &command, const string &command_name) const;
	virtual void Exec(const Command &command);

private:
	void CommandHelp() const;

	PICCommandTable command_table;
};


#endif
