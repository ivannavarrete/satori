
#ifndef NUMBERARGUMENTTEST_H
#define NUMBERARGUMENTTEST_H


#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>

#include "../numberargument.h"

using namespace std;


/**
 * Test suite for NumberArgument class.
 *
 * Untested methods:
 *		Value()
 */
class NumberArgumentTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(NumberArgumentTest);
	CPPUNIT_TEST(IsType);
	CPPUNIT_TEST(SetType);
	CPPUNIT_TEST(Parse);
	CPPUNIT_TEST_SUITE_END();

public:
	static string SuiteName() { return "NumberArgumentTest"; }

	void IsType();
	void SetType();
	void Parse();
};


#endif
