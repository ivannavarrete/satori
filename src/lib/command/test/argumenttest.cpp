
#include "test/suitename.h"
#include "argumenttest.h"


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ArgumentTest, SuiteName::Command());


/**
 * Extract() method test.
 */
void ArgumentTest::Extract() {
	std::string arguments(" first second \nin_newlines\n \"in_quotes\"\n");
	std::string extracted_first(" second \nin_newlines\n \"in_quotes\"\n");
	std::string extracted_second(" \nin_newlines\n \"in_quotes\"\n");
	std::string extracted_third("\n \"in_quotes\"\n");
	std::string extracted_fourth("\n");

	std::string first_argument("first");
	std::string second_argument("second");
	std::string third_argument("in_newlines");
	std::string fourth_argument("\"in_quotes\"");

	MockArgument argument;

	CPPUNIT_ASSERT_EQUAL(first_argument, argument.Extract(arguments));
	CPPUNIT_ASSERT_EQUAL(extracted_first, arguments);

	CPPUNIT_ASSERT_EQUAL(second_argument, argument.Extract(arguments));
	CPPUNIT_ASSERT_EQUAL(extracted_second, arguments);
	
	CPPUNIT_ASSERT_EQUAL(third_argument, argument.Extract(arguments));
	CPPUNIT_ASSERT_EQUAL(extracted_third, arguments);

	CPPUNIT_ASSERT_EQUAL(fourth_argument, argument.Extract(arguments));
	CPPUNIT_ASSERT_EQUAL(extracted_fourth, arguments);

	CPPUNIT_ASSERT_EQUAL(std::string(""), argument.Extract(arguments));
	CPPUNIT_ASSERT_EQUAL(extracted_fourth, arguments);
}
