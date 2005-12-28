
#include <list>
#include <boost/shared_ptr.hpp>
#include "serialcommandtable.h"
#include "lib/command/command.h"


/**
 * Create a command table with the supported commands for the serial comm
 * domain.
 */
SerialCommandTable::SerialCommandTable() {
	boost::shared_ptr<Command> command;
	std::list<boost::shared_ptr<Argument> > argument_list;

	// help [command]
	command = boost::shared_ptr<Command>(new Command("help", Help));
	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new WordArgument(
													Argument::Optional)));
	command->AddArguments(argument_list);
	command_table.push_back(command);

	// set [serial] [variable]
	command = boost::shared_ptr<Command>(new Command("set", Set));
	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new WordArgument(
													Argument::Optional)));
	//argument_list.push_back(boost::shared_ptr<Argument>(new VariableArgument(
	//												Argument::Optional)));
	command->AddArguments(argument_list);
	//command_table.push_back(command);
}
