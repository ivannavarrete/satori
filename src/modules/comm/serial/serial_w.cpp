

#include <winbase.h>		// CreateFile(), CloseHandle(), ...

#include "serial_w.h"


/**
 * Open serial port.
 *
 * @param devname	name of the serial port
 */
Serial::Serial(const string &devname) {
	dev = CreateFile(devname.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0,
					OPEN_EXISTING, 0, NULL); // FILE_ATTRIBUTE_NORMAL inst.of 0

	if(dev == INVALID_HANDLE_VALUE)
		/// @todo Handle the error properly (log and continue).
		throw "Serial::Serial():CreateFile() - nonfatal";
}


/**
 * Restore and close serial port.
 */
Serial::~Serial() {
	//if (Flush() < 0)
	//	/// @todo Handle the error properly (log and continue).
	//	cout << "Serial():~Serial():Flush()\n";

	if (Restore() < 0)
		/// @todo Handle the error properly (log and continue).
		cout << "Serial():~Serial():Restore()\n";

	if (CloseHandle(dev) == FALSE)
		/// @todo Handle the error properly (log and continue).
		cout << "Serial:~Serial():CloseHandle()\n";
}


/**
 * Configure serial port.
 *
 * @param baud		baud rate as defined in serial.h
 * @param dbits		number of data bits, 5-8
 * @param sbits		number of stop bits, 1-2
 *
 * @return			0 on success, -1 on failure
 */
int Serial::Config(int baud, int dbits, int sbits) {
	COMMCONFIG term;
	DWORD term_size;
	int speed;
	BYTE sbitsf;

	// determine baud rate.. not all baud rates have predefined constants
	switch (baud) {
		case 110: speed = CBR_110; break;
		case 300: speed = CBR_300; break;
		case 600: speed = CBR_600; break;
		case 1200: speed = CBR_1200; break;
		case 2400: speed = CBR_2400; break;
		case 4800: speed = CBR_4800; break;
		case 9600: speed = CBR_9600; break;
		case 14400: speed = CBR_14400; break;
		case 19200: speed = CBR_19200; break;
		case 38400: speed = CBR_38400; break;
		case 56000: speed = CBR_56000; break;
		case 57600: speed = CBR_57600; break;
		case 115200: speed = CBR_115200; break;
		case 128000: speed = CBR_128000; break;
		case 256000: speed = CBR_256000; break;
		default: break;
	}

	// determine number of stop bits
	switch (sbits) {
		case 1: sbitsf = ONESTOPBIT; break;
		case 2: sbitsf = TWOSTOPBITS; break;
		default: return -1;
	}

	// get current mode
	if (GetCommConfig(hdl, &term, &term_size) == FALSE)
		return -1;

	// configure comm port mode
	term.dwProviderSubType = PTS_RS232;	// rs232 sub type
	term.dwProviderOffset = 0;			// no provider-specific data for rs232
	term.dwProviderSize = 0;

	term.DBCLength = sizeof(DCB);		// is this necessary?
	term.dcb.BaudRate = speed;			// baud rate
	term.dcb.fBinary = TRUE;			// use binary data transfer
	term.dcb.fParity = FALSE;			// disable parity check .. maybe TRUE ?
	term.dcb.fOutxCtsFlow = FALSE;		// Clear To Send flow control
	term.dcb.fOutxDsrFlow = FALSE;		// Data Set Ready flow control
	term.dcb.fDtrControl = DTR_CONTROL_DISABLE; // don't use DTR flow control
	term.dcb.fDsrSensitivity = FALSE;	// don't ignore data even if DSR not set
	term.dcb.fTXContinueOnXoff = TRUE;	// doesn't matter, we don't use XON/XOFF
	term.dcb.fOutX = FALSE;				// XON/XOFF flow control
	term.dcb.fInX = FALSE;				// XON/XOFF flow control
	term.dcb.fErrorChar = FALSE;		// don't use error replacement
	term.dcb.fNull = FALSE;				// don't discard null bytes
	term.dcb.fRtsControl = RTS_CONTROL_DISABLE; // don't use RTS flow control
	term.dcb.fAbortOnError = FALSE;		// abort read/write on error
	term.dcb.ByteSize = dbits;			// number of data bits
	term.dcb.Parity = NOPARITY;			// don't use parity
	term.dcb.StopBits = sbitsf;			// number of stop bits
	
	// set new mode
	if (SetCommConfig(dev, &term, &term_size) == FALSE)
		return -1;

	/// @todo Should probably set communication timeouts, with
	/// SetCommTimeouts() or something similar.

	return 0;
}


/**
 * Send data. Returns when entire buffer is sent or an error occurred.
 *
 * @param data		pointer to data buffer
 * @param dsize		number of bytes to send
 *
 * @return			0 on success, -1 on failure
 */
int Serial::Send(const uint8_t *data, const uint32_t dsize) {
	DWord nwritten;

	if (WriteFile(dev, (LPCVOID)data, (DWORD)dsize, &nwritten, NULL) == FALSE)
		return -1;

	// should not be possible since dev is not opened with FILE_FLAG_OVERLAPPED
	if (nwritten != (DWORD)dsize)
		return -1;

	return 0;
}


/**
 * Receive data. Blocks until @a dsize number of bytes received.
 *
 * @param data		pointer to data buffer
 * @param dsize		number of bytes to receive
 *
 * @return			0 on success, -1 on failure
 */
int Serial::Receive(uint8_t *data, const uint32_t dsize) {
	DWORD nread;

	if (ReadFile(dev, (LPVOID)data, (DWORD)dsize, &nread, NULL) == FALSE)
		return -1;

	// should not be possible since dev is not opened with FILE_FLAG_OVERLAPPED
	if (nread != (DWORD)dsize)
		return -1;

	return 0;
}


/**
 * Save serial port mode. For each call the previous saved mode is overwritten.
 *
 * @return			0 on success, -1 on failure
 */
int Serial::Save() {
	DWORD size = sizeof(saved_mode);
	
	if (GetCommConfig(dev, &saved_mode, &size) == FALSE)
		return -1;

	return 0;
}


/**
 * Restore serial port mode.
 *
 * @return			0 on success, -1 on failure
 */
int Serial::Restore() {
	DWORD size = sizeof(saved_mode);
	
	if (SetCommConfig(dev, &saved_mode, &size) == FALSE)
		return -1;

	return 0;
}


/*
int Serial::Flush() {
	// << not implemented >>
}
*/
