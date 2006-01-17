
#include "test/suitename.h"
#include "numberargumenttest.h"


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NumberArgumentTest, SuiteName::Command());


/**
 * IsType() method test. Test if different strings can be converted to numbers.
 */
void NumberArgumentTest::IsType() {
	NumberArgument number_argument;
	std::string number_string;

	// test some valid numbers
	number_string = "0";
	CPPUNIT_ASSERT(number_argument.IsType(number_string));

	number_string = "123";
	CPPUNIT_ASSERT(number_argument.IsType(number_string));

	number_string = "0x123";
	CPPUNIT_ASSERT(number_argument.IsType(number_string));

	number_string = "$123";
	CPPUNIT_ASSERT(number_argument.IsType(number_string));

	number_string = "0xabcdef";
	CPPUNIT_ASSERT(number_argument.IsType(number_string));

	number_string = "$abcdef";
	CPPUNIT_ASSERT(number_argument.IsType(number_string));
	
	number_string = "0xABCDEF";
	CPPUNIT_ASSERT(number_argument.IsType(number_string));

	number_string = "$ABCDEF";
	CPPUNIT_ASSERT(number_argument.IsType(number_string));

	// and some invalid numbers
	number_string = "";
	CPPUNIT_ASSERT(!number_argument.IsType(number_string));

	number_string = "123 ";			// trailing space
	CPPUNIT_ASSERT(!number_argument.IsType(number_string));
	
	number_string = " 123";			// leading space
	CPPUNIT_ASSERT(!number_argument.IsType(number_string));

	number_string = "foo";
	CPPUNIT_ASSERT(!number_argument.IsType(number_string));

	number_string = "0x";
	CPPUNIT_ASSERT(!number_argument.IsType(number_string));

	number_string = "$";
	CPPUNIT_ASSERT(!number_argument.IsType(number_string));
	
	number_string = "0x0x123";
	CPPUNIT_ASSERT(!number_argument.IsType(number_string));
	
	number_string = "$$123";
	CPPUNIT_ASSERT(!number_argument.IsType(number_string));
}


/**
 * SetType() method test. Test setting the value of a NumberArgument object.
 */
void NumberArgumentTest::SetType() {
	NumberArgument number_argument;
	std::string number_string;
	uint32_t expected_number;

	// test some valid numbers
	number_string = "0";
	expected_number = 0;
	CPPUNIT_ASSERT_NO_THROW(number_argument.SetType(number_string));
	CPPUNIT_ASSERT_EQUAL(expected_number, number_argument.Value());

	number_string = "123";
	expected_number = 123;
	CPPUNIT_ASSERT_NO_THROW(number_argument.SetType(number_string));
	CPPUNIT_ASSERT_EQUAL(expected_number, number_argument.Value());

	number_string = "0x123";
	expected_number = 0x123;
	CPPUNIT_ASSERT_NO_THROW(number_argument.SetType(number_string));
	CPPUNIT_ASSERT_EQUAL(expected_number, number_argument.Value());

	number_string = "$123";
	expected_number = 0x123;
	CPPUNIT_ASSERT_NO_THROW(number_argument.SetType(number_string));
	CPPUNIT_ASSERT_EQUAL(expected_number, number_argument.Value());

	number_string = "0xabcdef";
	expected_number = 0xabcdef;
	CPPUNIT_ASSERT_NO_THROW(number_argument.SetType(number_string));
	CPPUNIT_ASSERT_EQUAL(expected_number, number_argument.Value());

	number_string = "$abcdef";
	expected_number = 0xabcdef;
	CPPUNIT_ASSERT_NO_THROW(number_argument.SetType(number_string));
	CPPUNIT_ASSERT_EQUAL(expected_number, number_argument.Value());

	number_string = "0xABCDEF";
	expected_number = 0xABCDEF;
	CPPUNIT_ASSERT_NO_THROW(number_argument.SetType(number_string));
	CPPUNIT_ASSERT_EQUAL(expected_number, number_argument.Value());

	number_string = "$ABCDEF";
	expected_number = 0xABCDEF;
	CPPUNIT_ASSERT_NO_THROW(number_argument.SetType(number_string));
	CPPUNIT_ASSERT_EQUAL(expected_number, number_argument.Value());

	// and some invalid numbers
	number_string = "";
	CPPUNIT_ASSERT_THROW(number_argument.SetType(number_string),
										std::invalid_argument);

	number_string = "123 ";
	CPPUNIT_ASSERT_THROW(number_argument.SetType(number_string),
										std::invalid_argument);
	
	number_string = " 123";
	CPPUNIT_ASSERT_THROW(number_argument.SetType(number_string),
										std::invalid_argument);
	
	number_string = "foo";
	CPPUNIT_ASSERT_THROW(number_argument.SetType(number_string),
										std::invalid_argument);

	number_string = "0x";
	CPPUNIT_ASSERT_THROW(number_argument.SetType(number_string),
										std::invalid_argument);

	number_string = "$";
	CPPUNIT_ASSERT_THROW(number_argument.SetType(number_string),
										std::invalid_argument);
	
	number_string = "0x0x123";
	CPPUNIT_ASSERT_THROW(number_argument.SetType(number_string),
										std::invalid_argument);
	
	number_string = "$$123";
	CPPUNIT_ASSERT_THROW(number_argument.Parse(number_string),
										std::invalid_argument);
}


