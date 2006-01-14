
#ifndef STATETXTWINDOW_H
#define STATETXTWINDOW_H


#include <boost/shared_ptr.hpp>
#include "satori/state.h"


/**
 * Class for user interaction with the current state of an MCU. If better
 * visual output of the state is needed an architecture module can subclass
 * this class and overload the GetState() method.
 */
class StateTxtWindow {
public:
	StateTxtWindow(boost::shared_ptr<State> state);
	void GetState();
	void SetState();

private:
	boost::shared_ptr<State> state;
};


#endif
