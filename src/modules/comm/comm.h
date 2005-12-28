
#ifndef COMM_H
#define COMM_H


#include <inttypes.h>


/**
 * The Comm class provides the abstract communication interface to the
 * programmer.
 */
class Comm {
public:
	/**
	 * Send data to the programmer.
	 *
	 * @param data					pointer to data buffer
	 * @param dsize					number of bytes to send
	 * 
	 * @throws std::runtime_error	Thrown when there is an error writing
	 * 								to device.
	 */
	virtual void Send(const char *data, const uint32_t dsize) const = 0;

	/**
	 * Receive data from the programmer.
	 *
	 * @param data					pointer to data buffer
	 * @param dsize					number of bytes to receive
	 *
	 * @throws std::runtime_error	Thrown when there is an error reading from
	 * 								device.
	 */
	virtual void Receive(char *data, const uint32_t dsize) const = 0;
};


#endif
