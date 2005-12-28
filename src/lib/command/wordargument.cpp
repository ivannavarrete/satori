
#include <stdexcept>
#include "wordargument.h"


/**
 * Check whether the @a argument string represents a word argument.
 *
 * @param argument		string to check
 *
 * @return				True if @a argument represents a word argument, false
 * 						otherwise.
 */
bool WordArgument::IsType(const std::string &argument) const {
	// try to convert argument string to a word
	try {
		Parse(argument);
	} catch (std::invalid_argument e) {
		return false;
	}

	return true;
}


/**
 * Set the object value to the word represented by @a argument.
 *
 * @param argument			string to convert to word
 *
 * @throws invalid_argument	This exception is thrown if @a argument string does
 * 							not represent a valid word argument.
 */
inline void WordArgument::SetType(const std::string &argument) {
	value = Parse(argument);
}


/**
 * Convert the @a argument string into a word. In reality this means checking
 * that the @a argument follows the definition of an argument word and if it
 * does a copy of the @a argument string is returned.
 * 
 * @param argument				string to 'convert' to a word
 *
 * @return						the converted word
 *
 * @throws std::invalid_argument	This exception is thrown if the @a argument
 * 									string does not represent a valid word
 * 									argument
 */
std::string WordArgument::Parse(const std::string &argument) {
	// if argument is the empty string or if the first char is not an alphabetic
	// character, then the argument is an invalid word
	if (argument.length() == 0 || !isalpha(argument.at(0)))
		throw std::invalid_argument("Not a word");

	// if the rest of the argument contains anything else but alphanumeric
	// characters or underscores, then the argument is an invalid word
	for (std::string::const_iterator i=argument.begin()+1;
		 i!=argument.end(); ++i) {
		if (!isalnum(*i) && *i != '_')
			throw std::invalid_argument("Not a word");
	}

	return argument;
}
