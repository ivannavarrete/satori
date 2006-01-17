
#include <boost/shared_ptr.hpp>
#include "test/suitename.h"
#include "commandtest.h"


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CommandTest, SuiteName::Command());


/**
 * Command constructor test.
 */
void CommandTest::Constructor() {
	boost::shared_ptr<Command> command;

	// valid command name
	CPPUNIT_ASSERT_NO_THROW(command = boost::shared_ptr<Command>
											(new Command("test")));

	// invalid command name
	CPPUNIT_ASSERT_THROW((command = boost::shared_ptr<Command>
			(new Command("¶¡£ðøæ·"))), std::invalid_argument);
}


/**
 * @todo Implement.
 */
void CommandTest::AddArguments() {
	/*
	list<shared_ptr<Argument> > argument_list;
	Command command("test", 1);

	// at the beginning there are no children in the argument tree
	CPPUNIT_ASSERT(command.argument_tree->empty());

	// add a new argument type
	argument_list.clear();
	argument_list.push_back(shared_ptr<Argument>(new WordArgument));
	cout << "--1\n";
	command.AddArguments(argument_list);

	CPPUNIT_ASSERT(command.argument_tree->child_end() -
					command.argument_tree->child_begin() == 1);
	CPPUNIT_ASSERT((*(command.argument_tree->child_begin())->value()).Type() ==
					Argument::Word);

	// adding the same argument type does not add a new child to the tree
	argument_list.clear();
	argument_list.push_back(shared_ptr<Argument>(new WordArgument));
	cout << "--2\n";
	command.AddArguments(argument_list);

	CPPUNIT_ASSERT(command.argument_tree->child_end() -
					command.argument_tree->child_begin() == 1);
	CPPUNIT_ASSERT((*(command.argument_tree->child_begin())->value()).Type() ==
					Argument::Word);

	// adding a different argument type does add a new child to the tree
	argument_list.clear();
	argument_list.push_back(shared_ptr<Argument>(new NumberArgument));
	cout << "--3\n";
	command.AddArguments(argument_list);

	CPPUNIT_ASSERT(command.argument_tree->child_end() -
					command.argument_tree->child_begin() == 2);

	// adding a list of two arguments adds a child to the top node and a
	// child to that child
	argument_list.clear();
	argument_list.push_back(shared_ptr<Argument>(new StringArgument));
	argument_list.push_back(shared_ptr<Argument>(new StringArgument));
	cout << "--4\n";
	command.AddArguments(argument_list);
	
	CPPUNIT_ASSERT(command.argument_tree->child_end() -
					command.argument_tree->child_begin() == 3);
	*/
}


/**
 * Test for ParseArguments() and GetWord(), GetString() and GetNumber() methods.
 * We test them together since they are not easily tested by themselves. To
 * insure that ParseArguments() works we need to use the Get*() methods, and
 * vice versa.
 *
 * Assumes that AddArguments() works.
 */
