
#include <stdexcept>
#include "memory.h"
#include "commandengine.h"


/**
 * Create a memory object of a certain type.
 * 
 * @param mtype					memory type
 * @param start					start address
 * @param end					end address
 * @param command_engine		command engine implementing all hw commands
 *
 * @throws std::logic_error		Thrown if the start and end parameters are
 * 								invalid.
 */
Memory::Memory(const Type memory_type, const uint32_t start, const uint32_t end, boost::shared_ptr<CommandEngine> command_engine) {
	if (start >= end)
		/// @todo Handle error properly.
		throw std::logic_error("invalid memory object");

	// init memory parameters
	this->memory_type = memory_type;
	this->start = start;
	this->end = end;
	this->command_engine = command_engine;

/*
	// allocate memory for data cache
	cache_page_size = 512;
	cache_start = start;
	cache_end = cache_start + cache_page_size * 3;
	if (cache_end > end)
		cache_end = end;						// entire memory is cached
	cache = new char[cache_end - cache_start + 1];
*/
}


/**
 * Destroy memory object.
 */
Memory::~Memory() {
/*
	delete [] cache;
*/
}


/**
 * Read target memory. This function reads data from the internal cache if
 * possible.
 * 
 * Note: Ideally we should take precations not to read more data than fits
 * in the cache.
 *
 * @param start_addr	start address in memory
 * @param end_addr		end address in memory
 * @param data			destination buffer, must be at least
 * 						end_addr-start_addr+1 bytes
 *
 * @return				number of bytes read, or 0 on error
 */
uint32_t Memory::Read(uint32_t start_addr, uint32_t end_addr, char *data) {
	// check arguments for correctness
	if ((start_addr > end_addr) || (start_addr > end) || (end_addr < start))
		return 0;

	if (start_addr < start)
		start_addr = start;

	if (end_addr > end)
		end_addr = end;

	// create some convenience variables
	uint32_t data_size = end_addr - start_addr + 1;
	//uint32_t cache_size = cache_end - cache_start + 1;

	/// @todo Implement caching, instead of always reading from the programmer.
	command_engine->ReadMemory(memory_type, start_addr, data, data_size);
	return data_size;

/*
	// case 1: return data completely from target
	// if the data we need to read is larger than the cache, we read it all
	// from the target memory, updating the cache in the process.. as stated
	// above, this situation should be avoided
	if (start_addr < cache_start && end_addr > cache_end) {
		/// @todo Handle error properly.
		cmd->ReadMemory(memory_type, start_addr, data, data_size);
		memcpy(cache, data, cache_size);
		return data_size;
	}

	// case 2: return data completely from cache
	if (start_addr >= cache_start && end_addr <= cache_end) {
		memcpy(data, cache, data_size);
		return data_size;
	}

	// case 3: return data either from cache and target (overlap), or from
	// target only (no overlap)
	uint32_t icache_size = cache_size - data_size;
	uint32_t icache_front_size = icache_size / 2;
	uint32_t icache_back_size = icache_size / 2;
	if (icache_size%2 != 0)
		icache_back_size += 1;

	uint32_t new_cache_start = start_addr - icache_front_size;
	uint32_t new_cache_end = end_addr + icache_back_size;

	if (new_cache_start < start) {
		new_cache_end += start - new_cache_start;
		new_cache_start = start;
	}

	if (new_cache_end > end) {
		new_cache_start -= new_cache_end - end;
		new_cache_end = end;
	}

	// new cache does not overlap old cache, so reload all of it
	if (new_cache_end < cache_start || new_cache_start > cache_end) {
		/// @todo Handle error properly.
		cmd->ReadMemory(memory_type, new_cache_start, cache, cache_size);

	// new cache end overlaps into old cache beginning
	} else if (new_cache_end >= cache_start) {
		uint32_t overlap_size = new_cache_end - cache_start + 1;
		memmove(cache+cache_size-overlap_size, cache, overlap_size);
		/// @todo Handle error properly.
		cmd->ReadMemory(memory_type, new_cache_start, cache,
									cache_size-overlap_size);

	// new cache beginning overlaps into old cache end
	} else {
		uint32_t overlap_size = cache_end - new_cache_start + 1;
		memmove(cache, cache+cache_size-overlap_size, overlap_size);
		/// @todo Handle error properly.
		cmd->ReadMemory(memory_type, new_cache_start, cache+overlap_size,
												cache_size-overlap_size);
	}

	memcpy(data, cache, data_size);
		
	cache_start = new_cache_start;
	cache_end = new_cache_end;

	return data_size;
*/
}


/**
 * Write target memory.
 * 
 * @param start_addr	sag
 * @param end_addr		asdf
 * @param data			asdasd
 *
 * @return				dunno
 */
int Memory::Write(uint32_t start_addr, uint32_t end_addr, const char *data) {
	// check arguments for correctness
	if ((start_addr > end_addr) || (end_addr > end) || (start_addr < start))
		return 0;

	if (start_addr < start)
		start_addr = start;

	if (end_addr > end)
		end_addr = end;

	// write to target memory
	uint32_t data_size = end_addr - start_addr + 1;
	command_engine->WriteMemory(memory_type, start_addr, data, data_size);
	return data_size;

/*
	/// @todo Implement caching instead of always writing to programmer.
	// update cache if necessary
	if (start_addr > cache_end || end_addr < cache_start)
		return data_size;

	uint32_t update_start_addr = std::max(start_addr, cache_start);
	uint32_t update_end_addr = std::min(end_addr, cache_end);
	uint32_t update_size = update_end_addr - update_start_addr + 1;
	uint32_t update_offset = std::max(start_addr, cache_start) -
								std::min(start_addr, cache_start);

	cmd->ReadMemory(memory_type, update_start_addr, cache+update_offset,
															update_size);

	return data_size;
*/
}


/**
 * Search memory.
 *
 * @param start_addr	sdf
 * @param end_addr		afdsg
 * @param data			asdldsf
 * @param dsize			sdlk
 *
 * @return				dunno
 */
/*
uint32_t Memory::Search(uint32_t start_addr, uint32_t end_addr, const char *data, const uint8_t dsize) {
	///  @todo Implement search method

	// supress compilation errors until function is implemented
	start_addr = end_addr = dsize;
	data = data;

	// if search string is bigger than memory
	// ...

	return 0;
}
*/
