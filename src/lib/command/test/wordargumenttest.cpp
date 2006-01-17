
#include "test/suitename.h"
#include "wordargumenttest.h"


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(WordArgumentTest, SuiteName::Command());


/**
 * IsType() method test.
 */
void WordArgumentTest::IsType() {
	WordArgument word_argument;
	std::string word_string;

	word_string = "f";
	CPPUNIT_ASSERT(word_argument.IsType(word_string));
	
	word_string = "foo";
	CPPUNIT_ASSERT(word_argument.IsType(word_string));

	word_string = "f00";
	CPPUNIT_ASSERT(word_argument.IsType(word_string));
	
	word_string = "f_00";
	CPPUNIT_ASSERT(word_argument.IsType(word_string));
	
	word_string = "f_";
	CPPUNIT_ASSERT(word_argument.IsType(word_string));

	// invalid word arguments
	word_string = "";
	CPPUNIT_ASSERT(!word_argument.IsType(word_string));

	word_string = " foo";
	CPPUNIT_ASSERT(!word_argument.IsType(word_string));
	
	word_string = "foo ";
	CPPUNIT_ASSERT(!word_argument.IsType(word_string));

	word_string = "0";
	CPPUNIT_ASSERT(!word_argument.IsType(word_string));

	word_string = "_";
	CPPUNIT_ASSERT(!word_argument.IsType(word_string));

	word_string = "$";
	CPPUNIT_ASSERT(!word_argument.IsType(word_string));

	word_string = "foo$";
	CPPUNIT_ASSERT(!word_argument.IsType(word_string));
}


/**
 * SetType() method test.
 */
void WordArgumentTest::SetType() {
	WordArgument word_argument;
	std::string word_string;

	// valid word arguments
	word_string = "f";
	CPPUNIT_ASSERT_NO_THROW(word_argument.SetType(word_string));
	CPPUNIT_ASSERT_EQUAL(word_string, word_argument.Value());
	
	word_string = "foo";
	CPPUNIT_ASSERT_NO_THROW(word_argument.SetType(word_string));
	CPPUNIT_ASSERT_EQUAL(word_string, word_argument.Value());

	word_string = "f00";
	CPPUNIT_ASSERT_NO_THROW(word_argument.SetType(word_string));
	CPPUNIT_ASSERT_EQUAL(word_string, word_argument.Value());
	
	word_string = "f_00";
	CPPUNIT_ASSERT_NO_THROW(word_argument.SetType(word_string));
	CPPUNIT_ASSERT_EQUAL(word_string, word_argument.Value());
	
	word_string = "f_";
	CPPUNIT_ASSERT_NO_THROW(word_argument.SetType(word_string));
	CPPUNIT_ASSERT_EQUAL(word_string, word_argument.Value());

	// invalid word arguments
	word_string = "";
	CPPUNIT_ASSERT_THROW(word_argument.SetType(word_string),
									std::invalid_argument);

	word_string = " foo";
	CPPUNIT_ASSERT_THROW(word_argument.SetType(word_string),
									std::invalid_argument);
	
	word_string = "foo ";
	CPPUNIT_ASSERT_THROW(word_argument.SetType(word_string),
									std::invalid_argument);

	word_string = "0";
	CPPUNIT_ASSERT_THROW(word_argument.SetType(word_string),
									std::invalid_argument);

	word_string = "_";
	CPPUNIT_ASSERT_THROW(word_argument.SetType(word_string),
									std::invalid_argument);

	word_string = "$";
	CPPUNIT_ASSERT_THROW(word_argument.SetType(word_string),
									std::invalid_argument);

	word_string = "foo$";
	CPPUNIT_ASSERT_THROW(word_argument.SetType(word_string),
									std::invalid_argument);
}


/**
 * Parse() method test.
 */
void WordArgumentTest::Parse() {
	WordArgument word_argument;
	std::string word_string;

	// valid word arguments
	word_string = "f";
	CPPUNIT_ASSERT(word_argument.IsType(word_string));
	
	word_string = "foo";
	CPPUNIT_ASSERT(word_argument.IsType(word_string));

	word_string = "f00";
	CPPUNIT_ASSERT(word_argument.IsType(word_string));
	
	word_string = "f_00";
	CPPUNIT_ASSERT(word_argument.IsType(word_string));
	
	word_string = "f_";
	CPPUNIT_ASSERT(word_argument.IsType(word_string));

	// invalid word arguments
	word_string = "";
	CPPUNIT_ASSERT(!word_argument.IsType(word_string));

	word_string = " foo";
	CPPUNIT_ASSERT(!word_argument.IsType(word_string));
	
	word_string = "foo ";
	CPPUNIT_ASSERT(!word_argument.IsType(word_string));

	word_string = "0";
	CPPUNIT_ASSERT(!word_argument.IsType(word_string));

	word_string = "_";
	CPPUNIT_ASSERT(!word_argument.IsType(word_string));

	word_string = "$";
	CPPUNIT_ASSERT(!word_argument.IsType(word_string));

	word_string = "foo$";
	CPPUNIT_ASSERT(!word_argument.IsType(word_string));
}
