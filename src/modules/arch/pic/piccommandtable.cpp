
#include <list>
#include "piccommandtable.h"
#include "../command.h"


PICCommandTable::PICCommandTable() {
	boost::shared_ptr<Command> command;

	// help
	command = boost::shared_ptr<Command>(new Command("help", Help));
	command_table.push_back(command);
}
