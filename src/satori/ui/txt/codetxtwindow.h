
#ifndef CODETXTWINDOW
#define CODETXTWINDOW


#include <boost/shared_ptr.hpp>
#include "satori/code.h"
#include "satori/instruction.h"


/**
 *
 * Specific architecture modules can (and should) subclass this class and
 * reimplement the Show() method for more specific disassembly output.
 *
 * @Todo: Fix the hardcoded padding lenghts in the Show() methods.
 */
class CodeTxtWindow {
public:
	CodeTxtWindow(boost::shared_ptr<Code> code);
	virtual ~CodeTxtWindow() {}
	void Disassemble(uint32_t start_addr, uint32_t end_addr) const;

private:
	virtual void Show(const Instruction::List &disasm_list) const;

	boost::shared_ptr<Code> code;
};


#endif
