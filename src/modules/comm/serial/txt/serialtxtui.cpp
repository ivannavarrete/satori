
#include <iostream>
#include "serialtxtui.h"


/**
 *
 */
SerialTxtUI::SerialTxtUI() {
	// @todo Remove this. Have the Serial created and modified with a 'set'
	// 		 command instead.
	serial = boost::shared_ptr<Serial>(new Serial("/dev/ttyS1"));
	serial->Baud(38400);
	serial->DataBits(8);
	serial->StopBits(1);
}


/**
 *
 */
boost::shared_ptr<Comm> SerialTxtUI::GetComm() const {
	return serial;
}


/**
 *
 */
bool SerialTxtUI::Find(Command &command, const std::string &command_name) const{
	return command_table.Find(command, command_name);
}


/**
 *
 */
void SerialTxtUI::Exec(const Command &command) {
	switch (command.Type()) {
	case SerialCommandTable::Set:
		CommandSet(command);
		break;
	case SerialCommandTable::Help:
		CommandHelp(command);
		break;
	default:
		// throw invalid_argument here
		break;
	}

}


/**
 *
 */
void SerialTxtUI::CommandSet(const Command &command) {
	// if 'set' is called with no arguments or an argument equal to 'serial'
	// then display all serial variables
	if (command.ArgumentCount() == 0 ||
		(command.ArgumentCount() == 1 && command.GetWord(1) == "serial")) {
		std::cout << "] --[ Serial variables ]--\n"
				  << "]  baud: " << serial->Baud() << "\n"
				  << "]  data_bits: " << serial->DataBits() << "\n"
				  << "]  stop_bits: " << serial->StopBits() << std::endl;

	// else if 'set' is called with two arguments and first is equal to 'serial'
	// then we set a serial variable
	} else if (command.ArgumentCount() == 2 && command.GetWord(1) == "serial") {
		// check second var and change corresponding variable or throw exception
		// if no such variable
		// @todo Fix this if clause.
		// ...
		// ...

		// temporary..
		std::cout << "set serial var...\n";
		serial->Baud(9600);
		serial->DataBits(8);
		serial->StopBits(1);
	}
}


/**
 * Display help.
 *
 * @param command				Command object holding optional command name.
 */
void SerialTxtUI::CommandHelp(const Command &command) const {
	// display long help on specific command
	if (command.IsValid(1)) {
		Command help_command("null_command");
		if (Find(help_command, command.GetWord(1)))
			std::cout << help_command.LongDescription();
	// display brief help on all commands
	} else {
		std::cout << "] --[ Serial communication commands ]--\n";

		for (CommandTable::const_command_iterator command_i =
			 command_table.begin(); command_i != command_table.end();
			 ++command_i) {
			std::cout << (*command_i)->ShortDescription();
		}
	}
}


Q_EXPORT_PLUGIN(SerialTxtUI)
