
#include <stdexcept>
#include <boost/tokenizer.hpp>
#include "characterargument.h"


/**
 * Check whether the @a argument represents a character.
 *
 * @param argument				string to check
 *
 * @returns						True if @a argument represents a character
 * 								argument, false otherwise.
 */
bool CharacterArgument::IsType(const std::string &argument) const {
	// try to convert argument string to a character
	try {
		Parse(argument);
	} catch (std::invalid_argument e) {
		return false;
	}

	return true;
}


/**
 * Set the value of this object to the character represented by @a argument.
 *
 * @param argument				 string to convert to character
 *
 * @throws std::invalid_argument This exception is thrown if @a argument does
 * 								 not represent a valid character.
 */
void CharacterArgument::SetType(const std::string &argument) {
	value = Parse(argument);
}


/**
 * Extract the first argument from the @a arguments if it is a valid character
 * argument. The argument is removed from @a arguments. If the first argument
 * is not a valid character argument then an empty string is returned and @a
 * arguments remains unchanged.
 *
 * @param arguments				string to extract character argument from
 *
 * @return						The extracted first argument or the empty
 * 								string if it wasn't a character argument.
 */
std::string CharacterArgument::Extract(std::string &arguments) const {
	// create a tokenizer from the arguments string
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> separators("", "\'");
	tokenizer tokens(arguments, separators);

	if (tokens.begin() == tokens.end())
		return "";

	// remove leading whitespace and check whether first token is a single quote
	tokenizer::iterator token_i = tokens.begin();
	std::string first_token = *token_i;
	if (first_token.find_first_not_of(" \t\n") == std::string::npos)
		first_token = *(++token_i);

	if (first_token != "\'")
		return "";

	// get second token
	++token_i;
	if (token_i == tokens.end())
		return "";
	std::string second_token = *token_i;

	// get third token
	++token_i;
	if (token_i == tokens.end())
		return "";
	std::string third_token = *token_i;

	// if second token is bigger than one char and third token not a quote
	if (second_token.length() != 1 || third_token != "'")
		return "";

	// combine all three tokens and check if they form a valid <argument string>
	std::string argument = first_token + second_token + third_token;
	if (!IsType(argument))
		return "";

	// remove the character argument from arguments
	int n = arguments.find(argument);
	arguments.erase(0, n + argument.length());

	// return a copy of the extracted character argument
	return argument;
}


/**
 * Convert the @a argument string to a normal char.
 *
 * @param argument				 string to convert to char
 *
 * @return						 corresponding char
 *
 * @throws std::invalid_argument This exception is thrown if @a argument is not
 * 								 a valid character argument.
 */
char CharacterArgument::Parse(const std::string &argument) {
	// check that the string is exactly three characters and that the first
	// and last characters are single quotes
	if (argument.length()==3 && argument.at(0)=='\'' && argument.at(2)=='\'')
		return argument.at(1);

	throw std::invalid_argument("Not a character");
		
}
