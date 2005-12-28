
#ifndef PACKET_H
#define PACKET_H


#include <iostream>				// NULL
#include <boost/shared_ptr.hpp>
#include "modules/comm/comm.h"


/**
 * Packet class. bla bla bla.
 *
 */
class Packet {
public:
	Packet(const uint8_t cmd, const uint8_t cmd_mod, const uint32_t arg1=0,
		const uint32_t arg2=0, const char *data=NULL, const uint16_t dsize=0);
	Packet(char *data, const uint16_t dsize);

	void Send(boost::shared_ptr<Comm> comm) const;
	void Receive (boost::shared_ptr<Comm> comm);
	
private:
	enum Type { SendPacket, ReceivePacket };
	int type;

	static const int header_size = 12;

	union {
		char raw[header_size];
		struct {
			uint8_t cmd;
			uint8_t cmd_mod;
			uint16_t dsize;
			uint32_t arg1;
			uint32_t arg2;
		} field;
	} header;
	
	char *data;					///< pointer to packet payload
	uint16_t dsize;				///< payload size, limited to 65535 bytes
};


#endif
