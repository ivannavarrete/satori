
#ifndef CODETXTWINDOW
#define CODETXTWINDOW


#include <boost/shared_ptr.hpp>
#include "satori/code.h"


class CodeTxtWindow {
public:
	CodeTxtWindow(boost::shared_ptr<Code> code);
	void Disassemble(uint32_t start_addr, uint32_t end_addr);

private:
	boost::shared_ptr<Code> code;
};


#endif
