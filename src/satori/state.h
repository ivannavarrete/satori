
#ifndef STATE_H
#define STATE_H


#include <string>
#include <vector>
#include <inttypes.h>

using namespace std;


/**
 * State entry class.
 *
 */
class StateEntry {
public:
	StateEntry(string n, uint16_t v, unsigned int bs) :
				name(n), size(bs), value(v) { updated = false; }
	
	string name;
	unsigned int size;
	uint32_t value;
	bool updated;
};

/**
 * State class.
 *
 */
class State {
public:
	State();
	~State();
	int Read(vector<StateEntry> *state_map_copy);
	int Write(const StateEntry *state_entry);

private:
	static const int state_size = 37;
	static const int num_regs = 32;

	union {
		char raw[state_size];
		struct {
			uint8_t regs[num_regs];
			uint16_t pc;
			uint16_t sp;
			uint8_t sreg;
		} field;
	} state;

	vector<StateEntry> state_map;
};


#endif
