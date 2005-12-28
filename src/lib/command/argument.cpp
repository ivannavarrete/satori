
#include <boost/tokenizer.hpp>
#include "argument.h"


/**
 * Extract the first whitespace separated argument, if it is of the derived
 * argument type, from the @a arguments string. The argument is removed from
 * the @a arguments string if the method succeeds. For example, if the derived
 * class is a form of number, then if the first whitespace separated argument
 * in @a arguments is a number, the method extracts that argument and returns
 * it. If the method fails then the empty string is returned and the @a
 * arguments string remains unchanged.
 *
 * @param arguments		string to extract argument from
 *
 * @return				The extracted first argument or the empty string if the
 * 						argument wasn't of the proper type.
 */
std::string Argument::Extract(std::string &arguments) const {
	// create a tokenizer from the arguments string
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> separators(" \t\n");
	tokenizer tokens(arguments, separators);
	if (tokens.begin() == tokens.end())
		return "";

	// @todo Figure out why using const string does not work. Right now we're
	// creating an extra string, where it should be possible to use a reference
	// to the old string.
	//const string &argument = *(tokens.begin());
	std::string argument = *(tokens.begin());

	// check whether the first token is a valid argument type
	if (!IsType(argument))
		return "";

	// remove the token substring from the arguments string
	int n = arguments.find(argument);
	arguments.erase(0, n + argument.length());

	// return the first token
	return argument;
}
