
#include <stdexcept>
#include <netinet/in.h>	/// @Todo Phase out this file since it's not portable.
#include "state.h"


/**
 * Create state object. Each architecture module should create a state object
 * with a state map specific for that architecture and device. The entries in
 * the state map must match the order and size of the state structure in the
 * monitor.
 *
 * @param state_map				state map with proper state entries
 * @param command_engine		command engine used to communicate with monitor
 */
State::State(std::vector<StateEntry> &state_map, boost::shared_ptr<CommandEngine> command_engine) {
	// calculate size of raw state and allocate buffer for it
	raw_state_size = 0;
	for (StateMap::iterator state_entry=state_map.begin();
		 state_entry!=state_map.end(); ++state_entry)
		raw_state_size += state_entry->size;

	raw_state = boost::shared_array<char>(new char[raw_state_size]);

	// save command engine and copy state map to internal variables
	this->state_map = state_map;
	this->command_engine = command_engine;
}


/**
 * Read state from monitor and return the updated state map.
 *
 * @return					reference to the updated state map
 */
const State::StateMap &State::Read() {
	// get state from monitor
	command_engine->GetState(raw_state.get(), raw_state_size);
	
	// update state map
	unsigned int raw_i = 0;
	for (StateMap::iterator state_entry=state_map.begin();
		 state_entry!=state_map.end(); ++state_entry){
		uint32_t value = 0;

		// get value from raw state
		if (state_entry->size == 1)
			value = static_cast<uint8_t>(raw_state[raw_i]);
		else if (state_entry->size == 2)
			value = ntohs(*(reinterpret_cast<uint16_t *>(&raw_state[raw_i])));
		else if (state_entry->size == 4)
			value = ntohl(*(reinterpret_cast<uint32_t *>(&raw_state[raw_i])));
		else
			throw std::logic_error("wrong register size");

		raw_i += state_entry->size;

		// update state entry
		if (state_entry->value == value) {
			state_entry->updated = false;
		} else {
			state_entry->value = value;
			state_entry->updated = true;
		}
	}

	return state_map;
}


/**
 * Write state entry.
 *
 * @param state_entry	dunno
 */
void State::Write(const StateEntry *state_entry) {
	// temporatily remove 'unused parameter' warnings
	if (state_entry->size == state_entry->size);

	/*
	unsigned int size = 1;
	for (unsigned int i=0, j=0; i<state_map.size(); i++, j+=size) {
		size = state_map.at(i).size;

		if (state_map.at(i).name == state_entry->name) {
			state_map.at(i).value = state_entry->value;
			
			if (size == 1) {
				state.raw[j] = state_entry->value;
			} else if (size == 2) {
				uint16_t *valuep = reinterpret_cast<uint16_t *>(&state.raw[j]);
				*valuep = htons(state_entry->value);
			} else if (size == 4) {
				uint32_t *valuep = reinterpret_cast<uint32_t *>(&state.raw[j]);
				*valuep = htonl(state_entry->value);
			} else {
				/// @todo Handle error properly.
				return -1;		// should not happen!! throw ex instead
			}
			
			break;
		}
	}

	/// @todo Handle error properly.
	cmd->SetState(state.raw, state_size);
	return 0;
	*/
}
