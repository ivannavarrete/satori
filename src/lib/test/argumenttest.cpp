
#include "argumenttest.h"


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ArgumentTest, ArgumentTest::SuiteName());


/**
 * @todo Implement.
 */
void ArgumentTest::Constructor() {

}


/**
 * Extract() method test.
 */
void ArgumentTest::Extract() {
	string arguments(" first second \nin_newlines\n \"in_quotes\"\n");
	string extracted_first(" second \nin_newlines\n \"in_quotes\"\n");
	string extracted_second(" \nin_newlines\n \"in_quotes\"\n");
	string extracted_third("\n \"in_quotes\"\n");
	string extracted_fourth("\n");

	string first_argument("first");
	string second_argument("second");
	string third_argument("in_newlines");
	string fourth_argument("\"in_quotes\"");

	MockArgument argument;

	CPPUNIT_ASSERT_EQUAL(first_argument, argument.Extract(arguments));
	CPPUNIT_ASSERT_EQUAL(extracted_first, arguments);

	CPPUNIT_ASSERT_EQUAL(second_argument, argument.Extract(arguments));
	CPPUNIT_ASSERT_EQUAL(extracted_second, arguments);
	
	CPPUNIT_ASSERT_EQUAL(third_argument, argument.Extract(arguments));
	CPPUNIT_ASSERT_EQUAL(extracted_third, arguments);

	CPPUNIT_ASSERT_EQUAL(fourth_argument, argument.Extract(arguments));
	CPPUNIT_ASSERT_EQUAL(extracted_fourth, arguments);

	CPPUNIT_ASSERT_EQUAL(string(""), argument.Extract(arguments));
	CPPUNIT_ASSERT_EQUAL(extracted_fourth, arguments);
}
