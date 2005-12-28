
#ifndef BASECOMMANDTABLE_H
#define BASECOMMANDTABLE_H


#include "lib/command/commandtable.h"


/**
 * This class contains the commands for the architecture-independent text
 * user interface class TxtUI.
 */
class BaseCommandTable : public CommandTable {
public:
	enum BaseCommand {
		Quit = 1,
		ShowModule = 2,
		LoadModule = 3,
		ClearScreen = 4,
		Help = 5
	};

	BaseCommandTable();
};


#endif
