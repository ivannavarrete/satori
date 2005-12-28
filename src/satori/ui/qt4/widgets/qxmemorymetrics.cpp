
#include "qxmemorymetrics.h"


QxMemoryMetrics::QxMemoryMetrics(const QFontMetrics &font_metrics) {
	int char_width = font_metrics.maxWidth();
	int char_height = font_metrics.height();

	v_indent = char_height;
	h_indent = 6;

	cell_height = char_height - 2;
	addr_cell_width = static_cast<int>(char_width*4.5);
	hex_cell_width = static_cast<int>(char_width*2.5);
	ascii_cell_width = char_width;

	block_space = char_width;
	hex_mid_space = static_cast<int>(hex_cell_width*0.2);

	hex_indent = h_indent + addr_cell_width + block_space;
	ascii_indent = hex_indent + hex_cell_width*16 + hex_mid_space + block_space;
}


const QRect &QxMemoryMetrics::AddrRect(int row) {
	
}


const QRect &QxMemoryMetrics::HexRect(int row, int col) {
	
}


const QRect &QxMemoryMetrics::AsciiRect(int row, int col) {

}
