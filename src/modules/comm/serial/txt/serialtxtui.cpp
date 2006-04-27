
#include <iostream>
#include "serialtxtui.h"
#include "satori/ui/txt/color.h"


/**
 *
 */
SerialTxtUI::SerialTxtUI() {
	Color::Init256();

	// @todo Remove this. Have the Serial created and modified with a 'set'
	// 		 command instead.
	serial = boost::shared_ptr<Serial>(new Serial("/dev/ttyS1"));
	serial->SetBaud(38400);
	serial->SetDataBits(8);
	serial->SetStopBits(1);
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
boost::shared_ptr<Comm> SerialTxtUI::GetComm() const {
	return serial;
}


/**
 *
 */
void SerialTxtUI::CommandSet(const Command &command) {
	// if 'set' is called with no arguments or an argument equal to 'serial'
	// then display all serial variables
	if (command.ArgumentCount() == 0 ||
		(command.ArgumentCount() == 1 && command.GetWord(1) == "serial")) {
		Message("--[ Serial variables ]--");
		Message("  baud: " + serial->Baud());
		Message("  data_bits: " + serial->DataBits());
		Message("  stop_bits: " + serial->StopBits());

	// else if 'set' is called with two arguments and first is equal to 'serial'
	// then we set a serial variable
	} else if (command.ArgumentCount() == 2 && command.GetWord(1) == "serial") {
		// check second var and change corresponding variable or throw exception
		// if no such variable
		// @todo Fix this if clause.
		// ...
		// ...

		// temporary..
		Message("set serial var...");
		serial->SetBaud(9600);
		serial->SetDataBits(8);
		serial->SetStopBits(1);
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
			Message(help_command.LongDescription());
	// display brief help on all commands
	} else {
		Message("--[ Serial communication commands ]--");

		for (CommandTable::const_command_iterator command_i =
			 command_table.begin(); command_i != command_table.end();
			 ++command_i) {
			Message((*command_i)->ShortDescription());
		}
	}
}


void SerialTxtUI::Message(const std::string &message) const {
	std::cout << Color::Prompt << "] " << Color::Headline << message<<std::endl;
}


/**
 * Display an error message.
 *
 * @param reason				description of error
 */
void SerialTxtUI::Error(const std::string &reason) const {
	std::cout << Color::Prompt << "] " << Color::Error << reason << std::endl;
}


Q_EXPORT_PLUGIN(SerialTxtUI)
