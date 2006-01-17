
#ifndef COMMANDTABLETEST_H
#define COMMANDTABLETEST_H


#include <boost/shared_ptr.hpp>
#include <cppunit/extensions/HelperMacros.h>
#include "../commandtable.h"
#include "../command.h"
#include "../argument.h"


/**
 * Unit tests for CommandTable class.
 */
class CommandTableTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(CommandTableTest);
	CPPUNIT_TEST(Find);
	CPPUNIT_TEST_SUITE_END();

public:
	void Find();
};


/**
 * Simple command table used for testing, since the base class contains no
 * commands.
 */
class MockCommandTable : public CommandTable {
public:
	enum MockCommand {
		Command1 = 1,
		Command2 = 2,
		Command3 = 3
	};

	MockCommandTable() {
		boost::shared_ptr<Command> command;
		std::list<boost::shared_ptr<Argument> > argument_list;

		command = boost::shared_ptr<Command>(new Command("command1", Command1,
													"no help\n", "no help\n"));
		command_table.push_back(command);
		
		command = boost::shared_ptr<Command>(new Command("command2", Command2,
													"no help\n", "no help\n"));
		command_table.push_back(command);
		
		command = boost::shared_ptr<Command>(new Command("command3", Command3,
													"no help\n", "no help\n"));
		command_table.push_back(command);
	}
};


#endif
