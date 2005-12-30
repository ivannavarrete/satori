
#ifndef QXMEMORYMETRICS
#define QXMEMORYMETRICS


#include <QFontMetrics>
//#include "qxmemory.h"


class QxMemoryMetrics {
public:
	enum SubWindow { NoWindow, HexWindow, AsciiWindow };

	QxMemoryMetrics(const QFontMetrics &font_metrics);

	const QRect &AddrCell(int row);
	const QRect &HexCell(int row, int col);
	const QRect &AsciiCell(int row, int col);
	SubWindow CellIndex(int x, int y, int &row, int &col) const;

private:
	int v_indent;
	int h_indent;

	int cell_height;
	int addr_cell_width;
	int hex_cell_width;
	int ascii_cell_width;

	int block_space;
	int hex_mid_space;

	int hex_indent;
	int ascii_indent;

	QRect cell;
};


#endif
