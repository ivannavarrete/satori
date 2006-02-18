
#include <QCoreApplication>
#include <QFile>
#include <exception>
#include <stdexcept>
#include <sstream>
#include "avrdeviceinfo.h"


/**
 * Parse the device file specified by @a device_name. The device information
 * can then be queried with the other methods in this class.
 *
 * @param device_name
 *
 * @throws std::runtime_error	Thrown if the device is not supported or if
 * 								there was an error loading the device info.
 * 
 * @Todo Change to another (custom, XML) exception.
 */
AvrDeviceInfo::AvrDeviceInfo(const std::string &device_name) {
	ParseDeviceFile(QString(device_name.c_str()));
}


/**
 * Extract the start and end addresses of a memory segment. If the method fails
 * the @a start_addr and @a end_addr are undefined.
 *
 * @param mtype					type of AVR memory
 * @param start_addr			result parameter to hold the start address
 * @param end_addr				result parameter to hold the end address
 *
 * @throws std::runtime_error	Thrown if the memory type is unknown, the
 * 								memory info is not in the file, or if the
 * 								memory info was corrupt (wrong XML syntax).
 * 
 * @Todo Change to another (custom, XML) exception.
 */
void AvrDeviceInfo::Memory_(Memory::Type mtype, uint32_t *start_addr, uint32_t *end_addr) {
	QString memory_name;

	switch (mtype) {
	case Memory::SRAM: memory_name = "SRAM"; break;
	case Memory::EEPROM: memory_name = "EEPROM"; break;
	case Memory::FLASH: memory_name = "FLASH"; break;
	default:
		throw std::runtime_error("unsupported memory type");
	}

	// find the Memory tag and extract its start and end attributes
	QDomElement child = document.documentElement().firstChildElement("Memory");
	while (!child.isNull()) {
		if (child.attribute("name") == memory_name) {
			bool ok_start;
			bool ok_end;
			*start_addr = child.attribute("start").toInt(&ok_start, 16);
			*end_addr = child.attribute("end").toInt(&ok_end, 16);

			if (ok_start && ok_end)
				return;
			else
				throw std::runtime_error("memory info corrupt");
				
		}

		child = child.nextSiblingElement("Memory");
	}

	throw std::runtime_error("memory info not found");
}


/**
 * Extract the device state map.
 *
 * @param state_map				result parameter to hold the state map
 *
 * @throws std::runtime_error	Thrown if the memory type is unknown, the
 * 								memory info is not in the file, or if the
 * 								memory info was corrupt (wrong XML syntax).
 * 
 * @Todo Change to another (custom, XML) exception.
 */
void AvrDeviceInfo::StateMap(std::vector<StateEntry> *state_map) {
	// Todo: Figure out whether we want to cheat (not read XML file), or
	// if we should keep the state map in every device file. The advantage
	// of creating a hardcoded state map is that the information is not needed
	// in the XML device file. This is a good thing since it is likely to be
	// the same for all AVR devices.
	//
	// Remember that the order of creation of the state map is important and
	// should mirror the state structure in monitor.asm
	state_map->clear();

	for (unsigned int i=0; i<32; i++) {
		std::ostringstream reg_name;
		reg_name << "r" << i;
		state_map->push_back(StateEntry(reg_name.str(), 1));
	}

	state_map->push_back(StateEntry("PC", 2));
	state_map->push_back(StateEntry("SP", 2));
	state_map->push_back(StateEntry("SREG", 2));
}

/**
 * Parse the device file specified by @a device_name. The device information
 * can then be queried with the other methods in this class.
 *
 * @param device_name
 *
 * @throws std::runtime_error	Thrown if the device is not supported or if
 * 								there was an error loading the device info.
 * 
 * @Todo Change to another (custom, XML) exception.
 */
void AvrDeviceInfo::ParseDeviceFile(const QString &device_name) {
	QString file_name = QCoreApplication::applicationDirPath() +
			"/../modules/arch/avr/device/" + device_name.toLower() + ".xml";

	QFile device_file(file_name);
	if (!device_file.open(QFile::ReadOnly | QFile::Text)) {
		throw std::runtime_error("unable to open device file");
	}

	QString error_string;
	int error_row;
	int error_col;

	// do the actual parsing
	if (!document.setContent(&device_file, true, &error_string,
										&error_row, &error_col)) {
		device_file.close();	// necessary? this object will destruct anyway
		throw std::runtime_error("error parsing device file");
	}
	device_file.close();

	// check that the file is a device file and containing the requested device
	QDomElement root = document.documentElement();
	if (root.tagName() != "Device" ||
		root.attribute("name").toLower() != device_name.toLower()) {
		throw std::runtime_error("invalid device file");
	}
}
