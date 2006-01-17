
#include "test/suitename.h"
#include "stringargumenttest.h"


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(StringArgumentTest, SuiteName::Command());


/**
 * IsType() method test. Test if different strings are of type <argument
 * string>.
 */
void StringArgumentTest::IsType() {
	StringArgument string_argument;
	std::string string_string;

	// first some valid argument strings

	// empty argument string
	string_string = "\"\"";
	CPPUNIT_ASSERT(string_argument.IsType(string_string));

	// nonempty argument string
	string_string = "\"foo\"";
	CPPUNIT_ASSERT(string_argument.IsType(string_string));

	// argument string with spaces
	string_string = "\"foo bar \"";
	CPPUNIT_ASSERT(string_argument.IsType(string_string));
	
	// argument string with tabs
	string_string = "\"foo\tbar\"";
	CPPUNIT_ASSERT(string_argument.IsType(string_string));

	// argument string with newlines
	string_string = "\"foo\nbar\"";
	CPPUNIT_ASSERT(string_argument.IsType(string_string));


	// invalid argument strings

	// not enclosed by quotes
	string_string = "";
	CPPUNIT_ASSERT(!string_argument.IsType(string_string));

	// not enclosed by quotes, only one quote
	string_string = "\"";
	CPPUNIT_ASSERT(!string_argument.IsType(string_string));
	
	// enclosed by quotes but one additional quote in the middle
	string_string = "\"\"\"";
	CPPUNIT_ASSERT(!string_argument.IsType(string_string));
	
	// null character in the middle
	string_string.assign("\"foo\0bar\"", 10);
	CPPUNIT_ASSERT(!string_argument.IsType(string_string));
}


/**
 * SetType() method test. Test setting the value of a StringArgument object
 * to an <argument string>.
 */
void StringArgumentTest::SetType() {
	StringArgument string_argument;
	std::string string_string;
	std::string normal_string;

	// valid argument strings

	// empty argument string
	string_string = "\"\"";
	normal_string = "";
	CPPUNIT_ASSERT_NO_THROW(string_argument.SetType(string_string));
	CPPUNIT_ASSERT_EQUAL(string_argument.Value(), normal_string);

	// nonempty argument string
	string_string = "\"foo\"";
	normal_string = "foo";
	CPPUNIT_ASSERT_NO_THROW(string_argument.SetType(string_string));
	CPPUNIT_ASSERT_EQUAL(string_argument.Value(), normal_string);

	// argument string with spaces
	string_string = "\"foo bar \"";
	normal_string = "foo bar ";
	CPPUNIT_ASSERT_NO_THROW(string_argument.SetType(string_string));
	CPPUNIT_ASSERT_EQUAL(string_argument.Value(), normal_string);
	
	// argument string with tabs
	string_string = "\"foo\tbar\"";
	normal_string = "foo\tbar";
	CPPUNIT_ASSERT_NO_THROW(string_argument.SetType(string_string));
	CPPUNIT_ASSERT_EQUAL(string_argument.Value(), normal_string);

	// argument string with newlines
	string_string = "\"foo\nbar\"";
	normal_string = "foo\nbar";
	CPPUNIT_ASSERT_NO_THROW(string_argument.SetType(string_string));
	CPPUNIT_ASSERT_EQUAL(string_argument.Value(), normal_string);


	// invalid argument strings

	// not enclosed by quotes
	string_string = "";
	CPPUNIT_ASSERT_THROW(string_argument.SetType(string_string),
										std::invalid_argument);

	// not enclosed by quotes, only one quote
	string_string = "\"";
	CPPUNIT_ASSERT_THROW(string_argument.SetType(string_string),
										std::invalid_argument);
	
	// enclosed by quotes but one additional quote in the middle
	string_string = "\"\"\"";
	CPPUNIT_ASSERT_THROW(string_argument.SetType(string_string),
										std::invalid_argument);
	
	// null character in the middle
	string_string.assign("\"foo\0bar\"", 10);
	CPPUNIT_ASSERT_THROW(string_argument.SetType(string_string),
										std::invalid_argument);
}


