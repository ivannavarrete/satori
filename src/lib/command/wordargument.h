
#ifndef WORDARGUMENT_H
#define WORDARGUMENT_H


#include <string>
#include "argument.h"


class WordArgument : public Argument {
public:
	WordArgument(Option opt=Argument::Mandatory, int mod=0) :
						Argument(Argument::Word, opt, mod) {}

	virtual bool IsType(const std::string &argument) const;
	virtual void SetType(const std::string &argument);
	static std::string Parse(const std::string &token);
	const std::string &Value() const { return value; }

private:
	std::string value;
};


#endif
