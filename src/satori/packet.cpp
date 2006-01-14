
#include <netinet/in.h>			// hton*(), ntoh*()
#include <stdexcept>
#include "packet.h"


/**
 * Create a packet for sending. The parameters are used to build the packet
 * header and payload info. Only cmd and cmd_mod are mandatory.
 *
 * @param cmd					the command to send to monitor
 * @param cmd_mod				command modifier
 * @param arg1					command argument (address, etc)
 * @param arg2					command argument (address, etc)
 * @param data					pointer to packet payload
 * @param dsize					size of packet payload
 */
Packet::Packet(const uint8_t cmd, const uint8_t cmd_mod, const uint32_t arg1, const uint32_t arg2, const char *data, const uint16_t dsize) {
	type = SendPacket;

	// initialize packet header
	header.field.cmd = cmd;
	header.field.cmd_mod = cmd_mod;
	header.field.dsize = htons(dsize);
	header.field.arg1 = htonl(arg1);
	header.field.arg2 = htonl(arg2);

	// initialize packet data.. the constness of the data is 'ensured' by the
	// fact that this is a sengind packet
	this->data = const_cast<char *>(data);
	this->dsize = dsize;
}


/**
 * Create a packet for receiving.
 *
 * @param data					pointer to buffer where to store packet payload
 * @param dsize					size of payload buffer
 */
Packet::Packet(char *data, const uint16_t dsize) {
	type = ReceivePacket;

	for (int i=0; i<header_size; i++)
		header.raw[i] = 0;

	this->data = data;
	this->dsize = dsize;
}


/**
 * Send packet to programmer.
 *
 * @param comm					Comm interface to send on
 *
 * @throws std::runtime_error	Thrown if there was an error writing to device.
 * @throws std::logic_error		Thrown if Send() was called on a packet
 * 								created through the receiving constructor.
 */
void Packet::Send(boost::shared_ptr<Comm> comm) const {
	if (type != SendPacket)
		throw std::logic_error("Receiving packet used to send");

	comm->Send(header.raw, header_size);
	comm->Send(data, dsize);
}


/**
 * Receive packet from programmer.
 *
 * @param comm					Comm interface to receive on
 *
 * @throws std::runtime_error	Thrown if there was an error reading from dev.
 * @throws std::logic_error		Thrown if Receive() was called on a packet
 * 								created through the sending constructor or
 * 								if packet data size is wrong which indicates
 * 								that sender or receiver is implementing the
 * 								higher level protocol incorrectly.
 */
void Packet::Receive(boost::shared_ptr<Comm> comm) {
	if (type != ReceivePacket)
		throw std::logic_error("Sending packet used to receive");

	// receive packet header
	comm->Receive(header.raw, header_size);

	// convert header to host byte order
	header.field.dsize = ntohs(header.field.dsize);
	header.field.arg1 = ntohl(header.field.arg1);
	header.field.arg2 = ntohl(header.field.arg2);

	//uint16_t real_dsize = (uint16_t)std::min(dsize, (header.field.dsize));

	// if dsize != header.field.dsize then there's probably a protocol error
	if (dsize != header.field.dsize)
		throw std::logic_error("protocol error: data size mismatch");

	// receive packet data
	comm->Receive(data, dsize);
}
