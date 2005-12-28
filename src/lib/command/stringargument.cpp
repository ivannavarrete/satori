
#include <stdexcept>
#include <boost/tokenizer.hpp>
#include "stringargument.h"


/**
 * Check whether the @a argument represents an <argument string>.
 *
 * @param argument		string to check
 *
 * @return				True if @a argument represents an <argument string>,
 * 						false if it doesn't.
 */
bool StringArgument::IsType(const std::string &argument) const {
	// try to convert <argument string> to a <normal string>
	try {
		Parse(argument);
	} catch (std::invalid_argument e) {
		return false;
	}

	return true;
}


/**
 * Set the value of this object to the <normal string> represented by the
 * <argument string>.
 *
 * @param argument			<argument string> to convert to <normal string>
 *
 * @throws std::invalid_argument	This exception is thrown if @a argument is
 * 									not a valid <argument string>.
 */
void StringArgument::SetType(const std::string &argument) {
	value = Parse(argument);
}


/**
 * Extract the first argument from the @a arguments if it is a valid <argument
 * string>. The argument is removed from @a arguments. If the first argument is
 * not a valid <argument string> then an empty string is returned and @a
 * arguments remains unchanged.
 *
 * @param arguments		string to extract <argument string> from
 *
 * @return				The extracted first argument or the empty string if it
 * 						wasn't an <argument string>.
 */
std::string StringArgument::Extract(std::string &arguments) const {
	// create a tokenizer from the arguments string
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> separators("", "\"");
	tokenizer tokens(arguments, separators);

	if (tokens.begin() == tokens.end())
		return "";

	// remove leading whitespace and check whether first token is a quote
	tokenizer::iterator token_i = tokens.begin();
	std::string first_token = *token_i;
	if (first_token.find_first_not_of(" \t\n") == std::string::npos)
		first_token = *(++token_i);
	
	if (first_token != "\"")
		return "";
	
	// get second token
	++token_i;
	if (token_i == tokens.end())
		return "";

	std::string second_token = *token_i;
	std::string third_token;

	// if second token is a quote then we're dealing with an empty string
	if (second_token == "\"") {
		second_token = "";
		third_token = "\"";

	// otherwise we check that the third token is a quote
	} else {
		++token_i;
		if (token_i == tokens.end())
			return "";

		third_token = *token_i;
		if (third_token != "\"")
			return "";
	}

	// combine all three tokens and check if they form a valid <argument string>
	std::string argument = first_token + second_token + third_token;
	if (!IsType(argument))
		return "";

	// remove the <argument string> from arguments
	int n = arguments.find(argument);
	arguments.erase(0, n + argument.length());

	// return a copy of the extracted <argument string>
	return argument;
}


/**
 * Convert the <argument string> to a normal string and return the result.
 *
 * @param argument			<argument string> to convert to <normal string>
 *
 * @return					corresponding <normal string>
 *
 * @throws invalid_argument	This exception is thrown if @a argument is not a
 * 							valid <argument string>.
 */
std::string StringArgument::Parse(const std::string &argument) {
	std::string tok(argument);

	// check whether the string is at least two characters and that the first
	// and last characters are '"'
	if ((tok.length() > 1) && *(tok.begin()) == '"' && *(tok.end()-1) == '"') {
		// remove first and last '"'
		tok.erase(tok.begin());
		tok.erase(tok.end()-1);

		// throw exception if the string contains additional '"'
		if (tok.find_first_of("\"") != tok.npos)
			throw std::invalid_argument("Not an argument string");

		// return a copy of the resulting string
		return tok;
	}

	throw std::invalid_argument("Not an argument string");
}
