
#include <iostream>
#include "avrtxtui.h"
#include "satori/ui/txt/color.h"


/**
 * Set the object used for communication with the hardware.
 *
 * @param comm					pointer to communication object
 */
void AvrTxtUi::SetComm(boost::shared_ptr<Comm> comm) {
	this->comm = comm;
	if (avr_device)
		avr_device->SetComm(comm);
}


/**
 *
 */
bool AvrTxtUi::Find(Command &command, const std::string &command_name) const {
	return command_table.Find(command, command_name);
}


/**
 *
 */
void AvrTxtUi::Exec(const Command &command) {
	Color::Init256();				// Todo: Move to proper location

	switch (command.Type()) {
	case AVRCommandTable::GetDevice:
		CommandGetDevice();
		break;

	case AVRCommandTable::SetDevice:
		CommandSetDevice(command);
		break;

	case AVRCommandTable::GetCode:
		CommandGetCode(command);
		break;

	case AVRCommandTable::GetSRAM:
	case AVRCommandTable::GetEEPROM:
	case AVRCommandTable::GetFLASH:
		CommandGetMemory(command);
		break;

	case AVRCommandTable::SetSRAM:
	case AVRCommandTable::SetEEPROM:
		CommandSetMemory(command);
		break;

	case AVRCommandTable::GetState:
		CommandGetState(command);
		break;

	case AVRCommandTable::SetState:
		CommandSetState(command);
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
 * Display information about the currently loaded device.
 */
void AvrTxtUi::CommandGetDevice() const {
	if (!DeviceLoaded())
		return;

	Message("device: " + avr_device->Name());
}



/**
 * Load a new device. If the method fails the old device (if any) is kept.
 *
 * @param command				Command object holding device name
 */
void AvrTxtUi::CommandSetDevice(const Command &command) {
	// create a new device
	try {
		// do we really need the intermediate new_device variable?
		boost::shared_ptr<AvrDevice> new_device
				(new AvrDevice(command.GetWord(1)));
		avr_device = new_device;
	} catch (std::runtime_error &e) {
		Error(std::string("failed loading device [") + command.GetWord(1) +
			  std::string("]: ") + e.what());
		return;
	}

	// set the Comm object to use, or keep the default one
	if (comm)
		avr_device->SetComm(comm);

	// create views for the new device
	sram = boost::shared_ptr<MemoryTxtWindow>
			(new MemoryTxtWindow(avr_device->Sram()));
	
	eeprom = boost::shared_ptr<MemoryTxtWindow>
			(new MemoryTxtWindow(avr_device->Eeprom()));
	
	flash = boost::shared_ptr<MemoryTxtWindow>
			(new MemoryTxtWindow(avr_device->Flash()));

	//io = boost::shared_ptr<IoTxtWindow>
	//		(new IoTxtWindow(avr_device->Io()));

	state = boost::shared_ptr<StateTxtWindow>
			(new StateTxtWindow(avr_device->State_()));

	code = boost::shared_ptr<CodeTxtWindow>
			(new AvrCodeTxtWindow(avr_device->Code_()));

	Message("device loaded: " + avr_device->Name());
}


void AvrTxtUi::CommandGetCode(const Command &command) {
	if (!DeviceLoaded())
		return;

	// there are 2 bytes per instruction (flash) address
	uint32_t start_addr = command.GetNumber(1) * 2;
	uint32_t end_addr = start_addr + 0x1F * 2;
	if (command.IsValid(2))
		end_addr = command.GetNumber(2) * 2;

	// add 1 so that the end addr becomes the last byte of an instruction word
	code->Disassemble(start_addr, end_addr+1);
}


/**
 *
 */
void AvrTxtUi::CommandGetMemory(const Command &command) {
	if (!DeviceLoaded())
		return;

	uint32_t start_addr = command.GetNumber(1);
	uint32_t end_addr = start_addr + 0x3F;
	if (command.IsValid(2))				// don't use IsValid(2,Argument::Number)
		end_addr = command.GetNumber(2);
	
	if (command.Type() == AVRCommandTable::GetSRAM) {
		Message("--[ sram ]--");
		sram->Read(start_addr, end_addr);
	} else if (command.Type() == AVRCommandTable::GetEEPROM) {
		Message("--[ eeprom ]--");
		eeprom->Read(start_addr, end_addr);
	} else if (command.Type() == AVRCommandTable::GetFLASH) {
		Message("--[ flash ]--");
		flash->Read(start_addr, end_addr);
	}
}


/**
 *
 */
void AvrTxtUi::CommandSetMemory(const Command &command) {
	if (!DeviceLoaded())
		return;

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
void AvrTxtUi::CommandGetState(const Command & /* command */) {
	if (!DeviceLoaded())
		return;

	state->GetState();
}


/**
 *
 */
void AvrTxtUi::CommandSetState(const Command & /* command */) {
	if (!DeviceLoaded())
		return;

	Error("Not implemented");
}


/**
 * Display help.
 *
 * @param command				Command object holding optional command name
 */
void AvrTxtUi::CommandHelp(const Command &command) const {
	// display long help on specific command
	if (command.IsValid(1)) {
		Command help_command("null_command");
		if (Find(help_command, command.GetWord(1)))
			Message(help_command.LongDescription());
	// display brief help on all commands
	} else {
		Message("--[ AVR architecture commands ]--");

		for (CommandTable::const_command_iterator command_i =
			 command_table.begin(); command_i != command_table.end();
			 ++command_i) {
			Message((*command_i)->ShortDescription());
		}
	}
}


/**
 * Check whether a device is loaded. This method is used by all the command
 * methods that require a device to be loaded in order to function.
 *
 * @returns						True if a device is loaded, false otherwise.
 */
bool AvrTxtUi::DeviceLoaded() const {
	if (!avr_device) {
		Error("no device loaded");
		return false;
	}

	return true;
}


void AvrTxtUi::Message(const std::string &message) const {
	std::cout << Color::Prompt << "] " << Color::Headline << message<<std::endl;
}


/**
 * Display an error message.
 *
 * @param reason				description of error
 */
void AvrTxtUi::Error(const std::string &reason) const {
	std::cout << Color::Prompt << "] " << Color::Error << reason << std::endl;
}


Q_EXPORT_PLUGIN(AvrTxtUi)
