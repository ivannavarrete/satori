
#ifndef AVRCOMMANDTABLE_H
#define AVRCOMMANDTABLE_H


#include "lib/command/commandtable.h"


/**
 * Command table holding the AVR architecture commands.
 */
class AVRCommandTable : public CommandTable {
public:
	enum AVRCommand {
		GetDevice = 100,
		SetDevice = 101,
		GetSRAM = 102,
		GetEEPROM = 103,
		GetFLASH = 104,
		SetSRAM = 105,
		SetEEPROM = 106,
		GetState = 107,
		SetState = 108,
		GetCode = 109,
		Help = 199
	};

	AVRCommandTable();
};


#endif
