
#ifndef SERIALTEST_H
#define SERIALTEST_H


#include <string>
#include <cppunit/extensions/HelperMacros.h>
#include <QObject>

#include "test/signalcatcher.h"
#include "comm/serial_u.h"

using namespace std;


extern SignalCatcher signal_catcher;


namespace SerialTest {
	/**
	 * Return the name of the Serial test suites.
	 */
	string SuiteName();


	/**
	 * Test the Open() method of the Serial class.
	 */
	class OpenTest : public CppUnit::TestFixture {
		CPPUNIT_TEST_SUITE(OpenTest);
		CPPUNIT_TEST(ConstructorTest);
		CPPUNIT_TEST_SUITE_END();

	public:
		void ConstructorTest();
	};


	/**
	 * Test the Close() method of the Serial class.
	 */
	class CloseTest : public CppUnit::TestFixture {
		CPPUNIT_TEST_SUITE(CloseTest);
		CPPUNIT_TEST(DestructorTest);
		CPPUNIT_TEST_SUITE_END();

	public:
		void DestructorTest();
	};


	class SerialTest : public CppUnit::TestFixture {
		CPPUNIT_TEST_SUITE(SerialTest);
		CPPUNIT_TEST(ConfigTest);
		CPPUNIT_TEST_SUITE_END();

	public:
		void ConfigTest();
	};
}


#endif
