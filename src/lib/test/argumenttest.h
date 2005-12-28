
#ifndef ARGUMENTTEST_H
#define ARGUMENTTEST_H


#include <cppunit/extensions/HelperMacros.h>

#include "../argument.h"

using namespace std;


class ArgumentTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(ArgumentTest);
	CPPUNIT_TEST(Constructor);
	CPPUNIT_TEST(Extract);
	CPPUNIT_TEST_SUITE_END();

public:
	static string SuiteName() { return "ArgumentTest"; }

	void Constructor();
	void Extract();
};


/**
 * Since we can't create Argument objects because of it's pure virtual methods
 * we use this class for testing the base Argument.
 */
class MockArgument : public Argument {
public:
	MockArgument() : Argument(Argument::Word) {}
	
	virtual bool IsType(const string &argument) const {
		if (argument.length() > 1 && isalpha(argument.at(1)))
			return true;
		return false;
	}
	
	virtual void SetType(const string &argument) {
		argument.length();
	}
};


#endif
