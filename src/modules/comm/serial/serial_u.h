
#ifndef SERIAL_U_H
#define SERIAL_U_H


#include <string>
#include <cstring>
#include <QObject>

#include <termios.h>
#include <stdint.h>

#include "../comm.h"


/**
 * The Serial class provides communication facilities to talk to the programmer
 * through a serial interface. This class implements the unix version.
 */
class Serial : public QObject, public Comm {
	Q_OBJECT

public:
	Serial(const std::string &device_name);
	virtual ~Serial();
	
	virtual void Send(const char* data, uint32_t dsize) const;
	virtual void Receive(char *data, uint32_t dsize) const;

	unsigned int Baud() const;
	unsigned int DataBits() const;
	unsigned int StopBits() const;
	void Baud(const unsigned int baud);
	void DataBits(const unsigned int data_bits);
	void StopBits(const unsigned int stop_bits);

signals:
	void ErrorSignal(std::string cause); ///< signal for error reporting

private:
	void SetMode(struct termios &new_mode);
	bool Save();
	bool Restore();
	bool Flush() const;
	bool TermiosCompare(const struct termios &term1,
						const struct termios &term2) const;

	std::string device_name;			///< name of the serial device file
	int dev;							///< device file descriptor
	struct termios mode;				///< current device mode
	struct termios saved_mode;			///< original device mode
};


#endif
