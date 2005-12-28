
#ifndef STATETXTWINDOW_H
#define STATETXTWINDOW_H


#include <boost/shared_ptr.hpp>


class StateTxtWindow {
public:
	StateTxtWindow(boost::shared_ptr<State> state);
	void GetState();
	void SetState();

private:
	void Show() const;

	boost::shared_ptr<State> state;
};

#endif
