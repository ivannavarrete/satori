
#ifndef CHARACTERARGUMENTTEST_H
#define CHARACTERARGUMENTTEST_H


#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>
#include "../characterargument.h"


/**
 * Unit tests for the CharacterArgument class.
 *
 * Untested methods:
 *		Constructor
 *		Value()					(trivial)
 */
class CharacterArgumentTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(CharacterArgumentTest);
	CPPUNIT_TEST(IsType);
	CPPUNIT_TEST(SetType);
	CPPUNIT_TEST(Parse);
	CPPUNIT_TEST(Extract);
	CPPUNIT_TEST_SUITE_END();

public:
	void IsType();
	void SetType();
	void Parse();
	void Extract();
};


#endif
