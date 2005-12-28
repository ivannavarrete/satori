
#ifndef STRINGARGUMENTTEST_H
#define STRINGARGUMENTTEST_H


#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>

#include "../stringargument.h"

using namespace std;


class StringArgumentTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(StringArgumentTest);
	CPPUNIT_TEST(IsType);
	CPPUNIT_TEST(SetType);
	CPPUNIT_TEST(Parse);
	CPPUNIT_TEST(Extract);
	CPPUNIT_TEST_SUITE_END();

public:
	static string SuiteName() { return "StringArgumentTest"; }

	void IsType();
	void SetType();
	void Parse();
	void Extract();
};


#endif
