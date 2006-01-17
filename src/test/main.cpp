
#include <cppunit/TextTestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include "suitename.h"


int main() {
	CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::
															getRegistry();

	//registry.addRegistry(SuiteName::Serial());
	//registry.addRegistry(SuiteName::Lib());
	registry.addRegistry(SuiteName::Command());

	CppUnit::TextTestRunner runner;
	runner.addTest(registry.makeTest());
	runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(),
															std::cerr));

	return runner.run() ? 0 : 1;
}
