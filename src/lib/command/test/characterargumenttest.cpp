
#include "test/suitename.h"
#include "characterargumenttest.h"


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CharacterArgumentTest,
									SuiteName::Command());


/**
 * IsType() method test. Test if different strings are of type <argument
 * string>.
 */
void CharacterArgumentTest::IsType() {
	CharacterArgument character_argument;
	std::string character_string;

	// test all valid argument strings
	character_string = "' '";
	for (unsigned int i=0; i<0xFF; i++) {
		character_string.at(1) = static_cast<char>(i);
		CPPUNIT_ASSERT(character_argument.IsType(character_string));
	}

	// some invalid argument strings
	// null string
	character_string = "";
	CPPUNIT_ASSERT(!character_argument.IsType(character_string));

	// only one quote
	character_string = "'";
	CPPUNIT_ASSERT(!character_argument.IsType(character_string));

	// two quotes but not enclosing anything
	character_string = "''";
	CPPUNIT_ASSERT(!character_argument.IsType(character_string));

	// two quotes enclosing more than one character
	character_string = "'ab'";
	CPPUNIT_ASSERT(!character_argument.IsType(character_string));
	
	// too many quotes
	character_string = "''''";
	CPPUNIT_ASSERT(!character_argument.IsType(character_string));
}


/**
 * SetType() method test. Test setting the value of a CharacterArgument object.
 */
void CharacterArgumentTest::SetType() {
	CharacterArgument character_argument;
	std::string character_string;
	char expected_char;

	// test all valid argument strings
	character_string = "' '";
	for (unsigned int i=0; i<0xFF; i++) {
		character_string.at(1) = static_cast<char>(i);
		expected_char = static_cast<char>(i);
        CPPUNIT_ASSERT_NO_THROW(character_argument.SetType(character_string));
		CPPUNIT_ASSERT_EQUAL(expected_char, character_argument.Value());
	}

	// some invalid argument strings
	// null string
	character_string = "";
	CPPUNIT_ASSERT_THROW(character_argument.SetType(character_string),
												std::invalid_argument);

	// only one quote
	character_string = "'";
	CPPUNIT_ASSERT_THROW(character_argument.SetType(character_string),
												std::invalid_argument);

	// two quotes but not enclosing anything
	character_string = "''";
	CPPUNIT_ASSERT_THROW(character_argument.SetType(character_string),
												std::invalid_argument);

	// two quotes enclosing more than one character
	character_string = "'ab'";
	CPPUNIT_ASSERT_THROW(character_argument.SetType(character_string),
												std::invalid_argument);
	
	// too many quotes
	character_string = "''''";
	CPPUNIT_ASSERT_THROW(character_argument.SetType(character_string),
												std::invalid_argument);
}


/**
 * Parse() method test. Test parsing a character argument string.
 */
void CharacterArgumentTest::Parse() {
	CharacterArgument character_argument;
	std::string character_string;
	char expected_char;

	// test all valid argument strings
	character_string = "' '";
	for (unsigned int i=0; i<0xFF; i++) {
		character_string.at(1) = static_cast<char>(i);
		expected_char = static_cast<char>(i);
		CPPUNIT_ASSERT_EQUAL(expected_char, character_argument.Parse
												(character_string));
	}

	// some invalid argument strings
	// null string
	character_string = "";
	CPPUNIT_ASSERT_THROW(character_argument.Parse(character_string),
											std::invalid_argument);

	// only one quote
	character_string = "'";
	CPPUNIT_ASSERT_THROW(character_argument.Parse(character_string),
											std::invalid_argument);

	// two quotes but not enclosing anything
	character_string = "''";
	CPPUNIT_ASSERT_THROW(character_argument.Parse(character_string),
											std::invalid_argument);

	// two quotes enclosing more than one character
	character_string = "'ab'";
	CPPUNIT_ASSERT_THROW(character_argument.Parse(character_string),
											std::invalid_argument);
	
	// too many quotes
	character_string = "''''";
	CPPUNIT_ASSERT_THROW(character_argument.Parse(character_string),
											std::invalid_argument);
}


/**
 * Extract() method test. Test extraction of first argument from a string of
 * arguments.
 */
void CharacterArgumentTest::Extract() {
	CharacterArgument character_argument;
	std::string command_line;
	std::string rest_of_command_line;
	std::string expected_first;

	// test all valid characters arguments as the first argument
	for (unsigned int i=0; i<0xFF; i++) {
		command_line = " \t\n' ' 'a' \"additional\" arguments 123";
		rest_of_command_line = " 'a' \"additional\" arguments 123";
		expected_first = "' '";
		command_line.at(4) = static_cast<char>(i);
		expected_first.at(1) = static_cast<char>(i);
		CPPUNIT_ASSERT_EQUAL(expected_first, character_argument.Extract
														(command_line));
		CPPUNIT_ASSERT_EQUAL(rest_of_command_line, command_line);
	}

	// special case; first argument is valid, while another call to Extract()
	// would fail
	command_line = "''''";
	expected_first = "'''";
	CPPUNIT_ASSERT_EQUAL(expected_first, character_argument.Extract
													(command_line));

	// invalid first arguments
	// empty command line
	command_line = "";
	expected_first = "";
	CPPUNIT_ASSERT_EQUAL(expected_first, character_argument.Extract
													(command_line));

	// only one quote in command line
	command_line = "'";
	expected_first = "";
	CPPUNIT_ASSERT_EQUAL(expected_first, character_argument.Extract
													(command_line));
	
	// two quotes but not enclosing anything
	command_line = "'' 'a' \"additional\" arguments 123";
	expected_first = "";
	CPPUNIT_ASSERT_EQUAL(expected_first, character_argument.Extract
													(command_line));

	// two quotes enclosing more than one character
	command_line = "'ab' 'a' \"additional\" arguments 123";
	expected_first = "";
	CPPUNIT_ASSERT_EQUAL(expected_first, character_argument.Extract
													(command_line));

	// junk in front of valid argument
	command_line = "leading_junk '.' 'a' \"additional\" arguments 123";
	expected_first = "";
	CPPUNIT_ASSERT_EQUAL(expected_first, character_argument.Extract
													(command_line));

	// missing first quote
	command_line = ".' 'a' \"additional\" arguments 123";
	expected_first = "";
	CPPUNIT_ASSERT_EQUAL(expected_first, character_argument.Extract
													(command_line));

	// missing end quote
	command_line = "'. 'a' \"additional\" arguments 123";
	expected_first = "";
	CPPUNIT_ASSERT_EQUAL(expected_first, character_argument.Extract
													(command_line));
}
