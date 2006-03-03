
#include <unistd.h>			// close(), etc
#include <stdexcept>
#include <sys/types.h>		// open(), stat, etc
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "serial_u.h"


/**
 * Open serial device. The current mode of the serial device is determined and
 * saved.
 * 
 * @bug There is a potential problem because the device is allowed to block on
 * 		open, causing the constructor to block.
 *
 * @throws std::runtime_error	This exception is thrown if there was an error
 * 								opening the file, if the file wasn't a character
 * 								device, or if the device mode couldn't be
 * 								determined and saved.
 */
Serial::Serial(const std::string &device_name) {
	this->device_name = device_name;

	// check whether it really is a character device
	struct stat file_info;
	if (stat(device_name.c_str(), &file_info) < 0) {
		std::string error("Unable to determine device info: ");
		error += strerror(errno);
		throw std::runtime_error(error);
	} else {
		if (!S_ISCHR(file_info.st_mode)) {
			throw std::runtime_error("Device is not a serial device");
		}
	}

	// open device
	if ( (dev = open(device_name.c_str(), O_RDWR | O_NOCTTY | O_SYNC)) == -1) {
		std::string error("Unable to open serial device: ");
		error += strerror(errno);
		throw std::runtime_error(error);
	}

	// determine and save device mode; if this fails the device would probably
	// fail in other ways too, so we make it a fatal error
	if (!Save()) {
		close(dev);
		std::string error("Unable to determine device mode: ");
		error += strerror(errno);
		throw std::runtime_error(error);
	}
}


/**
 * Flush, restore and close serial device.
 *
 * @signal ErrorSignal(string)	if there was an error flushing, restoring mode,
 * 								or closing the device.
 */
Serial::~Serial() {
	if (!Flush())
		emit ErrorSignal("Failed to flush serial device");
	
	if (!Restore())
		emit ErrorSignal("Failed to restore serial device mode");
	
	if (close(dev) < 0)
		emit ErrorSignal("Failed to close serial device");
}


/**
 * Send data. Returns when entire buffer is sent or an error occurred. Currently
 * this method sends one byte at a time and then waits for a random byte in
 * return. This is used as a form of flow control. The reason is that when
 * communicating with hardware we need a way to limit the transfer speed to the
 * capabilities of the hardware, most importantly ram/eeprom writing speed.
 *
 * @param data					pointer to data buffer
 * @param dsize					number of bytes to send
 *
 * @throws std::runtime_error	Thrown when there is an error writing to device.
 */
void Serial::Send(const char *data, uint32_t dsize) const {
	/*
	uint32_t i;
	//int res;

	for (i=res=0; i<dsize; i+=res) {
		res = write(dev, data+i, dsize-i);
		if (res < 0)
			throw std::runtime_error("failure writing to serial device");
	}
	*/

	char echo_byte;

	for (unsigned int i=0; i<dsize; ++i) {
		if (write(dev, data+i, 1) < 0)
			throw std::runtime_error("failure writing to serial device");
		if (read(dev, &echo_byte, 1) < 0)
			throw std::runtime_error("failure getting echo from serial device");
	}
}


/**
 * Receive data. Blocks until @a dsize number of bytes received.
 *
 * @param data					pointer to data buffer
 * @param dsize					number of bytes to receive
 *
 * @throws std::runtime_error	Thrown when there is an error reading from dev.
 */
void Serial::Receive(char *data, uint32_t dsize) const {
	uint32_t i;
	int res;

	for (i=res=0; i<dsize; i+=res) {
		res = read(dev, data+i, dsize-i);
		if (res < 0)
			throw std::runtime_error("failure reading from serial device");
	}
}


/**
 * Get the baud rate of the serial device.
 *
 * @exception std::logic_error	This exception should never occur. It is thrown
 * 								when the device somehow entered a mode that is
 * 								not valid according to the <termios.h>
 *
 * @return						baud rate
 */
unsigned int Serial::Baud() const {
	switch(cfgetispeed(&mode)) {
		case B50: return 50;
		case B75: return 75;;
		case B110: return 110;
		case B134: return 134;
		case B150: return 150;
		case B200: return 200;
		case B300: return 300;
		case B600: return 600;
		case B1200: return 1200;
		case B1800: return 1800;
		case B2400: return 2400;
		case B4800: return 4800;
		case B9600: return 9600;
		case B19200: return 19200;
		case B38400: return 38400;
		case B57600: return 57600;
		case B115200: return 115200;
		case B230400: return 230400;
	};
	
	// should be impossible (can happen if there are errors in termios code)
	throw std::logic_error("Device mode corrupt");
}


