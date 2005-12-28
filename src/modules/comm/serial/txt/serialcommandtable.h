
#ifndef SERIALCOMMANDTABLE_H
#define SERIALCOMMANDTABLE_H


#include "lib/command/commandtable.h"


/**
 * Command table holding the serial communication commands.
 */
class SerialCommandTable : public CommandTable {
public:
	enum SerialCommand {
		Set = 1000,
		Help = 1099
	};

	SerialCommandTable();
};


#endif
