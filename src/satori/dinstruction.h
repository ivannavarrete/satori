
#ifndef DINSTRUCTION_H
#define DINSTRUCTION_H


#include <vector>
#include <string>


class DInstruction {
public:
	typedef std::vector<DInstruction> List;

	#define DISASM_MCODE_SIZE 20

	std::string mnemonic;
	std::string operand1;   
	std::string operand2;   
	unsigned char machine_code[DISASM_MCODE_SIZE];
	unsigned int size;
	uint16_t addr;					// change to 32-bit address
};


#endif
