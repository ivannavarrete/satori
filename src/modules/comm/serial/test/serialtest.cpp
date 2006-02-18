
#include <QObject>
#include <stdexcept>
#include "test/suitename.h"
#include "test/signalcatcher.h"
#include "serialtest.h"


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SerialTest, SuiteName::Serial());

extern SignalCatcher signal_catcher;


/**
 * Serial object constructor tests.
 */
void SerialTest::Constructor() {
	// create a Serial object
	CPPUNIT_ASSERT_NO_THROW(Serial("/dev/ttyS0"));

	// create a Serial object from a nonexistent file
	CPPUNIT_ASSERT_THROW(Serial("f00bar"), std::runtime_error);

	// create a Serial object from a file that is not a character device
	CPPUNIT_ASSERT_THROW(Serial("bogus_serial_dev"), std::runtime_error);

	/// @Todo Implement test to create a Serial object from a character device
	/// file that can't be opened (no permission for example).

	/// @Todo Implement test to create a Serial object that can't determine
	/// and save it's mode.
}


/**
 * Test that the Serial object is properly destroyed (device flushed, restored
 * and closed).
 */
void SerialTest::Destructor() {
	Serial *serial = NULL;

	// create a Serial object
	CPPUNIT_ASSERT_NO_THROW(serial = new Serial("/dev/ttyS1"));

	// connect error signal to the signal catcher
	QObject::connect(serial, SIGNAL(ErrorSignal(std::string)),
					 &signal_catcher, SLOT(StringSlot(std::string)),
					 Qt::DirectConnection);

	// destroy object and check that no signals were received
	delete serial;
	CPPUNIT_ASSERT(signal_catcher.StringSignalsReceived(0));
}



void SerialTest::SetBaud() {
	Serial *serial = NULL;
	unsigned int baud_rates[] = { 50, 75, 110, 134, 150, 200, 300, 600, 1200,
								  1800, 2400, 4800, 9600, 19200, 38400, 57600,
								  115200, /* 230400 */ 0};

	// create serial object
	CPPUNIT_ASSERT_NO_THROW(serial = new Serial("/dev/ttyS1"));

	// perform baud setting tests
	for (unsigned int i=0; baud_rates[i]!=0; i++) {
		CPPUNIT_ASSERT_NO_THROW(serial->SetBaud(baud_rates[i]));
		CPPUNIT_ASSERT_EQUAL(baud_rates[i], serial->Baud());
	}

	CPPUNIT_ASSERT_THROW(serial->SetBaud(123456789), std::runtime_error);

	// destroy serial object
	delete serial;
	CPPUNIT_ASSERT(signal_catcher.StringSignalsReceived(0));
}


void SerialTest::SetDataBits() {
	Serial *serial = NULL;
	unsigned int data_bits[] = { 5, 6, 7, 8, 0};

	// create serial object
	CPPUNIT_ASSERT_NO_THROW(serial = new Serial("/dev/ttyS1"));

	// perform data bit setting tests
	for (unsigned int i=0; data_bits[i]!=0; i++) {
		CPPUNIT_ASSERT_NO_THROW(serial->SetDataBits(data_bits[i]));
		CPPUNIT_ASSERT_EQUAL(data_bits[i], serial->DataBits());
	}

	CPPUNIT_ASSERT_THROW(serial->SetDataBits(100), std::runtime_error);

	// destroy serial object
	delete serial;
	CPPUNIT_ASSERT(signal_catcher.StringSignalsReceived(0));
}


void SerialTest::SetStopBits() {
	Serial *serial = NULL;
	unsigned int stop_bits;

	// create serial object
	CPPUNIT_ASSERT_NO_THROW(serial = new Serial("/dev/ttyS1"));

	// perform stop bit setting tests
	stop_bits = 1;
	CPPUNIT_ASSERT_NO_THROW(serial->SetStopBits(stop_bits));
	CPPUNIT_ASSERT_EQUAL(stop_bits, serial->StopBits());

	stop_bits = 2;
	CPPUNIT_ASSERT_NO_THROW(serial->SetStopBits(stop_bits));
	CPPUNIT_ASSERT_EQUAL(stop_bits, serial->StopBits());

	CPPUNIT_ASSERT_THROW(serial->SetStopBits(100), std::runtime_error);

	// destroy serial object
	delete serial;
	CPPUNIT_ASSERT(signal_catcher.StringSignalsReceived(0));
}
