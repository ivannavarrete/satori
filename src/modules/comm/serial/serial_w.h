
#ifndef SERIAL_W_H
#define SERIAL_W_H


#include <string.h>
#include "comm.h"


using namespace std;


/**
 * The Serial class provides communication facilities to talk to the programmer
 * through a serial interface. This class is the windows version.
 */
class Serial : public Comm {
public:
	Serial(const string &devname);
	virtual ~Serial();
	int Config(int baud, int dbits, int sbits);
	virtual int Send(const uint8_t *data, const uint32_t dsize);
	virtual int Receive(uint8_t *data, const uint32_t dsize);

private:
	int Save();
	int Restore();
	//int Flush();

	COMMCONFIG saved_mode;
	Handle dev;
};


#endif
