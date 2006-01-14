
#include <list>
#include <boost/shared_ptr.hpp>
#include "avrcommandtable.h"
#include "lib/command/command.h"


/**
 * Create a command table with the supported commands for the AVR architecture.
 */
AVRCommandTable::AVRCommandTable() {
	boost::shared_ptr<Command> command;
	std::list<boost::shared_ptr<Argument> > argument_list;

	// device [device_name]
	command = boost::shared_ptr<Command>(new Command("device", GetDevice,
"]   device [device_name]                  display/select AVR device\n",
"]   device                                display current AVR device\n"
"]   device <device_name>                  select AVR device\n"));
	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new WordArgument(
										Argument::Optional, SetDevice)));
	command->AddArguments(argument_list);
	command_table.push_back(command);


	// ds <start_addr> [end_addr | string | array]
	command = boost::shared_ptr<Command>(new Command("ds", GetSRAM,
"]   ds <start_addr> [end_addr | string ]  get/set SRAM memory\n",
"]   ds <start_addr>                       display 40 bytes of SRAM memory\n"
"]                                         starting at <start_addr>\n"
"]   ds <start_addr> <end_addr>            display SRAM memory starting at\n"
"]                                         <start_addr> and ending at <end_addr>\n"
"]   ds <start_addr> <string>              write <string> to SRAM starting at\n"
"]                                         <start_addr>\n"));

	// ds <start_addr> [end_addr]
	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new NumberArgument));
	argument_list.push_back(boost::shared_ptr<Argument>(new NumberArgument(
													Argument::Optional)));
	command->AddArguments(argument_list);
	// ds <start_addr> [string]
	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new NumberArgument));
	argument_list.push_back(boost::shared_ptr<Argument>(new StringArgument(
											Argument::Optional, SetSRAM)));
	command->AddArguments(argument_list);
//	// ds <start_addr> [array]
//	argument_list.clear();
//	argument_list.push_back(shared_ptr<Argument>(new NumberArgument));
//	argument_list.push_back(shared_ptr<Argument>(new ArrayArgument(
//									Argument::Optional, SetSRAM)));
//	command->AddArguments(argument_list);
	command_table.push_back(command);


	// de <start_addr> [end_addr | string | array]
	command = boost::shared_ptr<Command>(new Command("de", GetEEPROM,
"]   de <start_addr> [end_addr | string ]  get/set EEPROM memory\n",
"]   de <start_addr>                       display 40 bytes of EEPROM memory\n"
"]                                         starting at start_addr\n"
"]   de <start_addr> <end_addr>            display EEPROM memory starting at\n"
"]                                         <start_addr> and ending at <end_addr>\n"
"]   de <start_addr> <string>              write <string> to EEPROM starting\n"
"]                                         at <start_addr>\n"));

	// de <start_addr> [end_addr]
	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new NumberArgument));
	argument_list.push_back(boost::shared_ptr<Argument>(new NumberArgument(
													Argument::Optional)));
	command->AddArguments(argument_list);
	// de <start_addr> [string]
	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new NumberArgument));
	argument_list.push_back(boost::shared_ptr<Argument>(new StringArgument(
										Argument::Optional, SetEEPROM)));
	command->AddArguments(argument_list);
//	// de <start_addr> [array]
//	argument_list.clear();
//	argument_list.push_back(shared_ptr<Argument>(new NumberArgument));
//	argument_list.push_back(shared_ptr<Argument>(new ArrayArgument(
	//								Argument::Optional, SetEEPROM)));
//	command->AddArguments(argument_list);
	command_table.push_back(command);

	
	// df <start_addr> [end_addr]
	command = boost::shared_ptr<Command>(new Command("df", GetFLASH,
"]   df <start_addr> [end_addr]            get FLASH memory\n",
"]   df <start_addr>                       display 40 bytes of FLASH memory\n"
"]                                         starting at <start_addr>\n"
"]   df <start_addr> <end_addr>            display FLASH memory starting at\n"
"]                                         <start_addr> and ending at <end_addr>\n"));

	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new NumberArgument));
	argument_list.push_back(boost::shared_ptr<Argument>(new NumberArgument(
													Argument::Optional)));
	command->AddArguments(argument_list);
	command_table.push_back(command);


	// reg [reg_name]
	// @todo implement proper argument type for variables (x=y)
	command = boost::shared_ptr<Command>(new Command("reg", GetState,
"]   reg [reg_name]                        display/set CPU state\n",
"]   reg                                   display CPU state\n"
"]   reg <reg_name> ???                    set register to value ???\n"));

	argument_list.clear();
	//argument_list.push_back(boost::shared_ptr<Argument>(new VariableArgument),
	//    												Argument::Optional);

	//command->AddArguments(argument_list);
	command_table.push_back(command);


	// help [command]
	command = boost::shared_ptr<Command>(new Command("help", Help));
	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new WordArgument(
													Argument::Optional)));
	command->AddArguments(argument_list);
	command_table.push_back(command);
}
