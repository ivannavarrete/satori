
#ifndef CODE_H
#define CODE_H


#include "disasmengine.h"
#include "dinstruction.h"
#include "memory.h"


/**
 * The code class is a sort of decorator of a memory. It adds disassembly
 * capabilities.
 */
class Code {
public:
	Code(boost::shared_ptr<Memory> memory,
		 boost::shared_ptr<DisasmEngine> disasm_engine);

	const DInstruction::List &Disassemble(uint32_t start_addr,
											uint32_t end_addr);

private:
	boost::shared_ptr<Memory> memory;				///< memory to disassemble
	boost::shared_ptr<DisasmEngine> disasm_engine;	///< engine to use

	DInstruction::List disasm_list;		///< list of disassembled instructions
};


#endif
