
#ifndef AVRDEVICEINFO_H
#define AVRDEVICEINFO_H


#include <QDomDocument>
#include <QString>
#include <string>
#include "satori/memory.h"
#include "satori/state.h"


/**
 * This class holds all the passive information to represent an AVR device.
 * The raw information is stored on disk in XML format. In essence, this class
 * encapsulates the XML parsing and presents an interface for accessing the
 * device info.
 *
 * A recommended use is to create an AvrDeviceInfo object, extract all the
 * needed information and then destroy the object. In this way, the possible
 * exceptions thrown by this class can be confined to a small code area.
 *
 * Implementation note: One way to reimplement this class is to parse the
 * device file and store the information in local variables. Then the
 * information retrieval methods could read theese, instead of interpreting
 * the XML tree. Then the only method to generate exceptions would be the
 * parse method, and not the information retrieval methods. In this way it
 * would be acceptable to keep an object of this class for longer periods of
 * time since the error checking code will still be confined to a small area.
 * Currently there is no need to keep this object around, so I don't use this
 * implementation. This leads to slightly more readable code (each information
 * retrieval method knows the XML syntax of only the information it needs),
 * and to a cleaner class since we don't need to retain the device information.
 */
class AvrDeviceInfo {
public:
	AvrDeviceInfo(const std::string &device_name);
	void Memory_(Memory::Type mtype, uint32_t *start_addr,
								uint32_t *end_addr) const;
	void StateMap(std::vector<StateEntry> *state_map) const;

private:
	void ParseDeviceFile(const QString &device_name);

	QDomDocument document;
};


#endif
