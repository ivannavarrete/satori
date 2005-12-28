
#ifndef WORDARGUMENTTEST_H
#define WORDARGUMENTTEST_H


#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>

#include "../wordargument.h"

using namespace std;


class WordArgumentTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(WordArgumentTest);
	CPPUNIT_TEST(IsType);
	CPPUNIT_TEST(SetType);
	CPPUNIT_TEST(Parse);
	CPPUNIT_TEST_SUITE_END();

public:
	static string SuiteName() { return "WordArgumentTest"; }

	void IsType();
	void SetType();
	void Parse();
};


#endif