/**
 * Parse() method test. Test parsing an <argument string>.
 */
void StringArgumentTest::Parse() {
	StringArgument string_argument;
	std::string string_string;
	std::string normal_string;

	// valid argument strings

	// empty argument string
	string_string = "\"\"";
	normal_string = "";
	CPPUNIT_ASSERT_EQUAL(string_argument.Parse(string_string), normal_string);

	// nonempty argument string
	string_string = "\"foo\"";
	normal_string = "foo";
	CPPUNIT_ASSERT_EQUAL(string_argument.Parse(string_string), normal_string);

	// argument string with spaces
	string_string = "\"foo bar \"";
	normal_string = "foo bar ";
	CPPUNIT_ASSERT_EQUAL(string_argument.Parse(string_string), normal_string);
	
	// argument string with tabs
	string_string = "\"foo\tbar\"";
	normal_string = "foo\tbar";
	CPPUNIT_ASSERT_EQUAL(string_argument.Parse(string_string), normal_string);

	// argument string with newlines
	string_string = "\"foo\nbar\"";
	normal_string = "foo\nbar";
	CPPUNIT_ASSERT_EQUAL(string_argument.Parse(string_string), normal_string);

	// invalid argument strings

	// not enclosed by quotes
	string_string = "";
	CPPUNIT_ASSERT_THROW(string_argument.Parse(string_string),
										std::invalid_argument);

	// not enclosed by quotes, only one quote
	string_string = "\"";
	CPPUNIT_ASSERT_THROW(string_argument.Parse(string_string),
										std::invalid_argument);

	// enclosed by quotes but one additional quote in the middle
	string_string = "\"\"\"";
	CPPUNIT_ASSERT_THROW(string_argument.Parse(string_string),
										std::invalid_argument);
	
	// null character in the middle
	string_string.assign("\"foo\0bar\"", 10);
	CPPUNIT_ASSERT_THROW(string_argument.Parse(string_string),
										std::invalid_argument);
}


/**
 * Extract() method test. Test extraction of first argument from a string of
 * arguments.
 */
void StringArgumentTest::Extract() {
	StringArgument string_argument;
	std::string command_line;
	std::string expected_first;

	// valid first arguments

	// empty <argument string> as first argument
	command_line = "\"\"";
	expected_first = "\"\"";
	CPPUNIT_ASSERT_EQUAL(expected_first, string_argument.Extract(command_line));
	
	// plain valid argument in quotes
	command_line = "\"in quotes\" \"additional\" arguments 123";
	expected_first = "\"in quotes\"";
	CPPUNIT_ASSERT_EQUAL(expected_first, string_argument.Extract(command_line));

	// valid first argument but with leading whitespace
	command_line = " \t\n\"in quotes\" \"additional\" arguments 123";
	expected_first = "\"in quotes\"";
	CPPUNIT_ASSERT_EQUAL(expected_first, string_argument.Extract(command_line));


	// invalid first arguments

	// empty command line
	command_line = "";
	expected_first = "";
	CPPUNIT_ASSERT_EQUAL(expected_first, string_argument.Extract(command_line));

	// only one quote in command line
	command_line = "\"";
	expected_first = "";
	CPPUNIT_ASSERT_EQUAL(expected_first, string_argument.Extract(command_line));
	
	// no quotes around first argument
	command_line = "not_in_quotes \"additional\" arguments 123";
	expected_first = "";
	CPPUNIT_ASSERT_EQUAL(expected_first, string_argument.Extract(command_line));

	// junk in front of quoted argument
	command_line = "leading_junk\"in quotes\" \"additional\" arguments 123";
	expected_first = "";
	CPPUNIT_ASSERT_EQUAL(expected_first, string_argument.Extract(command_line));

	// missing first quote
	command_line = "not_in_quotes\" \"additional\" arguments 123";
	expected_first = "";
	CPPUNIT_ASSERT_EQUAL(expected_first, string_argument.Extract(command_line));

	// missing end quote
	command_line = "\"not in quotes additional arguments 123";
	expected_first = "";
	CPPUNIT_ASSERT_EQUAL(expected_first, string_argument.Extract(command_line));
}
