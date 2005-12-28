
#include "serialtest.h"

namespace SerialTest {

// register test fixtures into Serial registry
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OpenTest, SuiteName());
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CloseTest, SuiteName());
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SerialTest, SuiteName());
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SendTest, SuiteName());
//CPPUNIT_TEST_SUITE_NAMED_RGISTRATION(ReceiveTest, SuiteName());


/*
 * Return the name of the Serial test suites.
 */
string SuiteName() { return "Serial"; }


/**
 * Serial object constructor tests.
 */
void OpenTest::ConstructorTest() {
	// create a Serial object
	CPPUNIT_ASSERT_NO_THROW(Serial("/dev/ttyS0"));

	// create a Serial object from a nonexistent file
	CPPUNIT_ASSERT_THROW(Serial("f00bar"), runtime_error);

	// create a Serial object from a file that is not a character device
	CPPUNIT_ASSERT_THROW(Serial("bogus_serial_dev"), runtime_error);

	/// @todo implement test to create a Serial object from a character device
	/// file that can't be opened (no permission for example)

	/// @todo implement test to create a Serial object that can't determine
	/// and save it's mode
}



/**
 * Test that the serial device is properly flushed when closed.
 */
void CloseTest::DestructorTest() {
	Serial *serial = NULL;

	// create a Serial object
	try {
		serial = new Serial("/dev/ttyS0");
	} catch (...) {
		CPPUNIT_FAIL("Failed to construct Serial object for destructor test.");
	}

	// connect error signal to the signal catcher
	QObject::connect(serial, SIGNAL(ErrorSignal(string)),
					 &signal_catcher, SLOT(StringSlot(string)),
					 Qt::DirectConnection);

	// destroy object and check that no signals were received
	delete serial;
	CPPUNIT_ASSERT(signal_catcher.StringSignalsReceived(0));
}


void SerialTest::ConfigTest() {
	Serial *serial = NULL;
	int baud = 9600;
	int data_bits = 8;
	int stop_bits = 1;

	int invalid_baud = 1234567890;
	int invalid_data_bits = 100;
	int invalid_stop_bits = 100;
	
	// create a Serial object
	CPPUNIT_ASSERT_NO_THROW(serial = new Serial("/dev/ttyS0"));

	// configure device and check that it worked
	CPPUNIT_ASSERT_NO_THROW(serial->Config(baud, data_bits, stop_bits));
	CPPUNIT_ASSERT_EQUAL(baud, serial->Baud());
	CPPUNIT_ASSERT_EQUAL(data_bits, serial->DataBits());
	CPPUNIT_ASSERT_EQUAL(stop_bits, serial->StopBits());

	// configure device with invalid parameters and check that it fails
	CPPUNIT_ASSERT_THROW(serial->Config(invalid_baud, data_bits, stop_bits),
															runtime_error);
	CPPUNIT_ASSERT_THROW(serial->Config(baud, invalid_data_bits, stop_bits),
															runtime_error);
	CPPUNIT_ASSERT_THROW(serial->Config(baud, data_bits, invalid_stop_bits),
															runtime_error);
}


}
