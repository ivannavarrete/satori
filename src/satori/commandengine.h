
#ifndef COMMANDENGINE_H
#define COMMANDENGINE_H


#include <inttypes.h>
#include <boost/shared_ptr.hpp>
#include "memory.h"
#include "modules/comm/comm.h"


/**
 * This class implements the PC side of all the commands that are supported by
 * the monitor.
 *
 * @todo We need to check if comm is not null when using all command methods,
 * 		 and throw appropriate exception.
 */
class CommandEngine {
public:
	void SetComm(boost::shared_ptr<Comm> comm);
	void ReadMemory(Memory::Type memory_type, uint32_t addr, char *data,
														uint32_t dsize);
	void WriteMemory(Memory::Type memory_type, uint32_t addr, const char *data,
																uint32_t dsize);
	void GetState(char *state, unsigned int state_size);
	void SetState(const char *state, unsigned int state_size);
	void SetBreakpoint(uint32_t breakpoint_num, uint32_t addr);
	void ClearBreakpoint(uint32_t breakpoint_num, uint32_t addr);

private:
	/// The commands supported by the monitor. The numerical values must mirror
	/// the ones in the monitor code, and must never exceed 8 bits.
	enum Command { NoCommand=0,
				   CmdReadMemory=1,
				   CmdWriteMemory=2,
				   CmdGetState=3,
				   CmdSetState=4,
				   CmdSetBreakpoint=5,
				   CmdClearBreakpoint=6 };

	boost::shared_ptr<Comm> comm;		///< monitor communication object
};


#endif
