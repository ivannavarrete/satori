
#ifndef COMMANDTEST_H
#define COMMANDTEST_H


#include <cppunit/extensions/HelperMacros.h>
#include "../command.h"


/**
 * Unit tests for Command class.
 *
 * Untested methods:
 *		operator=()
 *		Name()					(trivial)
 *		Type()					(trivial)
 *		ShortDescription()		(trivial)
 *		LongDescription()		(trivial)
 *		ArgumentCount()			(trivial)
 *		AddArguments()
 *		IsValid()
 *
 * @Todo Add tests for operator=(), AddArguments() and IsValid() methods.
 */
class CommandTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(CommandTest);
	CPPUNIT_TEST(Constructor);
	CPPUNIT_TEST(AddArguments);
	CPPUNIT_TEST(ParseArguments);
	CPPUNIT_TEST_SUITE_END();

public:
	void Constructor();
	void AddArguments();
	void ParseArguments();
};


#endif