/**
 * Parse() method test. Test converting a string into a number.
 */
void NumberArgumentTest::Parse() {
	NumberArgument number_argument;
	std::string number_string;
	uint32_t expected_number;

	// test some valid numbers
	number_string = "0";
	expected_number = 0;
	CPPUNIT_ASSERT_EQUAL(expected_number, number_argument.Parse(number_string));

	number_string = "123";
	expected_number = 123;
	CPPUNIT_ASSERT_EQUAL(expected_number, number_argument.Parse(number_string));

	number_string = "0x123";
	expected_number = 0x123;
	CPPUNIT_ASSERT_EQUAL(expected_number, number_argument.Parse(number_string));

	number_string = "$123";
	expected_number = 0x123;
	CPPUNIT_ASSERT_EQUAL(expected_number, number_argument.Parse(number_string));

	number_string = "0xabcdef";
	expected_number = 0xabcdef;
	CPPUNIT_ASSERT_EQUAL(expected_number, number_argument.Parse(number_string));

	number_string = "$abcdef";
	expected_number = 0xabcdef;
	CPPUNIT_ASSERT_EQUAL(expected_number, number_argument.Parse(number_string));

	number_string = "0xABCDEF";
	expected_number = 0xABCDEF;
	CPPUNIT_ASSERT_EQUAL(expected_number, number_argument.Parse(number_string));

	number_string = "$ABCDEF";
	expected_number = 0xABCDEF;
	CPPUNIT_ASSERT_EQUAL(expected_number, number_argument.Parse(number_string));

	// and some invalid numbers
	number_string = "";
	CPPUNIT_ASSERT_THROW(number_argument.Parse(number_string),
										std::invalid_argument);

	number_string = "123 ";
	CPPUNIT_ASSERT_THROW(number_argument.Parse(number_string),
										std::invalid_argument);
	
	number_string = " 123";
	CPPUNIT_ASSERT_THROW(number_argument.Parse(number_string),
										std::invalid_argument);
	
	number_string = "foo";
	CPPUNIT_ASSERT_THROW(number_argument.Parse(number_string),
										std::invalid_argument);

	number_string = "0x";
	CPPUNIT_ASSERT_THROW(number_argument.Parse(number_string),
										std::invalid_argument);

	number_string = "$";
	CPPUNIT_ASSERT_THROW(number_argument.Parse(number_string),
										std::invalid_argument);
	
	number_string = "0x0x123";
	CPPUNIT_ASSERT_THROW(number_argument.Parse(number_string),
										std::invalid_argument);
	
	number_string = "$$123";
	CPPUNIT_ASSERT_THROW(number_argument.Parse(number_string),
										std::invalid_argument);
}
