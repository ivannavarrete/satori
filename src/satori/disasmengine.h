
#ifndef DISASMENGINE_H
#define DISASMENGINE_H


#include "instruction.h"


/**
 * The DisasmEngine class provides an abstract disassembly interface that the
 * different architecture modules must implement.
 */
class DisasmEngine {
public:
	/**
	 * There is no need for a virtual destructor in an interface class but
	 * some compilers generate warnings without it.
	 */
	virtual ~DisasmEngine() {}

	/**
	 * Disassemble memory and append result to a list of disassembled
	 * instructions.
	 *
	 * @param data					data to disassemble
	 * @param data_size				size of data buffer
	 * @param addr					address of the first instruction
	 * @param instruction_list		list to append disassembled instructions to
	 *
	 * @returns						Number of bytes disassembled.
	 */
	virtual unsigned int Disassemble(const char *data, unsigned int data_size,
				uint32_t addr, Instruction::List *instruction_list) const = 0;
};


#endif
