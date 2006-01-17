
#ifndef WORDARGUMENTTEST_H
#define WORDARGUMENTTEST_H


#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>
#include "../wordargument.h"


/**
 * Unit tests for the WordArgument class.
 *
 * Untested methods:
 *		Constructor
 *		Value()					(trivial)
 */
class WordArgumentTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(WordArgumentTest);
	CPPUNIT_TEST(IsType);
	CPPUNIT_TEST(SetType);
	CPPUNIT_TEST(Parse);
	CPPUNIT_TEST_SUITE_END();

public:
	void IsType();
	void SetType();
	void Parse();
};


#endif
