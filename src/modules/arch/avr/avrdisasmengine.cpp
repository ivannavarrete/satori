
#include <netinet/in.h>	/// @Todo Replace with platform-independent solution
#include <stdexcept>
#include "avrdisasmengine.h"


/**
 * Initialize opcode table. More specifically, convert the 16-bit opcode and
 * mask values in opcode table from undetermined endianess to big endian.
 *
 * The reason for this is that the table is easier to read and compare to
 * datasheet in it's current form, but it needs to be converted to a
 * determined form for comparison to the data taken from memory.
 */
AvrDisasmEngine::AvrDisasmEngine() {
	AvrInstruction *instruction = opcode_table;

	while (instruction->mnemonic) {
		instruction->opcode = htons(instruction->opcode);
		instruction->mask = htons(instruction->mask);
		instruction++;
	}
}


/**
 * Disassemble memory and append result to a list of disassembled instructions.
 *
 * @param data					data to disassemble
 * @param data_size				size of data buffer
 * @param addr					address of the first instruction, in bytes
 * @param instruction_list		list to append disassembled instructions to
 *
 * @returns						Number of bytes disassembled.
 *
 * @throws std::logic_error		Thrown if the opcode table is corrupt.
 */
unsigned int AvrDisasmEngine::Disassemble(const char *data, unsigned int data_size, uint32_t addr, Instruction::List *instruction_list) const {
	// AVR instructions are at least two bytes
	if (data == NULL || data_size < 2)
		return 0;

	uint32_t data_left = data_size;
	while (true) {
		// disassemble one instruction and put it in disassembly list
		boost::shared_ptr<AvrInstruction> instruction =
				boost::shared_ptr<AvrInstruction>(new AvrInstruction);

		DisassembleInstruction(data, data_left, addr, instruction);
		if (instruction->size == 0)
			return data_size - data_left;

		instruction_list->push_back(instruction);

		data += instruction->size;
		data_left -= instruction->size;
		addr += instruction->size;
	}
}


/**
 * Disassemble one instruction.
 *
 * @param data					data to disassemble
 * @param data_size				size of data buffer
 * @data addr					address of this instruction, in bytes
 * @data instruction			object to initialize with the disassembled info
 *
 * @returns						Lenght in bytes of the disassembled instruction,
 * 								or 0 if there is not enough data to disassemble
 * 								the instruction.
 *
 * @throws std::logic_error		Thrown if the opcode table is corrupt.
 */
void AvrDisasmEngine::DisassembleInstruction(const char *data, unsigned int data_size, uint32_t addr, boost::shared_ptr<AvrInstruction> &instruction) const {
	// find the opcode in table
	uint16_t raw_opcode = reinterpret_cast<const uint16_t *>(data)[0];
	uint16_t opcode = htons(raw_opcode);
	AvrInstruction *opcode_entry = opcode_table;

	while (opcode_entry->mnemonic) {
		if ((opcode & opcode_entry->mask) == opcode_entry->opcode)
			break;

		opcode_entry++;
	}

	// return if opcode is found but is to large to disassemble completely
	if (opcode_entry->size > data_size) {
		instruction->size = 0;
		return;
	}

	// copy opcode entry
	*instruction = *opcode_entry;
	instruction->addr = addr/2;
	if (instruction->mnemonic == 0)
		instruction->mnemonic = "[inv]";

	// copy raw instruction data
	for (unsigned int i=0; i<instruction->size; i++)
		instruction->machine_code.push_back((unsigned char)(data[i]));

	// @Bug Possible endianness problem with raw_opcode. In the definition of
	//		raw_opcode above we need to convert it from little-endian (AVR)
	//		to 'host'-endian so that the below manipulations will be correct.

	// extract first operand
	switch (opcode_entry->operand1_type) {
	case none:
		break;
	case OP_R2:
		instruction->operand1 = ((raw_opcode >> 3) & 0x06) | 0x18;	// OK
		break;
	case OP_R3:
		instruction->operand1 = ((raw_opcode >> 4) & 0x07) + 16;	// OK
		break;
	case OP_R4:
		instruction->operand1 = ((raw_opcode >> 4) & 0x0F) + 16;	// OK
		break;
	case OP_R4_l:
		instruction->operand1 = (raw_opcode >> 3) & 0x1E;
		break;
	case OP_R5:
		instruction->operand1 = (raw_opcode >> 4) & 0x1F;			// OK
		break;
	case OP_A5:
		instruction->operand1 = (raw_opcode >> 3) & 0x1F;
		break;
	case OP_A6:
		instruction->operand1 = ((raw_opcode>>5) & 0x30) | (raw_opcode & 0x0F);  // XXX
		break;
	case OP_k7:
		instruction->operand1 = ((raw_opcode >> 3) & 0x7F) + 1;
		break;
	case OP_k12:
		instruction->operand1 = (raw_opcode & 0xFFF) + 1;
		break;
	case OP_k22:
		instruction->operand1 = ((raw_opcode & 0x01) << 16) |
				((raw_opcode & 0x01F0) << 13) | ((uint16_t *)data)[1];
		break;
	case OP_q6:
		instruction->operand1 = ((raw_opcode & 0x2000) >> 8) |
				((raw_opcode & 0xC00) >> 7) | (raw_opcode & 0x07);
		break;
	default:
		throw std::logic_error("corrupt opcode table");
	}

	// extract second operand
	switch (opcode_entry->operand2_type) {
	case none:
		break;
	case OP_R3:
		instruction->operand2 = (raw_opcode & 0x07) + 16;			// OK
		break;
	case OP_R4:
		instruction->operand2 = (raw_opcode & 0x0F) + 16;
		break;
	case OP_R4_l:
		instruction->operand2 = (raw_opcode << 1) & 0x1E;
		break;
	case OP_R5:
		instruction->operand2 = ((raw_opcode>>5) & 0x10) | (raw_opcode & 0x0F);	// OK
		break;
	case OP_R5_r:
		instruction->operand2 = (raw_opcode >> 4) & 0x1F;
		break;
	case OP_K6:
		instruction->operand2 = ((raw_opcode & 0xC0)>>2) | (raw_opcode & 0x0F);	// OK
		break;
	case OP_K8:
		instruction->operand2 = ((raw_opcode & 0xF00)>>4) | (raw_opcode & 0x0F);
		break;
	case OP_A6:
		instruction->operand2 = ((raw_opcode & 0x600)>>5) | (raw_opcode & 0x0F);
		break;
	case OP_b3:
		instruction->operand2 = raw_opcode & 0x07;
		break;
	case OP_k16:
		instruction->operand2 = (reinterpret_cast<const uint16_t *>(data))[1];
		break;
	case OP_q6:
		instruction->operand2 = ((raw_opcode & 0x2000) >> 8) |
				((raw_opcode & 0xC00) >> 7) | (raw_opcode & 0x07);
		break;
	default:
		throw std::logic_error("corrupt opcode table");
	}
}


