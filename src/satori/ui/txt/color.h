
#ifndef COLOR_H
#define COLOR_H


class Color {
public:
	static void InitNone();
	static void Init16();
	static void Init256();

	// global colors
	static const char *Default;
	static const char *Prompt;
	static const char *Punctuation;
	static const char *Headline;

	static const char *Address;
	static const char *MachineCode;
	static const char *Mnemonic;
	static const char *Register;
	static const char *Number;

	static const char *HexData;
	static const char *AsciiData;

	static const char *Changed;
	static const char *Error;
};


#endif
