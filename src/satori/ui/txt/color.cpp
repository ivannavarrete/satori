
#include "color.h"


const char *Color::Default = "";
const char *Color::Prompt = Color::Default;
const char *Color::Punctuation = "";
const char *Color::Headline = "";
const char *Color::Address = "";
const char *Color::MachineCode = "";
const char *Color::Mnemonic = "";
const char *Color::Register = "";
const char *Color::Number = "";
const char *Color::HexData = "";
const char *Color::AsciiData = "";
const char *Color::Changed = "";
const char *Color::Error = "";


void Color::InitNone() {
	Default = "";
	Prompt = "";
	Punctuation = "";
	Headline = "";

	Address = "";
	MachineCode = "";
	Mnemonic = "";
	Register = "";
	Number = "";

	HexData = "";
	AsciiData = "";

	Changed = "";
	Error = "";
}


void Color::Init16() {
	Default = "\x1B[37m";				//  white
	Prompt = Default;
	Punctuation = "";
	Headline = "";

	Address = "\x1B[31m";
	MachineCode = "";
	Mnemonic = "\x1B[34m";				//  blue
	Register = "";
	Number = "";

	HexData = "\x1B[32m";				//  green
	AsciiData = "\x1B[32m";				//  green

	Changed = "\x1B[31m";				//  red
	Error = "\x1B[31m";					//  red
}


void Color::Init256() {					/// Approximate description ///
	Default = "\x1B[38;5;250m";			//  white
	Prompt = "\x1B[38;5;111m";			//  blue
	Punctuation = "\x1B[38;5;111m";		//  blue
	Headline = "\x1B[38;5;111m";		//  blue

	Address = "\x1B[38;5;208m";			//  orange
	MachineCode = "\x1B[38;5;244m";		//  grey
	Mnemonic = "\x1B[38;5;111m";		//  blue
	Register = "\x1B[38;5;64m";			//  green
	Number = "\x1B[38;5;204m";			//  light red

	HexData = "\x1B[38;5;28m";			//  green
	AsciiData = "\x1B[38;5;28m";		//  green

	Changed = "\x1B[38;5;196m";			//  red
	Error = "\x1B[38;5;196m";			//  red
}
