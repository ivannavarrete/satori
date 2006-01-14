
#include <iostream>
#include "statetxtwindow.h"


/**
 * Constructor for basic text state window.
 */
StateTxtWindow::StateTxtWindow(boost::shared_ptr<State> state) {
	this->state = state;
}


/**
 * Display the monitor state. This method just prints out all state entries
 * in created order.
 */
void StateTxtWindow::GetState() {
	// configure cout
	std::cout.fill('0');
	std::cout << std::hex;

	// get the state map from state object
	const State::StateMap &state_map = state->Read();

	// display state map
	for (State::StateMap::const_iterator state_entry=state_map.begin();
		 state_entry!=state_map.end(); ++state_entry) {
		std::cout << "] " << state_entry->name << ": "
				  << state_entry->value << std::endl;
	}
}


/**
 *
 */
void StateTxtWindow::SetState() {

}
