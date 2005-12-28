
#ifndef COMMANDTABLETEST_H
#define COMMANDTABLETEST_H


#include <cppunit/extensions/HelperMacros.h>
#include "../commandtable.h"
#include "../basecommandtable.h"

using namespace std;


class CommandTableTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(CommandTableTest);
	CPPUNIT_TEST(Find);
	CPPUNIT_TEST(BaseCommandTableConstructor);
	CPPUNIT_TEST_SUITE_END();

public:
	static string SuiteName() { return "CommandTableTest"; }

	void Find();
	void BaseCommandTableConstructor();

};


#endif
