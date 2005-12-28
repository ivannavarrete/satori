
#include <sstream>
#include <netinet/in.h>
#include "state.h"
#include "commandengine.h"


extern CommandEngine *cmd;


/**
 * Create state object.
 */
State::State() {
	stringstream regname(stringstream::out);

	// insert register entries into state map
	for (int i=0; i<num_regs; i++) {
		regname << "r" << i;
		state_map.push_back(*(new StateEntry(regname.str(), 0, 8)));
	}

	// insert pc, sp and sreg into state map
	state_map.push_back(*(new StateEntry("pc", 16, 0)));
	state_map.push_back(*(new StateEntry("sp", 16, 0)));
	state_map.push_back(*(new StateEntry("sreg", 8, 0)));
}


/**
 * Destroy state object.
 */
State::~State() {
	for (unsigned int i=0; i<state_map.size(); i++)
		delete &(state_map.at(i));
}


/**
 * Read state.
 *
 * @param state_map_copy	copy of state map
 *
 * @return					dunno
 */
int State::Read(vector<StateEntry> *state_map_copy) {
	// get state from target
	/// @todo Handle error properly.
	cmd->GetState(state.raw, state_size);
	
	// update state map
	unsigned int size = 1;
	for (unsigned int i=0, j=0; i<state_map.size(); i++, j+=size) {
		size = state_map.at(i).size;
		if (size == 1) {
			if (state_map.at(i).value == static_cast<uint32_t>(state.raw[j]))
				state_map.at(i).updated = false;
			else {
				state_map.at(i).value = state.raw[j];
				state_map.at(i).updated = true;
			}
		} else if (size == 2) {
			uint16_t *valuep = reinterpret_cast<uint16_t *>(&state.raw[j]);

			if (state_map.at(i).value == ntohs(*valuep))
				state_map.at(i).updated = false;
			else {
				state_map.at(i).value = ntohs(*valuep);
				state_map.at(i).updated = true;
			}
		} else if (size == 4) {
			uint32_t *valuep = reinterpret_cast<uint32_t *>(&state.raw[j]);

			if (state_map.at(i).value == ntohl(*valuep))
				state_map.at(i).updated = false;
			else {
				state_map.at(i).value = ntohl(*valuep);
				state_map.at(i).updated = true;
			}
		} else {
			/// @todo Handle error properly.
			return -1;			// should not happen!! throw ex instead
		}
	}

	// make copy of state map to return
	
	
	return 0;
}


/**
 * Write state entry.
 *
 * @param state_entry	dunno
 *
 * @return				dunno
 */
int State::Write(const StateEntry *state_entry) {
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
}
