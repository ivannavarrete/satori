
#include <sstream>
#include <stdexcept>
#include "numberargument.h"


/**
 * Check whether the @a argument string represents a number argument.
 *
 * @param argument		string to check
 *
 * @return				True if @a argument represents a number argument, false
 * 						otherwise.
 */
bool NumberArgument::IsType(const std::string &argument) const {
	// try to convert argument string to a number
	try {
		Parse(argument);
	} catch (std::invalid_argument e) {
		return false;
	}

	return true;
}


/**
 * Set the object value to the number represented by @a argument.
 *
 * @param argument			string to convert to number
 *
 * @throws invalid_argument	This exception is thrown if @a argument string does
 * 							not represent a valid number argument.
 */
inline void NumberArgument::SetType(const std::string &argument) {
	value = Parse(argument);
}


/**
 * Convert the @a argument string to a 32-bit integer.
 *
 * @param argument			string to convert to number
 *
 * @return					32-bit integer.
 *
 * @throws invalid_argument	This exception is thrown if @a argument string does
 * 							not represent a valid number argument.
 */
uint32_t NumberArgument::Parse(const std::string &argument) {
	std::string tok(argument);
	std::ios::fmtflags flags = std::ios::dec;

	// if the string starts with 0x or $, remove those characters and set
	// hex mode on stringstream
	if (argument.size()>2 && argument.at(0)=='0' && argument.at(1)=='x') {
		tok.erase(tok.begin());
		tok.erase(tok.begin());
		flags = std::ios::hex;
	} else if (tok.find_first_of("$") == 0) {
		tok.erase(tok.begin());
		flags = std::ios::hex;
	}

	// confirm that the rest of the string consists of valid digits and chars
	if (flags == std::ios::dec) {
		if (tok.find_first_not_of("0123456789") != tok.npos)
			throw std::invalid_argument("Not a number");
	} else {
		if (tok.find_first_not_of("0123456789ABCDEFabcdef") != tok.npos)
			throw std::invalid_argument("Not a number");
	}

	// convert string to number
	std::stringstream stream(tok);
	stream.flags(flags);
	uint32_t n;
	stream >> n;
	if (stream.fail())
		throw std::invalid_argument("Not a number");

	return n;
}
