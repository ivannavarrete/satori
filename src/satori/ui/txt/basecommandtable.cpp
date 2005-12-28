
#include <list>
#include <boost/shared_ptr.hpp>
#include "basecommandtable.h"
#include "lib/command/command.h"


/**
 * Create a command table with the default commands.
 */
BaseCommandTable::BaseCommandTable() {
	boost::shared_ptr<Command> command;
	std::list<boost::shared_ptr<Argument> > argument_list;

	// quit
	command = boost::shared_ptr<Command>(new Command("quit", Quit,
"]   quit                                  terminate program\n",
"]   quit                                  terminate program\n"));
	command_table.push_back(command);

	// cls
	command = boost::shared_ptr<Command>(new Command("cls", ClearScreen,
"]   cls                                   clear screen\n",
"]   cls                                   clear screen\n"));
	command_table.push_back(command);

	// help [command]
	command = boost::shared_ptr<Command>(new Command("help", Help,
"]   help [command]                        display help\n",
"]   help                                  display brief help on all commands\n"
"]   help <command>                        display detailed help on <command>\n"));
	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new WordArgument(
													Argument::Optional)));
	command->AddArguments(argument_list);
	command_table.push_back(command);

	// module [module name]
	command = boost::shared_ptr<Command>(new Command("module", ShowModule,
"]   module [module_name]                  show/load modules\n",
"]   module                                show available modules\n"
"]   module <module_name>                  load module <module_name>\n"));
	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new WordArgument(
										Argument::Optional, LoadModule)));
	command->AddArguments(argument_list);
	command_table.push_back(command);
}
