
#include "commandtable.h"
#include "command.h"


/**
 * Initialize the @a command object to equal a command in the command table.
 * The name of the command to search for is specified by the @a command object's
 * name, as set when it is constructed. If the command is not found in the
 * command table the @a command object is set to the syntax error command.
 * 
 * @param command			Command object to initialize
 * @param command_name		name of command to search for
 */
bool CommandTable::Find(Command &command, const std::string &command_name) const {
	for (unsigned int i=0; i<command_table.size(); ++i) {
		if (command_name == command_table.at(i)->Name()) {
			command = *command_table.at(i);
			return true;
		}
	}

	return false;
}
