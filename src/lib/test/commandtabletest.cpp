
#include <boost/shared_ptr.hpp>
#include "commandtabletest.h"
#include "../command.h"


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CommandTableTest, CommandTableTest::SuiteName());


void CommandTableTest::Find() {
	BaseCommandTable base_command_table;
	Command command("foobar");

	CPPUNIT_ASSERT(base_command_table.Find(command, "quit"));
	CPPUNIT_ASSERT(!base_command_table.Find(command, "bogus_command"));
}


/**
 * Test that all the right commands are in the BaseCommandTable.
 */
void CommandTableTest::BaseCommandTableConstructor() {
	BaseCommandTable base_command_table;
	Command command("foobar");

	CPPUNIT_ASSERT(base_command_table.Find(command, "quit"));
	CPPUNIT_ASSERT_EQUAL(CommandType::Quit, command.Type());

	CPPUNIT_ASSERT(base_command_table.Find(command, "cls"));
	CPPUNIT_ASSERT_EQUAL(CommandType::Cls, command.Type());

	CPPUNIT_ASSERT(base_command_table.Find(command, "help"));
	CPPUNIT_ASSERT_EQUAL(CommandType::Help, command.Type());

	CPPUNIT_ASSERT(!base_command_table.Find(command, "bogus_command"));
}