AvrInstruction AvrDisasmEngine::opcode_table[] = {
	// no operation							: 0000 0000 0000 0000
	AvrInstruction(0x0000, 0xFFFF, 2, "nop", none, none),

	// copy register word					: 0000 0001 dddd rrrr
	AvrInstruction(0x0100, 0xFF00, 2, "movw", OP_R4_l, OP_R4_l),
	// multiply signed						: 0000 0010 dddd rrrr
	AvrInstruction(0x0200, 0xFF00, 2, "muls", OP_R4, OP_R4),
	// multiply signed with unsigned		: 0000 0011 0 ddd 0 rrr 
	AvrInstruction(0x0300, 0xFF88, 2, "mulsu", OP_R3, OP_R3),
	// fractional multiply unsigned			: 0000 0011 0 ddd 1 rrr 
	AvrInstruction(0x0308, 0xFF88, 2, "fmul", OP_R3, OP_R3),
	// fractional multiply signed			: 0000 0011 1 ddd 0 rrr 
	AvrInstruction(0x0380, 0xFF88, 2, "fmuls", OP_R3, OP_R3),
	// fract mul signed with unsigned		: 0000 0011 1 ddd 1 rrr 
	AvrInstruction(0x0388, 0xFF88, 2, "fmulsu", OP_R3, OP_R3),
	// compare with carry *					: 0000 01 r ddddd rrrr 
	AvrInstruction(0x0400, 0xFC00, 2, "cpc", OP_R5, OP_R5),
	// subtract with carry					: 0000 10 r ddddd rrrr 
	AvrInstruction(0x0800, 0xFC00, 2, "sbc", OP_R5, OP_R5),

	// add without carry					: 0000 11 r ddddd rrrr 
	AvrInstruction(0x0C00, 0xFC00, 2, "add", OP_R5, OP_R5),
	// logical shift left					: 0000 11 dddddddddd 
	//{0x0C00, 0xFC00, "lsl"},

	// compare skip if equal				: 0001 00 r ddddd rrrr 
	AvrInstruction(0x1000, 0xFC00, 2, "cpse", OP_R5, OP_R5),
	// compare								: 0001 01 r ddddd rrrr 
	AvrInstruction(0x1400, 0xFC00, 2, "cp", OP_R5, OP_R5),
	// subtract without carry				: 0001 10 r ddddd rrrr 
	AvrInstruction(0x1800, 0xFC00, 2, "sub", OP_R5, OP_R5),

	// add with carry						: 0001 11 r ddddd rrrr 
	AvrInstruction(0x1C00, 0xFC00, 2, "adc", OP_R5, OP_R5),
	// rotate left through carry (adc)		: 0001 11 dddddddddd 
	//{0x1C00, 0xFC00, "rol"},

	// logical and							: 0010 00 r ddddd rrrr 
	AvrInstruction(0x2000, 0xFC00, 2, "and", OP_R5, OP_R5),
	// test for zero or minus (and)			: 0010 00 dddddddddd 
	//{0x2000,0xFC00, "tst", },

	// exclusive or							: 0010 01 r ddddd rrrr 
	AvrInstruction(0x2400, 0xFC00, 2, "eor", OP_R5, OP_R5),
	// clear register (eor)					: 0010 01 dddddddddd 
	//{0x2400, 0xFC00, "clr"},

	// logical or							: 0010 10 r ddddd rrrr 
	AvrInstruction(0x2800, 0xFC00, 2, "or", OP_R5, OP_R5),
	// copy register						: 0010 11 r ddddd rrrr 
	AvrInstruction(0x2C00, 0xFC00, 2, "mov", OP_R5, OP_R5),

	// compare with immediate				: 0011 KKKK dddd KKKK 
	AvrInstruction(0x3000, 0xF000, 2, "cpi", OP_R4, OP_K8),

	// subtract immediate with carry		: 0100 KKKK dddd KKKK 
	AvrInstruction(0x4000, 0xF000, 2, "sbci", OP_R4, OP_K8),
	// subtract immediate					: 0101 KKKK dddd KKKK 
	AvrInstruction(0x5000, 0xF000, 2, "subi", OP_R4, OP_K8),
	// logical or with immediate			: 0110 KKKK dddd KKKK 
	AvrInstruction(0x6000, 0xF000, 2, "ori", OP_R4, OP_K8),
	// set bits in register (ori)			: 0110 KKKK dddd KKKK 
	//{0x6000, 0xF000, "sbr", OP_R4, OP_K8},
	// logical and with immediate			: 0111 KKKK dddd KKKK 
	AvrInstruction(0x7000, 0xF000, 2, "andi", OP_R4, OP_K8),
	// clear bits in register (andi)		: 0111 ____ dddd ____ 
	//{0x7000, 0xF000, "cbr"},

	// load indirect from SRAM using (Z)	: 1000 000 ddddd 0000 
	AvrInstruction(0x8000, 0xFE0F, 2, "ld", OP_R5, none),
	// load indirect from SRAM using (Z+q)	: 10q0 qq0 ddddd 0qqq 
	AvrInstruction(0x8000, 0xD208, 2, "ldd", OP_R5, OP_q6),
	// load indirect from SRAM using (Y)	: 1000 000 ddddd 1000 
	AvrInstruction(0x8008, 0xFE0F, 2, "ld", OP_R5, none),
	// load indirect from SRAM using (Y+q)	: 10q0 qq0 ddddd 1qqq 
	AvrInstruction(0x8008, 0xD208, 2, "ldd", OP_R5, OP_q6),
	// store indirect to SRAM using (Z)		: 1000 001 rrrrr 0000 
	AvrInstruction(0x8200, 0xFE0F, 2, "st", OP_R5, none),
	// store indirect to SRAM using (Z+q)	: 10q0 qq1 rrrrr 0qqq 
	AvrInstruction(0x8200, 0xD208, 2, "std", OP_q6, OP_R5_r),
	// store indirect to SRAM using (Y)		: 1000 001 rrrrr 1000 
	AvrInstruction(0x8208, 0xFE0F, 2, "st", OP_R5, none),
	// store indirect to SRAM using (Y+q)	: 10q0 qq1 rrrrr 1qqq 
	AvrInstruction(0x8208, 0xD208, 2, "std", OP_q6, OP_R5_r),

	// add immediate to word				: 1001 0110 KK dd KKKK 
	AvrInstruction(0x9600, 0xFF00, 2, "adiw", OP_R2, OP_K6),
	// subtract immediate from word			: 1001 0111 KK dd KKKK 
	AvrInstruction(0x9700, 0xFF00, 2, "sbiw", OP_R2, OP_K6),

	// bit set in SREG						: 1001 0100 0 sss 1000 
	//{0x9408, 0xFF8F, "bset", OP_SB, none},
	// bit clear in SREG					: 1001 0100 1 sss 1000 
	//{0x9488, 0xFF8F, "bclr", OP_SB, none},
	// set carry flag						: 1001 0100 0 000 1000 
	AvrInstruction(0x9408, 0xFFFF, 2, "sec", none, none),
	// clear carry flag						: 1001 0100 1 000 1000 
	AvrInstruction(0x9488, 0xFFFF, 2, "clc", none, none),
	// set zero flag						: 1001 0100 0 001 1000 
	AvrInstruction(0x9418, 0xFFFF, 2, "sez", none, none),
	// clear zero flag						: 1001 0100 1 001 1000 
	AvrInstruction(0x9498, 0xFFFF, 2, "clz", none, none),
	// set negative flag					: 1001 0100 0 010 1000 
	AvrInstruction(0x9428, 0xFFFF, 2, "sen", none, none),
	// clear negative flag					: 1001 0100 1 010 1000 
	AvrInstruction(0x94A8, 0xFFFF, 2, "cln", none, none),
	// set overflow flag					: 1001 0100 0 011 1000 
	AvrInstruction(0x9438, 0xFFFF, 2, "sev", none, none),
	// clear overflow flag					: 1001 0100 1 011 1000 
	AvrInstruction(0x94B8, 0xFFFF, 2, "clv", none, none),
	// set signed flag						: 1001 0100 0 100 1000 
	AvrInstruction(0x9448, 0xFFFF, 2, "ses", none, none),
	// clear signed flag					: 1001 0100 1 100 1000 
	AvrInstruction(0x94C8, 0xFFFF, 2, "cls", none, none),
	// set half carry flag					: 1001 0100 0 101 1000 
	AvrInstruction(0x9458, 0xFFFF, 2, "seh", none, none),
	// clear half carry flag				: 1001 0100 1 101 1000 
	AvrInstruction(0x94D8, 0xFFFF, 2, "clh", none, none),
	// set T flag							: 1001 0100 0 110 1000 
	AvrInstruction(0x9468, 0xFFFF, 2, "set", none, none),
	// clear T flag							: 1001 0100 1 110 1000 
	AvrInstruction(0x94E8, 0xFFFF, 2, "clt", none, none),
	// set global interrupt flag			: 1001 0100 0 111 1000 
	AvrInstruction(0x9478, 0xFFFF, 2, "sei", none, none),
	// clear global interrupt flag			: 1001 0100 1 111 1000 
	AvrInstruction(0x94F8, 0xFFFF, 2, "cli", none, none),

	// indirect jump						: 1001 0100 0000 1001 
	AvrInstruction(0x9409, 0xFFFF, 2, "ijmp", none, none),
	// extended indirect jump				: 1001 0100 0001 1001 
	AvrInstruction(0x9419, 0xFFFF, 2, "eijmp", none, none),
	// indirect call to subroutine			: 1001 0101 0000 1001 
	AvrInstruction(0x9509, 0xFFFF, 2, "icall", none, none),
	// extended indirect call to subroutine	: 1001 0101 0001 1001 
	AvrInstruction(0x9519, 0xFFFF, 2, "eicall", none, none),
	// extended load program memory			: 1001 0101 1101 1000 
	AvrInstruction(0x95D8, 0xFFFF, 2, "elpm", none, none),
	// extended store program memory		: 1001 0101 1111 1000 
	AvrInstruction(0x95F8, 0xFFFF, 2, "espm", none, none),

	// jump									: 1001 010 kkkkk 110 k 16*k 
	AvrInstruction(0x940C, 0xFE0E, 4, "jmp", OP_k22, none),
	// long call to a subroutine			: 1001 010 kkkkk 111 k 16*k 
	AvrInstruction(0x940E, 0xFE0E, 4, "call", OP_k22, none),

	// return from subroutine				: 1001 0101 0000 1000 
	AvrInstruction(0x9508, 0xFFFF, 2, "ret", none, none),
	// return from interrupt				: 1001 0101 0001 1000 
	AvrInstruction(0x9518, 0xFFFF, 2, "reti", none, none),
	// set MCU in sleep mode				: 1001 0101 1000 1000 
	AvrInstruction(0x9588, 0xFFFF, 2, "sleep", none, none),
	// watchdog reset						: 1001 0101 1010 1000 
	AvrInstruction(0x95A8, 0xFFFF, 2, "wdr", none, none),
	// load program memory					: 1001 0101 1100 1000 
	AvrInstruction(0x95C8, 0xFFFF, 2, "lpm", none, none),
	// store program memory					: 1001 0101 1110 1000 
	AvrInstruction(0x95E8, 0xFFFF, 2, "spm", none, none),

	// load direct from SRAM				: 1001 000 ddddd 0000 16*k 
	AvrInstruction(0x9000, 0xFE0F, 4, "lds", OP_R5, OP_k16),
	// load indirect from SRAM using (Z+)	: 1001 000 ddddd 0001 
	AvrInstruction(0x9001, 0xFE0F, 2, "ld", OP_R5, none),
	// load indirect from SRAM using (-Z)	: 1001 000 ddddd 0010 
	AvrInstruction(0x9002, 0xFE0F, 2, "ld", OP_R5, none),
	// load indirect from SRAM using (Y+)	: 1001 000 ddddd 1001 
	AvrInstruction(0x9009, 0xFE0F, 2, "ld", OP_R5, none),
	// load indirect from SRAM using (-Y)	: 1001 000 ddddd 1010 
	AvrInstruction(0x900A, 0xFE0F, 2, "ld", OP_R5, none),
	// load indirect from SRAM using (X)	: 1001 000 ddddd 1100 
	AvrInstruction(0x900C, 0xFE0F, 2, "ld", OP_R5, none),
	// load indirect from SRAM using (X+)	: 1001 000 ddddd 1101 
	AvrInstruction(0x900D, 0xFE0F, 2, "ld", OP_R5, none),
	// load indirect from SRAM using (-X)	: 1001 000 ddddd 1110 
	AvrInstruction(0x900E, 0xFE0F, 2, "ld", OP_R5, none),
	// store direct to SRAM					: 1001 001 ddddd 0000 16*k 
	AvrInstruction(0x9200, 0xFE0F, 4, "sts", OP_R5, OP_k16),
	// store indirect to SRAM using (Z+)	: 1001 001 rrrrr 0001 
	AvrInstruction(0x9201, 0xFE0F, 2, "st", OP_R5, none),
	// store indirect to SRAM using (-Z)	: 1001 001 rrrrr 0010 
	AvrInstruction(0x9202, 0xFE0F, 2, "st", OP_R5, none),
	// store indirect to SRAM using (Y+)	: 1001 001 rrrrr 1001 
	AvrInstruction(0x9209, 0xFE0F, 2, "st", OP_R5, none),
	// store indirect to SRAM using (-Y)	: 1001 001 rrrrr 1010 
	AvrInstruction(0x920A, 0xFE0F, 2, "st", OP_R5, none),
	// store indirect to SRAM using (X)		: 1001 001 rrrrr 1100 
	AvrInstruction(0x920C, 0xFE0F, 2, "st", OP_R5, none),
	// store indirect using (X+)			: 1001 001 rrrrr 1101 
	AvrInstruction(0x920D, 0xFE0F, 2, "st", OP_R5, none),
	// store indirect using (-X)			: 1001 001 rrrrr 1110 
	AvrInstruction(0x920E, 0xFE0F, 2, "st", OP_R5, none),


	AvrInstruction(0x9004, 0xFE0F, 2, "lpm", OP_R5, none),// 1001 000 ddddd 0100// load program mem (Z)
	AvrInstruction(0x9005, 0xFE0F, 2, "lpm", OP_R5, none),	// 1001 000 ddddd 0101	// load program mem (Z+)
	AvrInstruction(0x9006, 0xFE0F, 2, "elpm", OP_R5, none),	// 1001 000 ddddd 0110
	AvrInstruction(0x9007, 0xFE0F, 2, "elpm", OP_R5, none),	// 1001 000 ddddd 0111

	// one's complement						: 1001 010 ddddd 0000 
	AvrInstruction(0x9400, 0xFE0F, 2, "com", OP_R5, none),
	// two's complement						: 1001 010 ddddd 0001 
	AvrInstruction(0x9401, 0xFE0F, 2, "neg", OP_R5, none),
	// swap nibbles							: 1001 010 ddddd 0010 
	AvrInstruction(0x9402, 0xFE0F, 2, "swap", OP_R5, none),
	// increment							: 1001 010 ddddd 0011 
	AvrInstruction(0x9403, 0xFE0F, 2, "inc", OP_R5, none),
	// arithmetic shift right				: 1001 010 ddddd 0101 
	AvrInstruction(0x9405, 0xFE0F, 2, "asr", OP_R5, none),
	// logical shift right					: 1001 010 ddddd 0110 
	AvrInstruction(0x9406, 0xFE0F, 2, "lsr", OP_R5, none),
	// rotate right through carry			: 1001 010 ddddd 0111 
	AvrInstruction(0x9407, 0xFE0F, 2, "ror", OP_R5, none),
	// decrement							: 1001 010 ddddd 1010 
	AvrInstruction(0x940A, 0xFE0F, 2, "dec", OP_R5, none),
	// pop register from stack				: 1001 000 ddddd 1111 
	AvrInstruction(0x900F, 0xFE0F, 2, "pop", OP_R5, none),
	// push register on stack				: 1001 001 ddddd 1111 
	AvrInstruction(0x920F, 0xFE0F, 2, "push", OP_R5, none),

	// clear bit in I/O register			: 1001 1000 AAAAA bbb 
	AvrInstruction(0x9800, 0xFF00, 2, "cbi", OP_A5, OP_b3),
	// skip if bit in I/O register cleared	: 1001 1001 AAAAA bbb 
	AvrInstruction(0x9900, 0xFF00, 2, "sbic", OP_A5, OP_b3),
	// set bit in I/O register				: 1001 1010 AAAAA bbb 
	AvrInstruction(0x9A00, 0xFF00, 2, "sbi", OP_A5, OP_b3),
	// skip if bit in I/O register is set	: 1001 1011 AAAAA bbb 
	AvrInstruction(0x9B00, 0xFF00, 2, "sbis", OP_A5, OP_b3),

	// multiply unsigned					: 1001 11 r ddddd rrrr 
	AvrInstruction(0x9C00, 0xFC00, 2, "mul", OP_R5, OP_R5),

	// load an I/O location to register		: 1011 0 AA ddddd AAAA 
	AvrInstruction(0xB000, 0xF800, 2, "in", OP_R5, OP_A6),
	// store register to I/O location		: 1011 1 AA rrrrr AAAA 
	AvrInstruction(0xB800, 0xF800, 2, "out", OP_A6, OP_R5_r),

	// relative jump						: 1100 kkkkkkkkkkkk 
	AvrInstruction(0xC000, 0xF000, 2, "rjmp", OP_k12, none),
	// relative call to subroutine			: 1101 kkkkkkkkkkkk 
	AvrInstruction(0xD000, 0xF000, 2, "rcall", OP_k12, none),

	// load immediate						: 1110 KKKK dddd KKKK 
	AvrInstruction(0xE000, 0xF000, 2, "ldi", OP_R4, OP_K8),
	// set all bits in register				: 1110 1111 dddd 1111 
	//{0xEF0F, 0xFF0F, "ser", OP_R4, none},

	//{0xF000, 0xFC00, "brbs",},// 11110 0 kkkkkkk sss // br if b SREG set
	//{0xF400, 0xFC00, "brbc",},// 11110 1 kkkkkkk sss // br if b SREG clr
	// branch if carry set					: 11110 0 kkkkkkk 000 
	AvrInstruction(0xF000, 0xFC07, 2, "brcs", OP_k7, none),
	// branch if carry cleared				: 11110 1 kkkkkkk 000 
	AvrInstruction(0xF400, 0xFC07, 2, "brcc", OP_k7, none),
	//{0xF000, 0xFC07, "brlo",},// 11110 0 kkkkkkk 000 // (brcs)
	//{0xF400, 0xF400, "brsh",},// 11110 1 kkkkkkk 000 // (brcc)
	// branch if equal						: 11110 0 kkkkkkk 001 
	AvrInstruction(0xF001, 0xFC07, 2, "breq", OP_k7, none),
	// branch if not equal					: 11110 1 kkkkkkk 001 
	AvrInstruction(0xF401, 0xFC07, 2, "brne", OP_k7, none),
	// branch if minus						: 11110 0 kkkkkkk 010 
	AvrInstruction(0xF002, 0xFC07, 2, "brmi", OP_k7, none),
	// branch if plus						: 11110 1 kkkkkkk 010 
	AvrInstruction(0xF402, 0xFC07, 2, "brpl", OP_k7, none),
	// branch if overflow set				: 11110 0 kkkkkkk 011 
	AvrInstruction(0xF003, 0xFC07, 2, "brvs", OP_k7, none),
	// branch if overflow cleared			: 11110 1 kkkkkkk 011 
	AvrInstruction(0xF403, 0xFC07, 2, "brvc", OP_k7, none),
	// branch if less than (signed)			: 11110 0 kkkkkkk 100 
	AvrInstruction(0xF004, 0xFC07, 2, "brlt", OP_k7, none),
	// branch if greater or equal (signed)	: 11110 1 kkkkkkk 100 
	AvrInstruction(0xF404, 0xFC07, 2, "brge", OP_k7, none),
	// branch if half carry flag is set		: 11110 0 kkkkkkk 101 
	AvrInstruction(0xF005, 0xFC07, 2, "brhs", OP_k7, none),
	// branch if half carry flag is cleared	: 11110 1 kkkkkkk 101 
	AvrInstruction(0xF405, 0xFC07, 2, "brhc", OP_k7, none),
	// branch if T flag is set				: 11110 0 kkkkkkk 110 
	AvrInstruction(0xF006, 0xFC07, 2, "brts", OP_k7, none),
	// branch if T flag is cleared			: 11110 1 kkkkkkk 110 
	AvrInstruction(0xF406, 0xFC07, 2, "brtc", OP_k7, none),
	// branch if global interrupts enabled	: 11110 0 kkkkkkk 111 
	AvrInstruction(0xF007, 0xFC07, 2, "brie", OP_k7, none),
	// branch if global interrupts disabled	: 11110 1 kkkkkkk 111 
	AvrInstruction(0xF407, 0xFC07, 2, "brid", OP_k7, none),

	// bit load from T flag to bit in reg	: 11111 00 ddddd 0 bbb 
	AvrInstruction(0xF800, 0xFE08, 2, "bld", OP_R5, OP_b3),
	// bit store from bit in reg to T flag	: 11111 01 ddddd 0 bbb 
	AvrInstruction(0xFA00, 0xFE08, 2, "bst", OP_R5, OP_b3),
	// skip if bit in register is cleared	: 11111 10 rrrrr 0 bbb 
	AvrInstruction(0xFC00, 0xFE08, 2, "sbrc", OP_R5, OP_b3),
	// skip if bit in register is set		: 11111 11 rrrrr 0 bbb 
	AvrInstruction(0xFE00, 0xFE08, 2, "sbrs", OP_R5, OP_b3),

	// table terminator						: 1111 1111 1111 1111 
	AvrInstruction(0xFFFF, 0xFFFF, 2, 0, none, none)
};


