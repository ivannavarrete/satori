
#ifndef CHARACTERARGUMENT_H
#define CHARACTERARGUMENT_H


#include <string>
#include "argument.h"


/**
 * Representation of a character argument. A valid character argument is a
 * string of three characters, of which the first and last must be single
 * quotes and the middle character can be any 8-bit value.
 */
class CharacterArgument : public Argument{
public:
	CharacterArgument(Option opt=Argument::Mandatory, int mod=0) :
						Argument(Argument::Character, opt, mod) {}

	virtual bool IsType(const std::string &argument) const;
	virtual void SetType(const std::string &argument);
	virtual std::string Extract(std::string &arguments) const;

	static char Parse(const std::string &argument);
	const char &Value() const { return value; }

private:
	char value;
};


#endif
