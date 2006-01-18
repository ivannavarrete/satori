
#ifndef SERIALTEST_H
#define SERIALTEST_H


#include <cppunit/extensions/HelperMacros.h>
#include "../serial_u.h"


/**
 * Unit tests for the Serial class.
 *
 * Untested methods:
 *		Baud()					(can't invoke error)
 *		DataBits()				(can't invoke error)
 *		StopBits()				(trivial)
 */
class SerialTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(SerialTest);
	CPPUNIT_TEST(Constructor);
	CPPUNIT_TEST(Destructor);
	CPPUNIT_TEST(SetBaud);
	CPPUNIT_TEST(SetDataBits);
	CPPUNIT_TEST(SetStopBits);
	CPPUNIT_TEST_SUITE_END();

public:
	void Constructor();
	void Destructor();
	void SetBaud();
	void SetDataBits();
	void SetStopBits();
};


#endif