/*
AvrInstruction AvrDisasmEngine::opcode_table[] = {
	// no operation							: 0000 0000 0000 0000
	AvrInstruction(0x0000, 0xFFFF, 2, "nop", none, none),

	// copy register word					: 0000 0001 dddd rrrr
	AvrInstruction(0x0100, 0xFF00, 2, "movw", OP_R4_l, OP_R4_l),
	// multiply signed						: 0000 0010 dddd rrrr
	AvrInstruction(0x0200, 0xFF00, 2, "muls", OP_R4, OP_R4),
	// multiply signed with unsigned		: 0000 0011 0 ddd 0 rrr 
	AvrInstruction(0x0300, 0xFF88, 2, "mulsu", OP_R3, OP_R3),
	// fractional multiply unsigned			: 0000 0011 0 ddd 1 rrr 
	AvrInstruction(0x0308, 0xFF88, 2, "fmul", OP_R3, OP_R3),
	// fractional multiply signed			: 0000 0011 1 ddd 0 rrr 
	AvrInstruction(0x0380, 0xFF88, 2, "fmuls", OP_R3, OP_R3),
	// fract mul signed with unsigned		: 0000 0011 1 ddd 1 rrr 
	AvrInstruction(0x0388, 0xFF88, 2, "fmulsu", OP_R3, OP_R3),
	// compare with carry *					: 0000 01 r ddddd rrrr 
	AvrInstruction(0x0400, 0xFC00, 2, "cpc", OP_R5, OP_R5),
	// subtract with carry					: 0000 10 r ddddd rrrr 
	AvrInstruction(0x0800, 0xFC00, 2, "sbc", OP_R5, OP_R5),

	// add without carry					: 0000 11 r ddddd rrrr 
	AvrInstruction(0x0C00, 0xFC00, 2, "add", OP_R5, OP_R5),
	// logical shift left					: 0000 11 dddddddddd 
	//{0x0C00, 0xFC00, "lsl"},

	// compare skip if equal				: 0001 00 r ddddd rrrr 
	AvrInstruction(0x1000, 0xFC00, 2, "cpse", OP_R5, OP_R5),
	// compare								: 0001 01 r ddddd rrrr 
	AvrInstruction(0x1400, 0xFC00, 2, "cp", OP_R5, OP_R5),
	// subtract without carry				: 0001 10 r ddddd rrrr 
	AvrInstruction(0x1800, 0xFC00, 2, "sub", OP_R5, OP_R5),

	// add with carry						: 0001 11 r ddddd rrrr 
	AvrInstruction(0x1C00, 0xFC00, 2, "adc", OP_R5, OP_R5),
	// rotate left through carry (adc)		: 0001 11 dddddddddd 
	//{0x1C00, 0xFC00, "rol"},

	// logical and							: 0010 00 r ddddd rrrr 
	AvrInstruction(0x2000, 0xFC00, 2, "and", OP_R5, OP_R5),
	// test for zero or minus (and)			: 0010 00 dddddddddd 
	//{0x2000,0xFC00, "tst", },

	// exclusive or							: 0010 01 r ddddd rrrr 
	AvrInstruction(0x2400, 0xFC00, 2, "eor", OP_R5, OP_R5),
	// clear register (eor)					: 0010 01 dddddddddd 
	//{0x2400, 0xFC00, "clr"},

	// logical or							: 0010 10 r ddddd rrrr 
	AvrInstruction(0x2800, 0xFC00, 2, "or", OP_R5, OP_R5),
	// copy register						: 0010 11 r ddddd rrrr 
	AvrInstruction(0x2C00, 0xFC00, 2, "mov", OP_R5, OP_R5),

	// compare with immediate				: 0011 KKKK dddd KKKK 
	AvrInstruction(0x3000, 0xF000, 2, "cpi", OP_R4, OP_K8),

	// subtract immediate with carry		: 0100 KKKK dddd KKKK 
	AvrInstruction(0x4000, 0xF000, 2, "sbci", OP_R4, OP_K8),
	// subtract immediate					: 0101 KKKK dddd KKKK 
	AvrInstruction(0x5000, 0xF000, 2, "subi", OP_R4, OP_K8),
	// logical or with immediate			: 0110 KKKK dddd KKKK 
	AvrInstruction(0x6000, 0xF000, 2, "ori", OP_R4, OP_K8),
	// set bits in register (ori)			: 0110 KKKK dddd KKKK 
	//{0x6000, 0xF000, "sbr", OP_R4, OP_K8},
	// logical and with immediate			: 0111 KKKK dddd KKKK 
	AvrInstruction(0x7000, 0xF000, 2, "andi", OP_R4, OP_K8),
	// clear bits in register (andi)		: 0111 ____ dddd ____ 
	//{0x7000, 0xF000, "cbr"},

	// load indirect from SRAM using (Z)	: 1000 000 ddddd 0000 
	AvrInstruction(0x8000, 0xFE0F, 2, "ld", OP_R5, none),
	// load indirect from SRAM using (Z+q)	: 10q0 qq0 ddddd 0qqq 
	AvrInstruction(0x8000, 0xD208, 2, "ldd", OP_R5, OP_q6),
	// load indirect from SRAM using (Y)	: 1000 000 ddddd 1000 
	AvrInstruction(0x8008, 0xFE0F, 2, "ld", OP_R5, none),
	// load indirect from SRAM using (Y+q)	: 10q0 qq0 ddddd 1qqq 
	AvrInstruction(0x8008, 0xD208, 2, "ldd", OP_R5, OP_q6),
	// store indirect to SRAM using (Z)		: 1000 001 rrrrr 0000 
	AvrInstruction(0x8200, 0xFE0F, 2, "st", OP_R5, none),
	// store indirect to SRAM using (Z+q)	: 10q0 qq1 rrrrr 0qqq 
	AvrInstruction(0x8200, 0xD208, 2, "std", OP_q6, OP_R5_r),
	// store indirect to SRAM using (Y)		: 1000 001 rrrrr 1000 
	AvrInstruction(0x8208, 0xFE0F, 2, "st", OP_R5, none),
	// store indirect to SRAM using (Y+q)	: 10q0 qq1 rrrrr 1qqq 
	AvrInstruction(0x8208, 0xD208, 2, "std", OP_q6, OP_R5_r),

	// add immediate to word				: 1001 0110 KK dd KKKK 
	AvrInstruction(0x9600, 0xFF00, 2, "adiw", OP_R2, OP_K6),
	// subtract immediate from word			: 1001 0111 KK dd KKKK 
	AvrInstruction(0x9700, 0xFF00, 2, "sbiw", OP_R2, OP_K6),

	// bit set in SREG						: 1001 0100 0 sss 1000 
	//{0x9408, 0xFF8F, "bset", OP_SB, none},
	// bit clear in SREG					: 1001 0100 1 sss 1000 
	//{0x9488, 0xFF8F, "bclr", OP_SB, none},
	// set carry flag						: 1001 0100 0 000 1000 
	AvrInstruction(0x9408, 0xFFFF, 2, "sec", none, none),
	// clear carry flag						: 1001 0100 1 000 1000 
	AvrInstruction(0x9488, 0xFFFF, 2, "clc", none, none),
	// set zero flag						: 1001 0100 0 001 1000 
	AvrInstruction(0x9418, 0xFFFF, 2, "sez", none, none),
	// clear zero flag						: 1001 0100 1 001 1000 
	AvrInstruction(0x9498, 0xFFFF, 2, "clz", none, none),
	// set negative flag					: 1001 0100 0 010 1000 
	AvrInstruction(0x9428, 0xFFFF, 2, "sen", none, none),
	// clear negative flag					: 1001 0100 1 010 1000 
	AvrInstruction(0x94A8, 0xFFFF, 2, "cln", none, none),
	// set overflow flag					: 1001 0100 0 011 1000 
	AvrInstruction(0x9438, 0xFFFF, 2, "sev", none, none),
	// clear overflow flag					: 1001 0100 1 011 1000 
	AvrInstruction(0x94B8, 0xFFFF, 2, "clv", none, none),
	// set signed flag						: 1001 0100 0 100 1000 
	AvrInstruction(0x9448, 0xFFFF, 2, "ses", none, none),
	// clear signed flag					: 1001 0100 1 100 1000 
	AvrInstruction(0x94C8, 0xFFFF, 2, "cls", none, none),
	// set half carry flag					: 1001 0100 0 101 1000 
	AvrInstruction(0x9458, 0xFFFF, 2, "seh", none, none),
	// clear half carry flag				: 1001 0100 1 101 1000 
	AvrInstruction(0x94D8, 0xFFFF, 2, "clh", none, none),
	// set T flag							: 1001 0100 0 110 1000 
	AvrInstruction(0x9468, 0xFFFF, 2, "set", none, none),
	// clear T flag							: 1001 0100 1 110 1000 
	AvrInstruction(0x94E8, 0xFFFF, 2, "clt", none, none),
	// set global interrupt flag			: 1001 0100 0 111 1000 
	AvrInstruction(0x9478, 0xFFFF, 2, "sei", none, none),
	// clear global interrupt flag			: 1001 0100 1 111 1000 
	AvrInstruction(0x94F8, 0xFFFF, 2, "cli", none, none),

	// indirect jump						: 1001 0100 0000 1001 
	AvrInstruction(0x9409, 0xFFFF, 2, "ijmp", none, none),
	// extended indirect jump				: 1001 0100 0001 1001 
	AvrInstruction(0x9419, 0xFFFF, 2, "eijmp", none, none),
	// indirect call to subroutine			: 1001 0101 0000 1001 
	AvrInstruction(0x9509, 0xFFFF, 2, "icall", none, none),
	// extended indirect call to subroutine	: 1001 0101 0001 1001 
	AvrInstruction(0x9519, 0xFFFF, 2, "eicall", none, none),
	// extended load program memory			: 1001 0101 1101 1000 
	AvrInstruction(0x95D8, 0xFFFF, 2, "elpm", none, none),
	// extended store program memory		: 1001 0101 1111 1000 
	AvrInstruction(0x95F8, 0xFFFF, 2, "espm", none, none),

	// jump									: 1001 010 kkkkk 110 k 16*k 
	AvrInstruction(0x940C, 0xFE0E, 4, "jmp", OP_k22, none),
	// long call to a subroutine			: 1001 010 kkkkk 111 k 16*k 
	AvrInstruction(0x940E, 0xFE0E, 4, "call", OP_k22, none),

	// return from subroutine				: 1001 0101 0000 1000 
	AvrInstruction(0x9508, 0xFFFF, 2, "ret", none, none),
	// return from interrupt				: 1001 0101 0001 1000 
	AvrInstruction(0x9518, 0xFFFF, 2, "reti", none, none),
	// set MCU in sleep mode				: 1001 0101 1000 1000 
	AvrInstruction(0x9588, 0xFFFF, 2, "sleep", none, none),
	// watchdog reset						: 1001 0101 1010 1000 
	AvrInstruction(0x95A8, 0xFFFF, 2, "wdr", none, none),
	// load program memory					: 1001 0101 1100 1000 
	AvrInstruction(0x95C8, 0xFFFF, 2, "lpm", none, none),
	// store program memory					: 1001 0101 1110 1000 
	AvrInstruction(0x95E8, 0xFFFF, 2, "spm", none, none),

	// load direct from SRAM				: 1001 000 ddddd 0000 16*k 
	AvrInstruction(0x9000, 0xFE0F, 4, "lds", OP_R5, OP_k16),
	// load indirect from SRAM using (Z+)	: 1001 000 ddddd 0001 
	AvrInstruction(0x9001, 0xFE0F, 2, "ld", OP_R5, none),
	// load indirect from SRAM using (-Z)	: 1001 000 ddddd 0010 
	AvrInstruction(0x9002, 0xFE0F, 2, "ld", OP_R5, none),
	// load indirect from SRAM using (Y+)	: 1001 000 ddddd 1001 
	AvrInstruction(0x9009, 0xFE0F, 2, "ld", OP_R5, none),
	// load indirect from SRAM using (-Y)	: 1001 000 ddddd 1010 
	AvrInstruction(0x900A, 0xFE0F, 2, "ld", OP_R5, none),
	// load indirect from SRAM using (X)	: 1001 000 ddddd 1100 
	AvrInstruction(0x900C, 0xFE0F, 2, "ld", OP_R5, none),
	// load indirect from SRAM using (X+)	: 1001 000 ddddd 1101 
	AvrInstruction(0x900D, 0xFE0F, 2, "ld", OP_R5, none),
	// load indirect from SRAM using (-X)	: 1001 000 ddddd 1110 
	AvrInstruction(0x900E, 0xFE0F, 2, "ld", OP_R5, none),
	// store direct to SRAM					: 1001 001 ddddd 0000 16*k 
	AvrInstruction(0x9200, 0xFE0F, 4, "sts", OP_R5, OP_k16),
	// store indirect to SRAM using (Z+)	: 1001 001 rrrrr 0001 
	AvrInstruction(0x9201, 0xFE0F, 2, "st", OP_R5, none),
	// store indirect to SRAM using (-Z)	: 1001 001 rrrrr 0010 
	AvrInstruction(0x9202, 0xFE0F, 2, "st", OP_R5, none),
	// store indirect to SRAM using (Y+)	: 1001 001 rrrrr 1001 
	AvrInstruction(0x9209, 0xFE0F, 2, "st", OP_R5, none),
	// store indirect to SRAM using (-Y)	: 1001 001 rrrrr 1010 
	AvrInstruction(0x920A, 0xFE0F, 2, "st", OP_R5, none),
	// store indirect to SRAM using (X)		: 1001 001 rrrrr 1100 
	AvrInstruction(0x920C, 0xFE0F, 2, "st", OP_R5, none),
	// store indirect using (X+)			: 1001 001 rrrrr 1101 
	AvrInstruction(0x920D, 0xFE0F, 2, "st", OP_R5, none),
	// store indirect using (-X)			: 1001 001 rrrrr 1110 
	AvrInstruction(0x920E, 0xFE0F, 2, "st", OP_R5, none),


	AvrInstruction(0x9004, 0xFE0F, 2, "lpm", OP_R5, none),// 1001 000 ddddd 0100// load program mem (Z)
	AvrInstruction(0x9005, 0xFE0F, 2, "lpm", OP_R5, none),	// 1001 000 ddddd 0101	// load program mem (Z+)
	AvrInstruction(0x9006, 0xFE0F, 2, "elpm", OP_R5, none),	// 1001 000 ddddd 0110
	AvrInstruction(0x9007, 0xFE0F, 2, "elpm", OP_R5, none),	// 1001 000 ddddd 0111

	// one's complement						: 1001 010 ddddd 0000 
	AvrInstruction(0x9400, 0xFE0F, 2, "com", OP_R5, none),
	// two's complement						: 1001 010 ddddd 0001 
	AvrInstruction(0x9401, 0xFE0F, 2, "neg", OP_R5, none),
	// swap nibbles							: 1001 010 ddddd 0010 
	AvrInstruction(0x9402, 0xFE0F, 2, "swap", OP_R5, none),
	// increment							: 1001 010 ddddd 0011 
	AvrInstruction(0x9403, 0xFE0F, 2, "inc", OP_R5, none),
	// arithmetic shift right				: 1001 010 ddddd 0101 
	AvrInstruction(0x9405, 0xFE0F, 2, "asr", OP_R5, none),
	// logical shift right					: 1001 010 ddddd 0110 
	AvrInstruction(0x9406, 0xFE0F, 2, "lsr", OP_R5, none),
	// rotate right through carry			: 1001 010 ddddd 0111 
	AvrInstruction(0x9407, 0xFE0F, 2, "ror", OP_R5, none),
	// decrement							: 1001 010 ddddd 1010 
	AvrInstruction(0x940A, 0xFE0F, 2, "dec", OP_R5, none),
	// pop register from stack				: 1001 000 ddddd 1111 
	AvrInstruction(0x900F, 0xFE0F, 2, "pop", OP_R5, none),
	// push register on stack				: 1001 001 ddddd 1111 
	AvrInstruction(0x920F, 0xFE0F, 2, "push", OP_R5, none),

	// clear bit in I/O register			: 1001 1000 AAAAA bbb 
	AvrInstruction(0x9800, 0xFF00, 2, "cbi", OP_A5, OP_b3),
	// skip if bit in I/O register cleared	: 1001 1001 AAAAA bbb 
	AvrInstruction(0x9900, 0xFF00, 2, "sbic", OP_A5, OP_b3),
	// set bit in I/O register				: 1001 1010 AAAAA bbb 
	AvrInstruction(0x9A00, 0xFF00, 2, "sbi", OP_A5, OP_b3),
	// skip if bit in I/O register is set	: 1001 1011 AAAAA bbb 
	AvrInstruction(0x9B00, 0xFF00, 2, "sbis", OP_A5, OP_b3),

	// multiply unsigned					: 1001 11 r ddddd rrrr 
	AvrInstruction(0x9C00, 0xFC00, 2, "mul", OP_R5, OP_R5),

	// load an I/O location to register		: 1011 0 AA ddddd AAAA 
	AvrInstruction(0xB000, 0xF800, 2, "in", OP_R5, OP_A6),
	// store register to I/O location		: 1011 1 AA rrrrr AAAA 
	AvrInstruction(0xB800, 0xF800, 2, "out", OP_A6, OP_R5_r),

	// relative jump						: 1100 kkkkkkkkkkkk 
	AvrInstruction(0xC000, 0xF000, 2, "rjmp", OP_k12, none),
	// relative call to subroutine			: 1101 kkkkkkkkkkkk 
	AvrInstruction(0xD000, 0xF000, 2, "rcall", OP_k12, none),

	// load immediate						: 1110 KKKK dddd KKKK 
	AvrInstruction(0xE000, 0xF000, 2, "ldi", OP_R4, OP_K8),
	// set all bits in register				: 1110 1111 dddd 1111 
	//{0xEF0F, 0xFF0F, "ser", OP_R4, none},

	//{0xF000, 0xFC00, "brbs",},// 11110 0 kkkkkkk sss // br if b SREG set
	//{0xF400, 0xFC00, "brbc",},// 11110 1 kkkkkkk sss // br if b SREG clr
	// branch if carry set					: 11110 0 kkkkkkk 000 
	AvrInstruction(0xF000, 0xFC07, 2, "brcs", OP_k7, none),
	// branch if carry cleared				: 11110 1 kkkkkkk 000 
	AvrInstruction(0xF400, 0xFC07, 2, "brcc", OP_k7, none),
	//{0xF000, 0xFC07, "brlo",},// 11110 0 kkkkkkk 000 // (brcs)
	//{0xF400, 0xF400, "brsh",},// 11110 1 kkkkkkk 000 // (brcc)
	// branch if equal						: 11110 0 kkkkkkk 001 
	AvrInstruction(0xF001, 0xFC07, 2, "breq", OP_k7, none),
	// branch if not equal					: 11110 1 kkkkkkk 001 
	AvrInstruction(0xF401, 0xFC07, 2, "brne", OP_k7, none),
	// branch if minus						: 11110 0 kkkkkkk 010 
	AvrInstruction(0xF002, 0xFC07, 2, "brmi", OP_k7, none),
	// branch if plus						: 11110 1 kkkkkkk 010 
	AvrInstruction(0xF402, 0xFC07, 2, "brpl", OP_k7, none),
	// branch if overflow set				: 11110 0 kkkkkkk 011 
	AvrInstruction(0xF003, 0xFC07, 2, "brvs", OP_k7, none),
	// branch if overflow cleared			: 11110 1 kkkkkkk 011 
	AvrInstruction(0xF403, 0xFC07, 2, "brvc", OP_k7, none),
	// branch if less than (signed)			: 11110 0 kkkkkkk 100 
	AvrInstruction(0xF004, 0xFC07, 2, "brlt", OP_k7, none),
	// branch if greater or equal (signed)	: 11110 1 kkkkkkk 100 
	AvrInstruction(0xF404, 0xFC07, 2, "brge", OP_k7, none),
	// branch if half carry flag is set		: 11110 0 kkkkkkk 101 
	AvrInstruction(0xF005, 0xFC07, 2, "brhs", OP_k7, none),
	// branch if half carry flag is cleared	: 11110 1 kkkkkkk 101 
	AvrInstruction(0xF405, 0xFC07, 2, "brhc", OP_k7, none),
	// branch if T flag is set				: 11110 0 kkkkkkk 110 
	AvrInstruction(0xF006, 0xFC07, 2, "brts", OP_k7, none),
	// branch if T flag is cleared			: 11110 1 kkkkkkk 110 
	AvrInstruction(0xF406, 0xFC07, 2, "brtc", OP_k7, none),
	// branch if global interrupts enabled	: 11110 0 kkkkkkk 111 
	AvrInstruction(0xF007, 0xFC07, 2, "brie", OP_k7, none),
	// branch if global interrupts disabled	: 11110 1 kkkkkkk 111 
	AvrInstruction(0xF407, 0xFC07, 2, "brid", OP_k7, none),

	// bit load from T flag to bit in reg	: 11111 00 ddddd 0 bbb 
	AvrInstruction(0xF800, 0xFE08, 2, "bld", OP_R5, OP_b3),
	// bit store from bit in reg to T flag	: 11111 01 ddddd 0 bbb 
	AvrInstruction(0xFA00, 0xFE08, 2, "bst", OP_R5, OP_b3),
	// skip if bit in register is cleared	: 11111 10 rrrrr 0 bbb 
	AvrInstruction(0xFC00, 0xFE08, 2, "sbrc", OP_R5, OP_b3),
	// skip if bit in register is set		: 11111 11 rrrrr 0 bbb 
	AvrInstruction(0xFE00, 0xFE08, 2, "sbrs", OP_R5, OP_b3),

	// table terminator						: 1111 1111 1111 1111 
	AvrInstruction(0xFFFF, 0xFFFF, 2, 0, none, none)
};
*/
