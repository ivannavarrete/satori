
#include <stdexcept>
#include "commandengine.h"
#include "packet.h"


/**
 * Set the object used for communicating with the monitor.
 *
 * @param comm					pointer to communication object
 */
void CommandEngine::SetComm(boost::shared_ptr<Comm> comm) {
	this->comm = comm;
}


/**
 * Read memory from the monitor.
 *
 * @param memory_type			type of memory to read
 * @param addr					start address
 * @param data					buffer where to store read data
 * @param dsize					size of buffer
 *
 * @throws std::runtime_error	Thrown if there was an error reading from or
 * 								writing to comm device.
 */
void CommandEngine::ReadMemory(Memory::Type mtype, uint32_t addr, char *data, uint32_t dsize) {
	uint8_t cmd = static_cast<uint8_t>(CmdReadMemory);
	uint8_t cmd_mod = static_cast<uint8_t>(mtype);

	Packet sp(cmd, cmd_mod, addr, dsize);
	sp.Send(comm);
	
	Packet rp(data, dsize);
	rp.Receive(comm);
}


/**
 * Write data to monitor memory.
 *
 * @param memory_type			type of memory to write to
 * @param addr					start address
 * @param data					data to write
 * @param dsize					size of data
 *
 * @throws std::runtime_error	Thrown if there was an error writing to Comm
 * 								device.
 */
void CommandEngine::WriteMemory(Memory::Type mtype, uint32_t addr, const char *data, uint32_t dsize) {
	uint8_t cmd = static_cast<uint8_t>(CmdWriteMemory);
	uint8_t cmd_mod = static_cast<uint8_t>(mtype);
	
	Packet sp(cmd, cmd_mod, addr, dsize, data, dsize);
	sp.Send(comm);
}


/**
 * Read state from the monitor.
 *
 * @param state
 * @param state_size
 *
 * @throws std::runtime_error	Thrown if there was an error reading or writing
 *								to Comm device.
 */
void CommandEngine::GetState(char *state, unsigned int state_size) {
	uint8_t cmd = static_cast<uint8_t>(CmdGetState);

	Packet sp(cmd, 0);
	sp.Send(comm);

	Packet rp(state, state_size);
	rp.Receive(comm);
}


/**
 * Set monitor state.
 *
 * @param state
 * @param state_size
 *
 * @throws std::runtime_error	Thrown if there was an error writing to Comm
 * 								device.
 */
void CommandEngine::SetState(const char *state, unsigned int state_size) {
	uint8_t cmd = static_cast<uint8_t>(CmdSetState);

	Packet sp(cmd, 0, 0, 0, state, state_size);
	sp.Send(comm);
}


/**
 *
 * @throws std::runtime_error	Thrown if there was an error writing to Comm
 * 								device.
 */
void CommandEngine::SetBreakpoint(uint32_t breakpoint_num, uint32_t addr) {
	uint8_t cmd = static_cast<uint8_t>(CmdSetBreakpoint);

	Packet sp(cmd, 0, breakpoint_num, addr);
	sp.Send(comm);
}


/**
 *
 * @throws std::runtime_error	Thrown if there was an error writing to Comm
 * 								device.
 */
void CommandEngine::ClearBreakpoint(uint32_t breakpoint_num, uint32_t addr) {
	uint8_t cmd = static_cast<uint8_t>(CmdClearBreakpoint);

	Packet sp(cmd, 0, breakpoint_num, addr);
	sp.Send(comm);
}
