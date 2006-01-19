
#ifndef ARGUMENT_H
#define ARGUMENT_H


#include <string>
#include "commandtable.h"			// for argument modifier in constructor


/**
 * Base argument class.
 *
 * The type attribute is determined by the derived class's constructor. We use
 * it for identifying the type of argument without resorting to runtime id,
 * which is said to be problematic.
 */
class Argument {
public:
	/**
	 * @todo Document.
	 */
	enum ArgType { Word, String, Character, Number };

	/**
	 * @todo Document.
	 */
	enum Option { Mandatory, Optional };

	Argument(ArgType t, Option opt=Mandatory, int mod=CommandType::NoOp) :
							type(t), option(opt), command_modifier(mod) {}
	virtual ~Argument() {}

	ArgType Type() const { return type; }
	Option Opt() const { return option; }
	int Modifier() const { return command_modifier; }

	virtual std::string Extract(std::string &argument) const;

	/**
	 * Check if the @a argument string is of a certain (derived) argument type.
	 *
	 * @param argument			string to check if it is of the derived argument
	 * 							type
	 *
	 * @return					True if @a argument is of the derived argument
	 * 							type, false if it isn't.
	 */
	virtual bool IsType(const std::string &argument) const = 0;

	/**
	 * Set the @a argument value to be the value of a certain (derived)
	 * argument type. The @a argument is first parsed to the correct type.
	 *
	 * @param argument			the string to convert to the derived
	 * 							argument value.
	 *
	 * @exception invalid_argument	Thrown if the @a argument string is not
	 * 								convertible to the derived argument value.
	 */
	virtual void SetType(const std::string &argument) = 0;

private:
	ArgType type;
	Option option;
	int command_modifier;
};


#endif
