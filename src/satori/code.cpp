
#include "code.h"


/**
 * Create a code object as a decorator of an existing memory.
 *
 * @param memory				the memory to use as code
 * @param disasm_engine			algorithm used for disassembly
 */
Code::Code(boost::shared_ptr<Memory> memory, boost::shared_ptr<DisasmEngine> disasm_engine /*, invalid_opcode list */) {
	this->memory = memory;
	this->disasm_engine = disasm_engine;
}


/**
 * Disassemble memory. If the @a start_addr and @a end_addr are out of bounds
 * the disassembly will be clipped.
 *
 * @param start_addr			start address, in bytes
 * @param end_addr				end address, in bytes
 *
 * @return						A list of disassembled instruction objects.
 */
const DInstruction::List &Code::Disassemble(uint32_t start_addr, uint32_t end_addr) {
	disasm_list.clear();

	// check arguments for correctness
	if ((start_addr > end_addr) ||
		(start_addr > memory->End()) ||
		(end_addr < memory->Start()))
		return disasm_list;

	start_addr = std::max(start_addr, memory->Start());
	end_addr = std::min(end_addr, memory->End());

	// the data buffer should at least be large enough to hold the largest
	// instruction, otherwise those instructions that don't fit would become
	// undisassembleable(?)
	uint32_t DATA_BUF_SIZE  = 0x4000;		// 16k
	char data[DATA_BUF_SIZE];

	uint32_t data_size = end_addr - start_addr + 1;
	uint32_t addr = start_addr;

	// read and disassemble data from memory, in chunks
	while (data_size > 0) {
		unsigned int chunk_size = std::min(data_size, DATA_BUF_SIZE);
		
		memory->Read(addr, addr+chunk_size, data);
		chunk_size = disasm_engine->Disassemble(data, chunk_size, addr,
														&disasm_list);

		if (chunk_size == 0)
			break;

		data_size -= chunk_size;
		addr += chunk_size;
	}

	return disasm_list;
}
