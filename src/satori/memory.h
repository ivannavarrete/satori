
#ifndef MEMORY_H
#define MEMORY_H


#include <inttypes.h>
#include <boost/shared_ptr.hpp>

class CommandEngine;


/**
 * Generic memory class.
 */
class Memory {
public:
	/// Memory types. The numerical values must match those in the monitor code.
	enum Type { SRAM=1, EEPROM=2, FLASH=3 };

	Memory(const Type memory_type, const uint32_t start, const uint32_t end,
							boost::shared_ptr<CommandEngine> command_engine);

	uint32_t Read(uint32_t start_addr, uint32_t end_addr, char *data);
	int Write(uint32_t start_addr, uint32_t end_addr, const char *data);
	//uint32_t Search(const uint32_t start_addr, const uint32_t end_addr,
	//							const char *data, const uint8_t dsize);

	uint32_t Start() { return start; }
	uint32_t End() { return end; }

private:
	Type memory_type;

	boost::shared_ptr<CommandEngine> command_engine;

	uint32_t start;				///< first valid address
	uint32_t end;				///< last valid address

	/*
	uint32_t cache_start;
	uint32_t cache_end;
	uint32_t cache_page_size;
	char *cache;
	*/
};


#endif