/**
 * Get the number of data bits.
 *
 * @exception std::logic_error	This exception should never occur. It is thrown
 * 								when the device somehow entered a mode that is
 * 								not valid according to the <termios.h>
 *
 * @return						number of data bits in transfer format, 5-8
 */
unsigned int Serial::DataBits() const {
	unsigned int data_bits;

	switch (mode.c_cflag & CSIZE) {
	case CS5: data_bits = 5; break;
	case CS6: data_bits = 6; break;
	case CS7: data_bits = 7; break;
	case CS8: data_bits = 8; break;
	default:
		// should be impossible (can happen if there are errors in termios code)
		throw std::logic_error("Device mode corrupt");
	}

	return data_bits;
}


/**
 * Get the number of stop bits.
 *
 * @return						number of stop bits in transfer format, 1-2
 */
unsigned int Serial::StopBits() const {
	if (mode.c_cflag & CSTOPB)
		return 2;

	return 1;
}


/**
 * Set the baud rate of the serial device.
 *
 * @throws std::runtime_error	Thrown if the baud rate is not supported.
 *
 * @param baud					baud rate
 */
void Serial::SetBaud(const unsigned int baud) {
	struct termios new_mode;
	speed_t speed;

	// determine baud rate
	switch (baud) {
		case 50: speed = B50; break;
		case 75: speed = B75; break;
		case 110: speed = B110; break;
		case 134: speed = B134; break;
		case 150: speed = B150; break;
		case 200: speed = B200; break;
		case 300: speed = B300; break;
		case 600: speed = B600; break;
		case 1200: speed = B1200; break;
		case 1800: speed = B1800; break;
		case 2400: speed = B2400; break;
		case 4800: speed = B4800; break;
		case 9600: speed = B9600; break;
		case 19200: speed = B19200; break;
		case 38400: speed = B38400; break;
		case 57600: speed = B57600; break;
		case 115200: speed = B115200; break;
		case 230400: speed = B230400; break;
		default: throw std::runtime_error("Invalid baud rate");
	}

	// set input and output baud rates (in practice this should never fail)
	memcpy(&new_mode, &mode, sizeof(struct termios));

	if ((cfsetospeed(&new_mode, speed)<0) || (cfsetispeed(&new_mode, speed)<0))
		throw std::runtime_error("Can't set baud rate");

	// SetMode can fail though
	SetMode(new_mode);
}


/**
 * Set number of data bits.
 * 
 * @throws std::runtime_error	Thrown if the data bit setting is not supported.
 *
 * @param data_bits				number of data bits, 5-8
 */
void Serial::SetDataBits(const unsigned int data_bits) {
	struct termios new_mode;

	// make modification on temporary mode so that SetMode() can undo the
	// change if it fails
	memcpy(&new_mode, &mode, sizeof(struct termios));

	// determine number of data bits
	switch (data_bits) {
		case 5: new_mode.c_cflag = (new_mode.c_cflag & ~CSIZE) | CS5; break;
		case 6: new_mode.c_cflag = (new_mode.c_cflag & ~CSIZE) | CS6; break;
		case 7: new_mode.c_cflag = (new_mode.c_cflag & ~CSIZE) | CS7; break;
		case 8: new_mode.c_cflag = (new_mode.c_cflag & ~CSIZE) | CS8; break;
		default: throw std::runtime_error("Invalid number of data bits");
	}

	// set number of data bits
	SetMode(new_mode);
}


/**
 * Set number of stop bits.
 * 
 * @throws std::runtime_error	Thrown if the stop bit setting is not supported.
 *
 * @param stop_bits				number of stop bits, 1-2
 */
void Serial::SetStopBits(const unsigned int stop_bits) {
	struct termios new_mode;

	// make modification on temporary mode so that SetMode() can undo the
	// change if it fails
	memcpy(&new_mode, &mode, sizeof(struct termios));

	// determine number of stop bits
	switch (stop_bits) {
		case 1: new_mode.c_cflag &= ~CSTOPB; break;
		case 2: new_mode.c_cflag |= CSTOPB; break;
		default: throw std::runtime_error("Invalid number of stop bits");
	}

	// set number of stop bits
	SetMode(new_mode);
}


