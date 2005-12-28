
#ifndef STRINGARGUMENT_H
#define STRINGARGUMENT_H


#include <string>
#include "argument.h"


/**
 * @todo Document.
 *
 * A valid argument string is enclosed by two '"' and consists of characters
 * that are not '"' or null.
 */
class StringArgument : public Argument {
public:
	/**
	 * @todo Document
	 */
	StringArgument(Option opt=Argument::Mandatory, int mod=0) :
						Argument(Argument::String, opt, mod) {}

	virtual bool IsType(const std::string &argument) const;
	virtual void SetType(const std::string &argument);
	virtual std::string Extract(std::string &arguments) const;

	static std::string Parse(const std::string &argument);
	const std::string &Value() const { return value; }

private:
	std::string value;
};


#endif
