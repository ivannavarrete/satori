
#include "test/suitename.h"
#include "commandtabletest.h"


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CommandTableTest, SuiteName::Command());


void CommandTableTest::Find() {
	MockCommandTable command_table;
	Command command("foobar");

	CPPUNIT_ASSERT(command_table.Find(command, "command1"));
	CPPUNIT_ASSERT(!command_table.Find(command, "bogus_command"));
}
