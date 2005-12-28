
#ifndef PICCOMMANDTABLE_H
#define PICCOMMANDTABLE_H


#include "../commandtable.h"


/**
 * Command table holding the AVR architecture commands.
 */
class PICCommandTable : public CommandTable {
public:
	enum PICCommand {
		Help = 299
	};

	PICCommandTable();
};


#endif
