
#include <iostream>
#include <iomanip>
#include "codetxtwindow.h"


/**
 * Set the code object to use with this code window.
 *
 * @param code
 */
CodeTxtWindow::CodeTxtWindow(boost::shared_ptr<Code> code) {
	this->code = code;
}


/**
 * Disassemble and display code. If the end address splits up an instruction
 * then that instruction won't be displayed.
 *
 * @param start_addr			start address
 * @param end_addr				end address
 */
void CodeTxtWindow::Disassemble(uint32_t start_addr, uint32_t end_addr) const {
	const Instruction::List &disasm_list =
				code->Disassemble(start_addr, end_addr);

	Show(disasm_list);
}


/*
 * Note: Need to make the padding length and display of machine code
 * configurable.
 */
void CodeTxtWindow::Show(const Instruction::List &/*disasm_list*/) const {
}
