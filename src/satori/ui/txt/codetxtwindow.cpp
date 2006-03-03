
#include <iostream>
#include <iomanip>
#include "codetxtwindow.h"
#include "satori/dinstruction.h"


CodeTxtWindow::CodeTxtWindow(boost::shared_ptr<Code> code) {
	this->code = code;
}


/**
 * Disassemble and display code. If the end address splits up an instruction
 * then that instruction won't be displayed.
 *
 * Note: Need to make the padding length and display of machine code
 * configurable.
 *
 * @param start_addr			start address
 * @param end_addr
 */
void CodeTxtWindow::Disassemble(uint32_t start_addr, uint32_t end_addr) {
	const DInstruction::List &disasm_list =
				code->Disassemble(start_addr, end_addr);

	std::cout.fill('0');
	std::cout << std::hex;

	// display disassembly
	for (DInstruction::List::const_iterator instruction=disasm_list.begin();
		 instruction!=disasm_list.end(); ++instruction) {
		// address
		std::cout << "] " << std::setw(3) << instruction->addr << " ";

		// machine code
		for (unsigned int i=0; i<instruction->size; i++) {
			std::cout << " " << std::setw(2) << static_cast<unsigned int>
												(instruction->machine_code[i]);
		}

		// machine-code to instruction padding
		unsigned int padding = std::max(1, (int)(16-instruction->size*3));
		for (unsigned int i=0; i<padding; i++)
			std::cout << " ";

		// mnemonic
		std::cout << instruction->mnemonic;
		
		// operands
		if (!instruction->operand1.empty()) {
			// instruction to operands padding
			padding = std::max(1, (int)(10-instruction->mnemonic.length()));
			for (unsigned int i=0; i<padding; i++)
				std::cout << " ";
			
			std::cout << instruction->operand1;
			if (!instruction->operand2.empty())
				std::cout << ", " << instruction->operand2;
		}

		std::cout << std::endl;
	}
}
