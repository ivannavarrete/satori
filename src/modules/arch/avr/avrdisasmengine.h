
#ifndef AVRDISASMENGINE_H
#define AVRDISASMENGINE_H


#include <vector>
#include "satori/disasmengine.h"
#include "satori/dinstruction.h"


/**
 * This class provides an AVR implementation of the DisasmEngine abstract class.
 * It is used to disassemble memory into AVR instructions.
 */
class AvrDisasmEngine : public DisasmEngine {
public:
	AvrDisasmEngine::AvrDisasmEngine();
	virtual unsigned int Disassemble(const char *data, unsigned int data_size,
						uint32_t addr, DInstruction::List *disasm_list) const;

private:
	unsigned int DisassembleInstruction(const char *data,unsigned int data_size,
								uint32_t addr, DInstruction *instruction) const;

	struct Opcode *invalid_opcode_table;
};


#endif
