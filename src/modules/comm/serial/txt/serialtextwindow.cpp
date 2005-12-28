
#include "serialtextwindow.h"


SerialTextWindow::SerialTextWindow() {
}


void SerialTextWindow::Info() const {
	std::cout << "] baud: " << serial.Baud() << "\n"
			  << "] data_bits: " << serial.DataBits() << "\n"
			  << "] stop_bits: " << serial.StopBits() << endl;
}
