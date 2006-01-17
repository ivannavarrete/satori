
#ifndef ARGUMENTTEST_H
#define ARGUMENTTEST_H


#include <cppunit/extensions/HelperMacros.h>
#include "../argument.h"


/**
 * Unit tests for the Argument class.
 *
 * Untested methods:
 *		Constructor
 *		Type()					(trivial)
 *		Opt()					(trivial)
 *		Modifier()				(trivial)
 *		IsType()				(pure virtual; mock class method)
 *		SetType()				(pure virtual; mock class method)
 */
class ArgumentTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(ArgumentTest);
	CPPUNIT_TEST(Extract);
	CPPUNIT_TEST_SUITE_END();

public:
	void Extract();
};


/**
 * Since we can't create Argument objects because of it's pure virtual methods
 * we use this class for testing the base Argument class.
 */
class MockArgument : public Argument {
public:
	MockArgument() : Argument(Argument::Word) {}
	
	virtual bool IsType(const std::string &argument) const {
		if (argument.length() > 1 && isalpha(argument.at(1)))
			return true;
		return false;
	}
	
	virtual void SetType(const std::string &argument) {
		argument.length();
	}
};


#endif
