
#ifndef AVRINSTRUCTION_H
#define AVRINSTRUCTION_H


#include "satori/instruction.h"


/**
 * This enum is placed outside of AvrInstruction for now, since it makes the
 * definition of opcode tables easier.
 */
enum Opcode {
	// no operation			: 0000 0000 0000 0000
	Nop     = 0x0000,	NopMask     = 0xFFFF,
	// copy register word	: 0000 0001 dddd rrrr
	Movw    = 0x0100,	MovwMask    = 0xFF00,
	
	Muls    = 0x0200,   MulsMask    = 0xFF00,
};


enum OperandType {
	none,
	OP_R2,				// [5..4]						/***/
	OP_R3,				// [6..4], [2..0]				/***/
	OP_R4,				// [7..4], [3..0]				/***/
	OP_R4_l,			// [7..4], [3..0]				/***/
	OP_R5,				// [8..4], [9,3..0]				/***/
	OP_R5_r,			// [8..4]						/***/
	OP_K6,				// [7..6,3..0]					/***/
	OP_K8,				// [11..8,3..0]					/***/
	OP_A5,				// [7..3]						/***/
	OP_A6,				// [10..9,0..3]					/***/
	OP_k7,				// [9..3]						/***/
	OP_k12,				// [11..0]						/***/
	OP_k16,				// [31..16]						/***/
	OP_k22,				// [24..20,16,15..0]			/***/
	OP_b3,				// [0..2]						/***/
	OP_q6				// [13,11..10,2..0]
};


class AvrInstruction : public Instruction {
public:
	typedef std::vector<boost::shared_ptr<AvrInstruction> > List;

	/**
	 * Constructor for use in disassembly. This create an empty instruction
	 * which is later filled in with values from the opcode table.
	 */
	AvrInstruction() {}

	/**
	 * Constructor for use in creating the opcode table.
	 */
	AvrInstruction(uint16_t opcode_, uint16_t mask_,
				   unsigned int size_, char *mnemonic_,
				   OperandType operand1_type_, OperandType operand2_type_) :

				   opcode(opcode_), mask(mask_),
				   size(size_), mnemonic(mnemonic_),
				   operand1_type(operand1_type_), operand2_type(operand2_type_),
				   operand1(0), operand2(0), addr(0) {}


	// The opcode table is only interested in the first set of data fields.
	uint16_t opcode;				///< opcode of instruction, wo arguments
	uint16_t mask;					///< mask to remove the argument bits
	unsigned int size;				///< size of instruction in bytes  (2 or 4)
	const char *mnemonic;			///< mnemonic of the instruction
	OperandType operand1_type;		///< operand 1 type
	OperandType operand2_type;		///< operand 2 type

	// The second set of data fields are additional values for use in
	// simulator and disassembly output.
	uint32_t operand1;				///< operand 1 value
	uint32_t operand2;				///< operand 2 value
	uint32_t addr;					///< address of instruction, in words
	std::vector<int> machine_code;	///< copy of raw instruction data
};


#endif
