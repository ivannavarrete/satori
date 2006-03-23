
#ifndef AVRDISASMENGINE_H
#define AVRDISASMENGINE_H


#include <boost/shared_ptr.hpp>
#include "avrinstruction.h"
#include "satori/disasmengine.h"


/**
 * This class provides an AVR implementation of the DisasmEngine abstract class.
 * It is used to disassemble memory into AVR instructions.
 */
class AvrDisasmEngine : public DisasmEngine {
public:
	AvrDisasmEngine::AvrDisasmEngine();
	virtual unsigned int Disassemble(const char *data, unsigned int data_size,
					uint32_t addr, Instruction::List *instruction_list) const;

private:
	void DisassembleInstruction(const char *data,unsigned int data_size,
		uint32_t addr, boost::shared_ptr<AvrInstruction> &instruction) const;

	static AvrInstruction opcode_table[];
	AvrInstruction *invalid_opcode_table;
};


#endif
