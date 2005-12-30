
#ifndef MEMORYTXTWINDOW_H
#define MEMORYTXTWINDOW_H


#include <inttypes.h>
#include <boost/shared_ptr.hpp>
#include "satori/memory.h"


class MemoryTxtWindow {
public:
	MemoryTxtWindow(boost::shared_ptr<Memory> memory);
	void Read(uint32_t start_addr, uint32_t end_addr);
	void Write(uint32_t start_adr, uint32_t end_addr, const char *data);

private:
	void Show(uint32_t start_addr, uint32_t end_addr,unsigned char *data) const;

	static const uint32_t data_buf_size = 0x400;
	unsigned char data[data_buf_size];
	boost::shared_ptr<Memory> memory;
};


#endif
