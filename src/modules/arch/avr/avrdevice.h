
#ifndef AVRDEVICE_H
#define AVRDEVICE_H


#include <string>
#include <boost/shared_ptr.hpp>
#include "satori/memory.h"
#include "satori/state.h"


/**
 * This class represents a high level model of an AVR device. It does not have
 * much functionality itself but instead serves as a contaner for the device
 * subsystems (memory, io, state, etc). It also has the ability to 
 */
class AvrDevice {
public:
	AvrDevice(const std::string &device_name);
	void SetComm(boost::shared_ptr<Comm> comm);
	
	const std::string &Name() const { return name; }
	const boost::shared_ptr<Memory> Sram() const { return sram; }
	const boost::shared_ptr<Memory> Eeprom() const { return eeprom; }
	const boost::shared_ptr<Memory> Flash() const { return flash; }
	const boost::shared_ptr<State> State_() const { return state; }

private:
	std::string name;

	boost::shared_ptr<Memory> sram;
	boost::shared_ptr<Memory> eeprom;
	boost::shared_ptr<Memory> flash;
	//boost::shared_ptr<Io> io;
	boost::shared_ptr<State> state;

	boost::shared_ptr<CommandEngine> command_engine;
};


#endif
