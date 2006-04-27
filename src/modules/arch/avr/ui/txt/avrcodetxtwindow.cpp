
#include <iostream>
#include <iomanip>
#include "avrcodetxtwindow.h"
#include "../../avrinstruction.h"
#include "satori/ui/txt/color.h"


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
		std::cout << Color::Prompt << "] "
				  << Color::Address
				  << std::setw(3) << std::setfill('0') << std::hex
				  << instruction->addr << "  ";

		// machine code
		std::cout << Color::MachineCode;
		for (unsigned int i=0; i<instruction->size; i++)
			std::cout << std::setw(2) << instruction->machine_code[i] << " ";

		// mnemonic
		std::cout << Color::Mnemonic
				  << std::setw(14-instruction->size*3) << std::setfill(' ')
				  << " " << instruction->mnemonic;
		
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
			std::cout << Color::Register << "r"
					  << std::dec << instruction->operand1;
			break;
		default:
			std::cout << Color::Number << "$"
					  << std::hex << instruction->operand1;
			break;
		}

		switch (instruction->operand2_type) {
		case none: break;
		case OP_R2:
		case OP_R3:
		case OP_R4:
		case OP_R4_l:
		case OP_R5:
		case OP_R5_r:
			std::cout << Color::Punctuation << ", "
					  << Color::Register << "r"
					  << std::dec << instruction->operand2;
			break;
		default:
			std::cout << Color::Punctuation << ", "
					  << Color::Number << "$"
					  << std::hex << instruction->operand2;
			break;
		}

		std::cout << std::endl;
	}
}