void CommandTest::ParseArguments() {
	std::string command_line;
	std::list<boost::shared_ptr<Argument> > argument_list;
	boost::shared_ptr<Command> command;

	// single word argument
	// test command: cmd <word>
	command = boost::shared_ptr<Command>(new Command("cmd"));
	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new WordArgument));
	command->AddArguments(argument_list);
	
	command_line = "cmd word_argument";
	CPPUNIT_ASSERT_NO_THROW(command->ParseArguments(command_line));
	CPPUNIT_ASSERT_EQUAL(std::string("word_argument"), command->GetWord(1));


	// single string argument
	// test command: cmd <string>
	command = boost::shared_ptr<Command>(new Command("cmd"));
	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new StringArgument));
	command->AddArguments(argument_list);
	
	command_line = "cmd \"string argument\"";
	CPPUNIT_ASSERT_NO_THROW(command->ParseArguments(command_line));
	CPPUNIT_ASSERT_EQUAL(std::string("string argument"), command->GetString(1));


	// single number argument
	// test command: cmd <number>
	command = boost::shared_ptr<Command>(new Command("cmd"));
	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new NumberArgument));
	command->AddArguments(argument_list);

	command_line = "cmd 123";
	CPPUNIT_ASSERT_NO_THROW(command->ParseArguments(command_line));
	CPPUNIT_ASSERT_EQUAL(uint32_t(123), command->GetNumber(1));


	// sequence of arguments
	// test command: cmd <word> <string> <number>
	command = boost::shared_ptr<Command>(new Command("cmd"));
	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new WordArgument));
	argument_list.push_back(boost::shared_ptr<Argument>(new StringArgument));
	argument_list.push_back(boost::shared_ptr<Argument>(new NumberArgument));
	command->AddArguments(argument_list);
	
	command_line = "cmd word_argument \"string argument\" 123";
	CPPUNIT_ASSERT_NO_THROW(command->ParseArguments(command_line));
	CPPUNIT_ASSERT_EQUAL(std::string("word_argument"), command->GetWord(1));
	CPPUNIT_ASSERT_EQUAL(std::string("string argument"), command->GetString(2));
	CPPUNIT_ASSERT_EQUAL(uint32_t(123), command->GetNumber(3));


	// parallel arguments at leaf nodes, with depth one
	// test command: cmd <word> | <string> | <number>
	command = boost::shared_ptr<Command>(new Command("cmd"));
	
	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new WordArgument));
	command->AddArguments(argument_list);
	
	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new StringArgument));
	command->AddArguments(argument_list);

	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new NumberArgument));
	command->AddArguments(argument_list);

	command_line = "cmd word_argument";
	CPPUNIT_ASSERT_NO_THROW(command->ParseArguments(command_line));
	CPPUNIT_ASSERT_EQUAL(std::string("word_argument"), command->GetWord(1));

	command_line = "cmd \"string argument\"";
	CPPUNIT_ASSERT_NO_THROW(command->ParseArguments(command_line));
	CPPUNIT_ASSERT_EQUAL(std::string("string argument"), command->GetString(1));

	command_line = "cmd 123";
	CPPUNIT_ASSERT_NO_THROW(command->ParseArguments(command_line));
	CPPUNIT_ASSERT_EQUAL(uint32_t(123), command->GetNumber(1));


	// parallel arguments at leaf nodes, at higher depth
	// test command: cmd <number> (<word> | <string>)
	command = boost::shared_ptr<Command>(new Command("cmd"));

	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new NumberArgument));
	argument_list.push_back(boost::shared_ptr<Argument>(new WordArgument));
	command->AddArguments(argument_list);

	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new NumberArgument));
	argument_list.push_back(boost::shared_ptr<Argument>(new StringArgument));
	command->AddArguments(argument_list);
	
	command_line = "cmd 123 word_argument";
	CPPUNIT_ASSERT_NO_THROW(command->ParseArguments(command_line));
	CPPUNIT_ASSERT_EQUAL(uint32_t(123), command->GetNumber(1));
	CPPUNIT_ASSERT_EQUAL(std::string("word_argument"), command->GetWord(2));

	command_line = "cmd 123 \"string argument\"";
	CPPUNIT_ASSERT_NO_THROW(command->ParseArguments(command_line));
	CPPUNIT_ASSERT_EQUAL(uint32_t(123), command->GetNumber(1));
	CPPUNIT_ASSERT_EQUAL(std::string("string argument"), command->GetString(2));


	// parallel arguments at internal and leaf nodes
	// test command: cmd <word> | (<number> (<word> | <string>))
	command = boost::shared_ptr<Command>(new Command("cmd"));

	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new WordArgument));
	command->AddArguments(argument_list);

	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new NumberArgument));
	argument_list.push_back(boost::shared_ptr<Argument>(new WordArgument));
	command->AddArguments(argument_list);

	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new NumberArgument));
	argument_list.push_back(boost::shared_ptr<Argument>(new StringArgument));
	command->AddArguments(argument_list);
	
	command_line = "cmd word_argument";
	CPPUNIT_ASSERT_NO_THROW(command->ParseArguments(command_line));
	CPPUNIT_ASSERT_EQUAL(std::string("word_argument"), command->GetWord(1));

	command_line = "cmd 123 word_argument";
	CPPUNIT_ASSERT_NO_THROW(command->ParseArguments(command_line));
	CPPUNIT_ASSERT_EQUAL(uint32_t(123), command->GetNumber(1));
	CPPUNIT_ASSERT_EQUAL(std::string("word_argument"), command->GetWord(2));

	command_line = "cmd 123 \"string argument\"";
	CPPUNIT_ASSERT_NO_THROW(command->ParseArguments(command_line));
	CPPUNIT_ASSERT_EQUAL(uint32_t(123), command->GetNumber(1));
	CPPUNIT_ASSERT_EQUAL(std::string("string argument"), command->GetString(2));


	// again, parallel arguments at internal and leaf nodes, with an additional
	// argument path; perhaps this is covered by the above test
	// test command: cmd <w> | (<s> <n> <n>) | (<n> (<w> | <s>))
	command = boost::shared_ptr<Command>(new Command("cmd"));

	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new WordArgument));
	command->AddArguments(argument_list);
	
	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new StringArgument));
	argument_list.push_back(boost::shared_ptr<Argument>(new NumberArgument));
	argument_list.push_back(boost::shared_ptr<Argument>(new NumberArgument));
	command->AddArguments(argument_list);

	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new NumberArgument));
	argument_list.push_back(boost::shared_ptr<Argument>(new WordArgument));
	command->AddArguments(argument_list);
	
	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new NumberArgument));
	argument_list.push_back(boost::shared_ptr<Argument>(new StringArgument));
	command->AddArguments(argument_list);

	command_line = "cmd word_argument";
	CPPUNIT_ASSERT_NO_THROW(command->ParseArguments(command_line));
	CPPUNIT_ASSERT_EQUAL(std::string("word_argument"), command->GetWord(1));

	command_line = "cmd \"string argument\" 123 456";
	CPPUNIT_ASSERT_NO_THROW(command->ParseArguments(command_line));
	CPPUNIT_ASSERT_EQUAL(std::string("string argument"), command->GetString(1));
	CPPUNIT_ASSERT_EQUAL(uint32_t(123), command->GetNumber(2));
	CPPUNIT_ASSERT_EQUAL(uint32_t(456), command->GetNumber(3));

	command_line = "cmd 123 word_argument";
	CPPUNIT_ASSERT_NO_THROW(command->ParseArguments(command_line));
	CPPUNIT_ASSERT_EQUAL(uint32_t(123), command->GetNumber(1));
	CPPUNIT_ASSERT_EQUAL(std::string("word_argument"), command->GetWord(2));

	command_line = "cmd 123 \"string argument\"";
	CPPUNIT_ASSERT_NO_THROW(command->ParseArguments(command_line));
	CPPUNIT_ASSERT_EQUAL(uint32_t(123), command->GetNumber(1));
	CPPUNIT_ASSERT_EQUAL(std::string("string argument"), command->GetString(2));
	

	// try to invoke errors in ParseArguments()
	// parallel arguments at leaf nodes, at higher depth
	// test command: cmd <number> (<word> | <string>)
	command = boost::shared_ptr<Command>(new Command("cmd"));

	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new NumberArgument));
	argument_list.push_back(boost::shared_ptr<Argument>(new WordArgument));
	command->AddArguments(argument_list);

	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new NumberArgument));
	argument_list.push_back(boost::shared_ptr<Argument>(new StringArgument));
	command->AddArguments(argument_list);
	
	// missing word or string argument
	command_line = "cmd 123";
	CPPUNIT_ASSERT_THROW(command->ParseArguments(command_line),
										std::invalid_argument);

	// second argument is of wrong type
	command_line = "cmd 123 456";
	CPPUNIT_ASSERT_THROW(command->ParseArguments(command_line),
										std::invalid_argument);

	// too many arguments
	command_line = "cmd 123 word_argument 456";
	CPPUNIT_ASSERT_THROW(command->ParseArguments(command_line),
										std::invalid_argument);

	// make sure two commands of the same type don't affect eachother, even
	// though they share the same parse tree
	Command mirror_command = *command;
	command_line = "cmd 123 word_argument";
	CPPUNIT_ASSERT_NO_THROW(command->ParseArguments(command_line));
	command_line = "cmd 456 word_argument2";
	CPPUNIT_ASSERT_NO_THROW(mirror_command.ParseArguments(command_line));
	CPPUNIT_ASSERT_EQUAL(uint32_t(123), command->GetNumber(1));
	CPPUNIT_ASSERT_EQUAL(uint32_t(456), mirror_command.GetNumber(1));
	CPPUNIT_ASSERT_EQUAL(std::string("word_argument"),  command->GetWord(2));
	CPPUNIT_ASSERT_EQUAL(std::string("word_argument2"),mirror_command.GetWord(2));


	// try to invoke errors in Get*() methods
	// parallel arguments at leaf nodes, at higher depth
	// test command: cmd <number> (<word> | <string>)
	command = boost::shared_ptr<Command>(new Command("cmd"));

	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new NumberArgument));
	argument_list.push_back(boost::shared_ptr<Argument>(new WordArgument));
	command->AddArguments(argument_list);

	argument_list.clear();
	argument_list.push_back(boost::shared_ptr<Argument>(new NumberArgument));
	argument_list.push_back(boost::shared_ptr<Argument>(new StringArgument));
	command->AddArguments(argument_list);

	// try to get argument value from an unparsed tree
	command_line = "cmd 123 word_argument";
	CPPUNIT_ASSERT_THROW(command->GetWord(2), std::logic_error);

	// try to get argument value from an argument of the wrong type
	CPPUNIT_ASSERT_NO_THROW(command->ParseArguments(command_line));
	CPPUNIT_ASSERT_THROW(command->GetWord(1), std::logic_error);

	// try to get argument value from a nonexistent argument
	CPPUNIT_ASSERT_THROW(command->GetWord(10), std::logic_error);
}
