
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "memorytxtwindow.h"


/**
 * Create memory 'window' wrapping a memory object.
 */
MemoryTxtWindow::MemoryTxtWindow(boost::shared_ptr<Memory> memory) {
	this->memory = memory;
}


/**
 *
 */
void MemoryTxtWindow::Read(uint32_t start_addr, uint32_t end_addr) {
	// check arguments for correctness
	if ((start_addr > end_addr) ||
		(start_addr > memory->End()) ||
		(end_addr < memory->Start()))
		return;

	start_addr = std::max(start_addr, memory->Start());
	end_addr = std::min(end_addr, memory->End());

	// read and display data in chunks of buffer size
	uint32_t data_size = end_addr - start_addr + 1;
	while (data_size > 0) {
		/// @bug For some reason the plugin breaks when using std::min with
		///      uint32_t types. Since we're probably going to replace the
		///      uint32_t types with our own and remove the inttypes.h /
		///		 stdint.h headers (for platform independence) it will be solved
		///		 later. In the meantime we cast the data_size and data_buf_size
		///		 to unsigned int.
		uint32_t read_size = std::min(static_cast<unsigned int>(data_size),
									  static_cast<unsigned int>(data_buf_size));

		uint32_t res = 0;
		while (res < read_size) {
			uint32_t r;
			if ( (r = memory->Read(start_addr, start_addr+read_size-1,
								reinterpret_cast<char *>(data))) == 0)
				/// @todo Probably throw exception here.
				return;

			res += r;
		} 

		Show(start_addr, start_addr+read_size-1, data);

		data_size -= read_size;
		start_addr += read_size;
	}
}


/**
 *
 */
void MemoryTxtWindow::Write(uint32_t start_addr, uint32_t end_addr, const char *data) {
	// check arguments for correctness
	if ((start_addr > end_addr) ||
		(start_addr > memory->End()) ||
		(end_addr < memory->Start()))
		return;

	if (start_addr < memory->Start())
		start_addr = memory->Start();

	if (end_addr > memory->End())
		end_addr = memory->End();

	memory->Write(start_addr, end_addr, data);
}


/**
 * Display memory data.
 *
 * @param start_addr			start memory address
 * @param end_addr				end memory address
 * @param data					pointer to data buffer
 */
void MemoryTxtWindow::Show(uint32_t start_addr, uint32_t end_addr, unsigned char *data) const {
	uint32_t data_size = end_addr - start_addr + 1;

	// configure cout
	std::cout.fill('0');
	std::cout << std::hex << std::uppercase;

	// display data
	for (uint32_t r=0; r<data_size; r+=0x10) {
		// address
		std::cout << ": " << std::setw(4) <<  start_addr + r << "  ";
								        
		// hex data
		for (uint32_t c=0; c<0x10 && c<data_size-r; c++)
			std::cout << std::setw(2) << (unsigned int)(data[r+c])
					  << ((c==7) ? "  " : " ");
										        
		// align pointer for ascii output
		if (data_size-r < 0x10) {
			for (uint32_t i=0; i<0x10-(data_size-r); i++)
				std::cout << "   " << ((i==7)?" ":"");
		}
		std::cout << "   ";

		// ascii data
		for (uint32_t c=0; c<0x10 && c<data_size-r; c++)
			std::cout << (char)((data[r+c]>=0x20 && data[r+c]<=0x7E) ?
													data[r+c] : '.');

		std::cout << std::endl;
	}
}
