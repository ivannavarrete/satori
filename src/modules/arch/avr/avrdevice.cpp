
#include "avrdevice.h"
#include "avrdeviceinfo.h"
#include "avrdisasmengine.h"
#include "satori/disasmengine.h"
#include "satori/commandengine.h"


/**
 * Create a new AVR device object.
 *
 * @param device_name
 *
 * @throws std::runtime_error	Thrown if the device is not supported or if
 * 								there was an error loading the device info.
 *
 * @Todo Change to another (custom, XML) exception.
 */
AvrDevice::AvrDevice(const std::string &device_name) : name(device_name) {
	AvrDeviceInfo device_info(device_name);

	// @Bug We *MUST* create a CommandEngine with a valid Comm object
	command_engine = boost::shared_ptr<CommandEngine>(new CommandEngine);
	boost::shared_ptr<DisasmEngine> disasm_engine(new AvrDisasmEngine);

	uint32_t start_addr;
	uint32_t end_addr;

	device_info.Memory_(Memory::SRAM, &start_addr, &end_addr);
	sram = boost::shared_ptr<Memory>(new Memory(Memory::SRAM, start_addr,
												end_addr, command_engine));

	device_info.Memory_(Memory::EEPROM, &start_addr, &end_addr);
	eeprom = boost::shared_ptr<Memory>(new Memory(Memory::EEPROM, start_addr,
												end_addr, command_engine));

	device_info.Memory_(Memory::FLASH, &start_addr, &end_addr);
	flash = boost::shared_ptr<Memory>(new Memory(Memory::FLASH, start_addr,
												end_addr, command_engine));

	code = boost::shared_ptr<Code>(new Code(flash, disasm_engine));


	std::vector<StateEntry> state_map;
	device_info.StateMap(&state_map);
	state = boost::shared_ptr<State>(new State(state_map, command_engine));
}


/**
 * Change Comm object of the command engine.
 *
 * @param comm					Comm object to change to.
 */
void AvrDevice::SetComm(boost::shared_ptr<Comm> comm) {
	command_engine->SetComm(comm);
}
