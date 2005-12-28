
#include <iostream>
#include "pictxtui.h"


/**
 *
 */
PICTxtUI::PICTxtUI() {

}


/**
 * @todo Document.
 */
bool PICTxtUI::Find(Command &command, const string &command_name) const {
	cout << "PICTxtUI::Find()" << endl;
	return command_table.Find(command, command_name);
}


/**
 * @todo Document.
 */
void PICTxtUI::Exec(const Command &command) {
	cout << "PICTxtUI::Exec()" << endl;
	switch (command.Type()) {
	case PICCommandTable::Help:
		CommandHelp();
		break;
	default:
		// throw invalid_argument here
		break;
	}
}


/**
 *
 */
void PICTxtUI::CommandHelp() const {
	cout << ": [ PIC architecture commands ]\n";
	cout << ": ds <start addr> [end addr]              display SRAM\n"
			": de <start addr> [end addr]              display EEPROM\n"
			": df <start addr> [end addr]              display FLASH\n";
}


Q_EXPORT_PLUGIN(PICTxtUI)
