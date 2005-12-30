
#include "qxmemorymetrics.h"


QxMemoryMetrics::QxMemoryMetrics(const QFontMetrics &font_metrics) {
	int char_width = font_metrics.maxWidth();
	int char_height = font_metrics.height();

	v_indent = 6;
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


/**
 *
 */
const QRect &QxMemoryMetrics::AddrCell(int row) {
	cell.setRect(h_indent, v_indent+row*cell_height,
						addr_cell_width, cell_height);

	return cell;
}


/**
 *
 */
const QRect &QxMemoryMetrics::HexCell(int row, int col) {
	cell.setRect(hex_indent+col*hex_cell_width+((col>7)?hex_mid_space:0),
				v_indent+row*cell_height, hex_cell_width, cell_height);

	return cell;
}


/**
 *
 */
const QRect &QxMemoryMetrics::AsciiCell(int row, int col) {
	cell.setRect(ascii_indent+col*ascii_cell_width, v_indent+row*cell_height,
												ascii_cell_width, cell_height);

	return cell;
}


/**
 *
 */
QxMemoryMetrics::SubWindow QxMemoryMetrics::CellIndex(int x, int y, int &row, int &col) const {
	if (x > hex_indent && x < ascii_indent - block_space) {
		if (x - hex_indent > hex_cell_width*8)
			x -= hex_mid_space;

		row = (y - v_indent) / cell_height;
		col = (x - hex_indent) / hex_cell_width;
		
		return HexWindow;
	} else if (x > ascii_indent && x < ascii_indent + ascii_cell_width*16) {
		row = (y - v_indent) / cell_height;
		col = (x - ascii_indent) / ascii_cell_width;
		
		return AsciiWindow;
	}

	return NoWindow;
}


/*
int QxMemoryMetrics::Row(int y) const {
	return (y - v_indent) / cell_height;
}


int QxMemoryMetrics::Col(int x) const {
	
}


SubWindow QxMemoryMetrics::SubWindow(int x) const {
	if (x > hex_indent && x < (ascii_indent - block_space))
		return HexWindow;
}
*/
