
#include <cppunit/TextTestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include "comm/test/serialtest.h"
#include "lib/test/treetest.h"
#include "ui/txt/test/argumenttest.h"
#include "ui/txt/test/numberargumenttest.h"
#include "ui/txt/test/stringargumenttest.h"
#include "ui/txt/test/wordargumenttest.h"
#include "ui/txt/test/commandtest.h"
#include "ui/txt/test/commandtabletest.h"


int main() {
	CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::
															getRegistry();

	registry.addRegistry(SerialTest::SuiteName());
	registry.addRegistry(TreeTest::SuiteName());
	registry.addRegistry(ArgumentTest::SuiteName());
	registry.addRegistry(NumberArgumentTest::SuiteName());
	registry.addRegistry(StringArgumentTest::SuiteName());
	registry.addRegistry(WordArgumentTest::SuiteName());
	registry.addRegistry(CommandTest::SuiteName());
	registry.addRegistry(CommandTableTest::SuiteName());

	// add test suites to the list of tests to run
	CppUnit::TextTestRunner runner;
	runner.addTest(registry.makeTest());
	runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(),
															std::cerr));

	return runner.run() ? 0 : 1;
}
