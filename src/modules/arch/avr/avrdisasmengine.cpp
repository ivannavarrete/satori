
#include <netinet/in.h>	/// @Todo Replace with platform-independent solution
#include <sstream>
#include <stdexcept>
#include "avrdisasmengine.h"
#include "opcodetable.h"



/**
 * Initialize opcode table. More specifically, convert the 16-bit opcode and
 * mask values in opcode table from undetermined endianess to big endian.
 *
 * The reason for this is that the table is easier to read and compare to
 * datasheet in it's current form, but it needs to be converted to a
 * determined form for comparison to the data taken from memory.
 */
AvrDisasmEngine::AvrDisasmEngine() {
	struct Opcode *op_entry = opcode_table;

	while (op_entry->mnemonic) {
		op_entry->opcode = htons(op_entry->opcode);
		op_entry->mask = htons(op_entry->mask);
		op_entry++;
	}
}


/**
 * Disassemble memory and append result to a list of disassembled instructions.
 *
 * @param disasm_list			list to append disassembled instructions to
 * @param data					data to disassemble
 * @param data_size				size of data buffer
 * @param addr					address of the first instruction
 *
 * @returns						Number of bytes disassembled.
 *
 * @throws std::logic_error		Thrown if the opcode table is corrupt.
 */
unsigned int AvrDisasmEngine::Disassemble(const char *data, unsigned int data_size, uint32_t addr, DInstruction::List *disasm_list) const {
	/* we need at least 16 bits of data to possibly dissasemble an instr */
	if (data == NULL || data_size < 2)
		return 0;

	uint32_t data_left = data_size;
	while (true) {
		DInstruction instruction;

		int instruction_size =
				DisassembleInstruction(data, data_left, addr, &instruction);

		if (instruction_size == 0)
			return data_size - data_left;

		disasm_list->push_back(instruction);

		data += instruction_size;
		data_left -= instruction_size;
		addr += instruction_size;
	}
}


/**
 * Disassemble one instruction.
 *
 * @param data					data to disassemble
 * @param data_size				size of data buffer
 * @data addr					address of this instruction
 * @data dinstruction			structure to fill with the disassembled info
 *
 * @returns						Lenght in bytes of the disassembled instruction,
 * 								or 0 if there is not enough data to disassemble
 * 								the instruction.
 *
 * @throws std::logic_error		Thrown if the opcode table is corrupt.
 */