/**
 * Set serial device mode.
 *
 * @param new_mode				the new device mode
 * 
 * @throws std::runtime_error	This exception is thrown if the specific device
 * 								configuration mode is not supported.
 *
 * It seems that the issue os setting the device mode without using cfmakeraw()
 * keeps reappearing. As far as I can tell it depend on whether cfmakeraw() was
 * run previously in which case subsequent runs of the program work even without
 * it. On a fresh boot the mode will not be set properly without cfmakeraw().
 */
void Serial::SetMode(struct termios &new_mode) {
	// set default mode parameters
	new_mode.c_iflag &= ~IGNBRK; // don't ignore break condition on input
	new_mode.c_iflag &= ~BRKINT; // don't send SIGINT on break condition
	new_mode.c_iflag &= ~PARMRK; // if prt/frm check off & err => read null byte
	new_mode.c_iflag &= ~ISTRIP; // don't strip off eight bit
	new_mode.c_iflag &= ~INLCR;  // don't translate NL to CR on input
	new_mode.c_iflag &= ~IGNCR;  // don't ignore carriage return on input
	new_mode.c_iflag &= ~ICRNL;  // don't translace CR to NL on input
	new_mode.c_iflag &= ~IXON;   // disable XON/XOFF flow control on output

	new_mode.c_oflag &= ~OPOST;  // disable implementation defined output procc.

	new_mode.c_lflag &= ~ECHO;
	new_mode.c_lflag &= ~ECHONL;
	new_mode.c_lflag &= ~ICANON;
	new_mode.c_lflag &= ~ISIG;
	new_mode.c_lflag &= ~IEXTEN;

	new_mode.c_cflag &= ~PARENB; // disable parity generation / checking
	
	// set mode
	if (tcsetattr(dev, TCSADRAIN, &new_mode))
		throw std::runtime_error("Can't set device mode");

	struct termios temp_mode;
	tcgetattr(dev, &temp_mode);	// if this fails then compare also likely? fails
	if (!TermiosCompare(temp_mode, new_mode))
		throw std::runtime_error("Can't set device mode");

	// update local copy of device mode
	memcpy(&mode, &new_mode, sizeof(struct termios));
}


/**
 * Save serial device mode and update the current mode. This method should only
 * be called once, from the constructor.
 *
 * @return						True on success, false on failure.
 */
bool Serial::Save() {
	if (tcgetattr(dev, &saved_mode) < 0)
		return false;

	memcpy(&mode, &saved_mode, sizeof(struct termios));
	return true;
}


/**
 * Restore serial device mode. This method should only be called once, from the
 * destructor.
 *
 * @return						True on success, false on failure.
 */
bool Serial::Restore() {
	// restore device mode
	struct termios tmp_mode;
	//if (tcsetattr(dev, TCSAFLUSH, &saved_mode) < 0)
	if (tcsetattr(dev, TCSADRAIN, &saved_mode) < 0)
		return false;

	if (tcgetattr(dev, &tmp_mode) < 0)
		return false;

	if (!TermiosCompare(tmp_mode, saved_mode))
		return false;

	return true;
}


/**
 * Discard incoming and outgoing data that is still in the serial port buffers.
 *
 * @return						True on success, false on failure.
 */
bool Serial::Flush() const {
	if (tcflush(dev, TCIOFLUSH) < 0)
		return false;

	return true;
}


/**
 * Compare termios structures in a structure-correct and portable way. This
 * function tries to be portable and thus may not compare all structure fields.
 *
 * @param term1					first termios structure
 * @param term2					second termios structure
 *
 * @return						True if term1 is equal to term2, false otherwise
 */
bool Serial::TermiosCompare(const struct termios &term1, const struct termios &term2) const {
	// compare input, output, control and local mode flags
	if (term1.c_iflag != term2.c_iflag || term1.c_oflag != term2.c_oflag ||
		term1.c_cflag != term2.c_cflag || term1.c_lflag != term2.c_lflag)
		return false;

	// compare input and output speeds
	if (cfgetispeed(&term1) != cfgetispeed(&term2) ||
		cfgetospeed(&term1) != cfgetospeed(&term2))
		return false;

	// compare control characters
	for (int i=0; i<NCCS; i++)
		if (term1.c_cc[i] != term2.c_cc[i])
			return false;

	return true;
}
