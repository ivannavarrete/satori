
#ifndef NUMBERARGUMENT_H
#define NUMBERARGUMENT_H

#include <inttypes.h>
#include "argument.h"


/**
 * Representation of a number argument. The number value this class can hold is
 * determined by a 32-bit integer. This class handles decimal and hexadecimal
 * numbers. Hexadecimal numbers begin with 0x or $.
 *
 * @todo Add support for binary numbers.
 */
class NumberArgument : public Argument {
public:
	NumberArgument(Option opt=Argument::Mandatory, int mod=0) :
						Argument(Argument::Number, opt, mod) {}

	virtual bool IsType(const std::string &argument) const;
	virtual void SetType(const std::string &argument);

	static uint32_t Parse(const std::string &token);
	uint32_t Value() const { return value; }

private:
	uint32_t value;
};


#endif