unsigned int AvrDisasmEngine::DisassembleInstruction(const char *data, unsigned int data_size, uint32_t addr, DInstruction *instruction) const {
	/* find the opcode in table */
	uint16_t raw_opcode = reinterpret_cast<const uint16_t *>(data)[0];
	uint16_t opcode = htons(raw_opcode);
	struct Opcode *opcode_entry = opcode_table;

	while (opcode_entry->mnemonic) {
		if ((opcode & opcode_entry->mask) == opcode_entry->opcode)
			break;

		opcode_entry++;
	}
	
	/* return if opcode is found but is to large to disassemble completely */
	if (opcode_entry->size*2 > data_size)
		return 0;

	// @Bug Possible endianness problem with raw_opcode. In the definition of
	//		raw_opcode above we need to convert it from little-endian (AVR)
	//		to 'host'-endian so that the below manipulations will be correct.

	/* extract first operand */
	uint32_t operand1 = 0;
	switch (opcode_entry->op1_type) {
	case none:
		break;
	case OP_R2:
		operand1 = ((raw_opcode >> 3) & 0x06) | 0x18;		/* OK */
		break;
	case OP_R3:
		operand1 = ((raw_opcode >> 4) & 0x07) + 16;			/* OK */
		break;
	case OP_R4:
		operand1 = ((raw_opcode >> 4) & 0x0F) + 16;			/* OK */
		break;
	case OP_R4_l:
		operand1 = (raw_opcode >> 3) & 0x1E;
		break;
	case OP_R5:
		operand1 = (raw_opcode >> 4) & 0x1F;				/* OK */
		break;
	case OP_A5:
		operand1 = (raw_opcode >> 3) & 0x1F;
		break;
	case OP_A6:
		operand1 = ((raw_opcode >> 5) & 0x30) | (raw_opcode & 0x0F);  /*XXX*/
		break;
	case OP_k7:
		operand1 = ((raw_opcode >> 3) & 0x7F) + 1;
		break;
	case OP_k12:
		operand1 = (raw_opcode & 0xFFF) + 1;
		break;
	case OP_k22:
		operand1 = ((raw_opcode & 0x01) << 16) | ((raw_opcode & 0x01F0) << 13) |
					((uint16_t *)data)[1];
		break;
	case OP_q6:
		operand1 = ((raw_opcode & 0x2000) >> 8) | ((raw_opcode & 0xC00) >> 7) |
					(raw_opcode & 0x07);
		break;
	default:
		throw std::logic_error("corrupt opcode table");
	}

	/* extract second operand */
	uint32_t operand2 = 0;
	switch (opcode_entry->op2_type) {
	case none:
		break;
	case OP_R3:
		operand2 = (raw_opcode & 0x07) + 16;			/* OK */
		break;
	case OP_R4:
		operand2 = (raw_opcode & 0x0F) + 16;
		break;
	case OP_R4_l:
		operand2 = (raw_opcode << 1) & 0x1E;
		break;
	case OP_R5:
		operand2 = ((raw_opcode >> 5) & 0x10) | (raw_opcode & 0x0F);	/* OK */
		break;
	case OP_R5_r:
		operand2 = (raw_opcode >> 4) & 0x1F;
		break;
	case OP_K6:
		operand2 = ((raw_opcode & 0xC0) >> 2) | (raw_opcode & 0x0F);	/* OK */
		break;
	case OP_K8:
		operand2 = ((raw_opcode & 0xF00) >> 4) | (raw_opcode & 0x0F);
		break;
	case OP_A6:
		operand2 = ((raw_opcode & 0x600) >> 5) | (raw_opcode & 0x0F);
		break;
	case OP_b3:
		operand2 = raw_opcode & 0x07;
		break;
	case OP_k16:
		operand2 = ((uint16_t *)data)[1];
		break;
	case OP_q6:
		operand2 = ((raw_opcode & 0x2000) >> 8) | ((raw_opcode & 0xC00) >> 7) |
					(raw_opcode & 0x07);
		break;
	default:
		throw std::logic_error("corrupt opcode table");
	}

	/* convert operands to strings */
	std::ostringstream stream;

	if (opcode_entry->op1_type==OP_R2 || opcode_entry->op1_type==OP_R3 ||
		opcode_entry->op1_type==OP_R4 || opcode_entry->op1_type==OP_R4_l ||
		opcode_entry->op1_type==OP_R5 || opcode_entry->op1_type==OP_R5_r) {
		stream << "r" << operand1;
	} else if (opcode_entry->op1_type != none) {
		stream << "$" << std::hex << operand1;
	}

	instruction->operand1 = stream.str();

	stream.str("");
	if (opcode_entry->op2_type==OP_R2 || opcode_entry->op2_type==OP_R3 ||
		opcode_entry->op2_type==OP_R4 || opcode_entry->op2_type==OP_R4_l ||
		opcode_entry->op2_type==OP_R5 || opcode_entry->op2_type==OP_R5_r) {
		stream << "r" << operand2;
	} else if (opcode_entry->op2_type != none) {
		stream << "$" << std::hex << operand2;
	}
	
	instruction->operand2 = stream.str();
	
	/* build instruction structure */
	if (opcode_entry->mnemonic)
		instruction->mnemonic = opcode_entry->mnemonic;
	else
		instruction->mnemonic = "[inv]";

	instruction->addr = addr/2;
	instruction->size = opcode_entry->size*2;

	for (unsigned int i=0; i<instruction->size; i++)
		instruction->machine_code[i] = data[i];

	return instruction->size;
}
