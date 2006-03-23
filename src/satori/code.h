
#ifndef CODE_H
#define CODE_H


#include "disasmengine.h"
#include "instruction.h"
#include "memory.h"


/**
 * The code class is a sort of decorator of a memory. It adds disassembly
 * capabilities.
 *
 * Note: We keep the disassembly list in this class instead of in a view
 *		 class so that we can add caching capabilities and such later on.
 */
class Code {
public:
	Code(boost::shared_ptr<Memory> memory,
		 boost::shared_ptr<DisasmEngine> disasm_engine);

	const Instruction::List &Disassemble(uint32_t start_addr,uint32_t end_addr);

private:
	boost::shared_ptr<Memory> memory;				///< memory to disassemble
	boost::shared_ptr<DisasmEngine> disasm_engine;	///< engine to use

	Instruction::List instruction_list;		///< list of decoded instructions
};


#endif
