
#ifndef STATE_H
#define STATE_H


#include <string>
#include <vector>
#include <inttypes.h>	/// @Todo Phase out this file since it's not portable.
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include "commandengine.h"


/**
 * This class describes and holds the value of a state register such as MCU
 * general purpose registers, flags, program counters, stack pointers etc.
 */
class StateEntry {
public:
	StateEntry(std::string n, unsigned int bs, uint16_t v=0) :
			name(n), size(bs), value(v) { updated = false; }
	
	std::string name;		///< name of the state register
	unsigned int size;		///< state register size in bytes
	uint32_t value;			///< value of state register
	bool updated;			///< flag to check if value changed since last read
};


/**
 * This class is a generic state class that holds the state of an MCU.
 *
 */
class State {
public:
	typedef std::vector<StateEntry> StateMap;

	State(StateMap &state_map, boost::shared_ptr<CommandEngine> command_engine);

	const StateMap &Read();
	void Write(const StateEntry *state_entry);

private:
	boost::shared_ptr<CommandEngine> command_engine;

	unsigned int raw_state_size;
	boost::shared_array<char> raw_state;	///< raw device state
	StateMap state_map;						///< state map mirroring raw state
};


#endif
