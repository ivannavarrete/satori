
#ifndef AVRDISASM_H
#define AVRDISASM_H


#include <inttypes.h>


//namespace Opcode {
	enum OperandType {
		none,
		OP_R2,				/* [5..4]				*/		/***/
		OP_R3,				/* [6..4], [2..0]		*/		/***/
		OP_R4,				/* [7..4], [3..0]		*/		/***/
		OP_R4_l,			/* [7..4], [3..0]		*/		/***/
		OP_R5,				/* [8..4], [9,3..0]		*/		/***/
		OP_R5_r,			/* [8..4]				*/		/***/
		OP_K6,				/* [7..6,3..0]			*/		/***/
		OP_K8,				/* [11..8,3..0]			*/		/***/
		OP_A5,				/* [7..3]				*/		/***/
		OP_A6,				/* [10..9,0..3]			*/		/***/
		OP_k7,				/* [9..3]				*/		/***/
		OP_k12,				/* [11..0]				*/		/***/
		OP_k16,				/* [31..16]				*/		/***/
		OP_k22,				/* [24..20,16,15..0]	*/		/***/
		OP_b3,				/* [0..2]				*/		/***/
		OP_q6				/* [13,11..10,2..0]		*/
	};

	struct Opcode {
		uint16_t opcode;		/* the opcode of the instruction */
		uint16_t mask;			/* mask to remove the arguments */
		unsigned int size;		/* size of the instruction in words (1 or 2) */
		char *mnemonic;			/* mnemonic of the instruction */
		OperandType op1_type;	/* operand 1 */
		OperandType op2_type;	/* operand 2 */
	};

//};

	extern struct Opcode opcode_table[];


#endif
