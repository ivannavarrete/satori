
#ifndef AVRCODETXTWINDOW_H
#define AVRCODETXTWINDOW_H


#include <boost/shared_ptr.hpp>
#include "satori/ui/txt/codetxtwindow.h"
#include "satori/instruction.h"


class AvrCodeTxtWindow : public CodeTxtWindow {
public:
	AvrCodeTxtWindow(boost::shared_ptr<Code> code) : CodeTxtWindow(code) {}
	virtual ~AvrCodeTxtWindow() {}

private:
	virtual void Show(const Instruction::List &disasm_list) const;
};


#endif
