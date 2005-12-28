
#ifndef COMMANDTEST_H
#define COMMANDTEST_H


#include <cppunit/extensions/HelperMacros.h>
#include "../command.h"

using namespace std;


class CommandTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(CommandTest);
	CPPUNIT_TEST(Constructor);
	CPPUNIT_TEST(AddArguments);
	CPPUNIT_TEST(ParseArguments);
	CPPUNIT_TEST_SUITE_END();

public:
	static string SuiteName() { return "CommandTest"; }

	void Constructor();
	void AddArguments();
	void ParseArguments();
};


#endif
