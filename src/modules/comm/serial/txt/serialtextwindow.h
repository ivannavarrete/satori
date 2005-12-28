
#ifndef SERIALTEXTWINDOW_H
#define SERIALTEXTWINDOW_H


#include <boost/shared_ptr.hpp>
#include "serial_u.h"


class SerialTextWindow {
public:
	SerialTextWindow();
	void SetBaud(unsigned int baud);
	void SetDataBits(unsigned int data_bits);
	void SetStopBits(unsigned int stop_bits);
	void Info() const;

private:
	boost::shared_ptr<Serial> serial;
};


#endif
