
#include <iostream>
#include "avrtxtui.h"
#include "memory.h"


/**
 * Init the AVR text UI.
 */
AVRTxtUI::AVRTxtUI() {
	command_engine = boost::shared_ptr<CommandEngine>(new CommandEngine);
}


/**
 * Set the object used for communication with the hardware.
 *
 * @param comm					pointer to communication object
 */
void AVRTxtUI::SetComm(boost::shared_ptr<Comm> comm) {
	command_engine->SetComm(comm);
}


/**
 *
 */
bool AVRTxtUI::Find(Command &command, const std::string &command_name) const {
	return command_table.Find(command, command_name);
}


/**
 *
 */
void AVRTxtUI::Exec(const Command &command) {
	switch (command.Type()) {
	case (AVRCommandTable::GetDevice):
		CommandGetDevice();
		break;
	case (AVRCommandTable::SetDevice):
		CommandSetDevice(command);
		break;
	case (AVRCommandTable::GetSRAM):
	case AVRCommandTable::GetEEPROM:
	case AVRCommandTable::GetFLASH:
		CommandGetMemory(command);
		break;
	case AVRCommandTable::SetSRAM:
	case AVRCommandTable::SetEEPROM:
		CommandSetMemory(command);
		break;
	case AVRCommandTable::Help:
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
void AVRTxtUI::CommandGetDevice() const {
	std::cout << "GetDevice(): hardcoded device..\n";
}


void AVRTxtUI::CommandSetDevice(const Command &command) {
	std::cout << "SetDevice() [" << command.GetWord(1) << "]: hardcoded dev.\n";

	boost::shared_ptr<Memory> memory;
	uint32_t start_addr;
	uint32_t end_addr;

	start_addr = 0x60;
	end_addr = 0x25F;
	memory = boost::shared_ptr<Memory>(new Memory(Memory::SRAM, start_addr,
												end_addr, command_engine));
	sram = boost::shared_ptr<MemoryTxtWindow>(new MemoryTxtWindow(memory));
	
	start_addr = 0x0;
	end_addr = 0x1FF;
	memory = boost::shared_ptr<Memory>(new Memory(Memory::EEPROM, start_addr,
												end_addr, command_engine));
	eeprom = boost::shared_ptr<MemoryTxtWindow>(new MemoryTxtWindow(memory));
	
	start_addr = 0x0;
	end_addr = 0xFFF;
	memory = boost::shared_ptr<Memory>(new Memory(Memory::FLASH, start_addr,
												end_addr, command_engine));
	flash = boost::shared_ptr<MemoryTxtWindow>(new MemoryTxtWindow(memory));
}


/**
 *
 */
void AVRTxtUI::CommandGetMemory(const Command &command) {
	uint32_t start_addr = command.GetNumber(1);
	uint32_t end_addr = start_addr + 0x40;
	if (command.IsValid(2))				// don't use IsValid(2,Argument::Number)
		end_addr = command.GetNumber(2);
	
	if (command.Type() == AVRCommandTable::GetSRAM) {
		std::cout << "] --[ sram ]--\n";
		sram->Read(start_addr, end_addr);
	} else if (command.Type() == AVRCommandTable::GetEEPROM) {
		std::cout << "] --[ eeprom ]--\n";
		eeprom->Read(start_addr, end_addr);
	} else if (command.Type() == AVRCommandTable::GetFLASH) {
		std::cout << "] --[ flash ]--\n";
		flash->Read(start_addr, end_addr);
	}
}


/**
 *
 */
void AVRTxtUI::CommandSetMemory(const Command &command) {
	uint32_t start_addr = command.GetNumber(1);
	const char *data;
	uint32_t data_size;
	
	// extract the data and data size from arguments
	if (command.IsValid(2, Argument::String)) {
		data = command.GetString(2).c_str();
		data_size = command.GetString(2).length() + 1;
	/*} else if (command.IsValid(2, Argument::Char)) {
		data_buf = command.GetChar(2);
		data_size = 1;
	*/
	/*} else if (command.IsValid(2, Argument::Array)) {
		data = command.GetArray(2).data_or_something();
		data_size = command.GetArray(2).lemgth();
	*/
	} else {
		throw std::logic_error("bad argument: not a number, char or array");
	}

	// write data to memory
	if (command.Type() == AVRCommandTable::SetSRAM)
		sram->Write(start_addr, start_addr+data_size-1, data);
	else if (command.Type() == AVRCommandTable::SetEEPROM)
		eeprom->Write(start_addr, start_addr+data_size-1, data);
}


/**
 *
 */
void AVRTxtUI::CommandGetState(const Command &command) {

}


/**
 *
 */
void AVRTxtUI::CommandSetState(const Command &command) {

}


/**
 * Display help.
 *
 * @param command				Command object holding optional command name.
 */
void AVRTxtUI::CommandHelp(const Command &command) const {
	// display long help on specific command
	if (command.IsValid(1)) {
		Command help_command("null_command");
		if (Find(help_command, command.GetWord(1)))
			std::cout << help_command.LongDescription();
	// display brief help on all commands
	} else {
		std::cout << "] --[ AVR architecture commands ]--\n";

		for (CommandTable::const_command_iterator command_i =
			 command_table.begin(); command_i != command_table.end();
			 ++command_i) {
			std::cout << (*command_i)->ShortDescription();
		}
	}
}


Q_EXPORT_PLUGIN(AVRTxtUI)
