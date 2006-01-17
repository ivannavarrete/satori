
#ifndef NUMBERARGUMENTTEST_H
#define NUMBERARGUMENTTEST_H


#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>
#include "../numberargument.h"


/**
 * Unit tests for NumberArgument class.
 *
 * Untested methods:
 *      Constructor
 *		Value()					(trivial)
 */
class NumberArgumentTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(NumberArgumentTest);
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
