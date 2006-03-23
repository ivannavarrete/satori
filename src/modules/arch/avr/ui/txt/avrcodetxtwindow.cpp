
#include <iostream>
#include <iomanip>
#include "avrcodetxtwindow.h"
#include "../../avrinstruction.h"


/*
 * Note: Need to make the padding length and display of machine code
 * configurable.
 */
void AvrCodeTxtWindow::Show(const Instruction::List &disasm_list) const {
	// Cast the container of general instructions into a container of
	// avr instructions. This is potentially dangerous, if the list
	// somehow turns out to not contain avr instructions.
	const AvrInstruction::List &avr_disasm_list =
			reinterpret_cast<const AvrInstruction::List &>(disasm_list);

	// output instructions loop
	for (AvrInstruction::List::const_iterator i = avr_disasm_list.begin();
		 i != avr_disasm_list.end(); ++i) {
		const boost::shared_ptr<AvrInstruction> instruction = *i;
		
		// address
		std::cout << "] ";
		std::cout << std::setw(3) << std::setfill('0') << std::hex
				  << instruction->addr << "  ";

		// machine code
		for (unsigned int i=0; i<instruction->size; i++)
			std::cout << std::setw(2) << instruction->machine_code[i] << " ";

		// mnemonic
		std::cout << std::setw(14-instruction->size*3) <<std::setfill(' ')<<" ";
		std::cout << instruction->mnemonic;
		
		// operands
		std::cout << std::setw(10-strlen(instruction->mnemonic))
				  << std::setfill(' ') << " ";

		switch (instruction->operand1_type) {
		case none: break;
		case OP_R2:
		case OP_R3:
		case OP_R4:
		case OP_R4_l:
		case OP_R5:
		case OP_R5_r:
			std::cout << "r" << std::dec << instruction->operand1; break;
		default:
			std::cout << "$" << std::hex << instruction->operand1;
		}

		switch (instruction->operand2_type) {
		case none: break;
		case OP_R2:
		case OP_R3:
		case OP_R4:
		case OP_R4_l:
		case OP_R5:
		case OP_R5_r:
			std::cout << ", r" << std::dec << instruction->operand2; break;
		default:
			std::cout << ", $" << std::hex << instruction->operand2;
		}

		std::cout << std::endl;
	}
}
