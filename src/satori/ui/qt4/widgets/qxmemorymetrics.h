
#ifndef QXMEMORYMETRICS
#define QXMEMORYMETRICS


class QxMemoryMetrics {
public:
	QxMemoryMetrics(const QFontMetrics &font_metrics);

	const QRect &AddrRect(int row);
	const QRect &HexRect(int row, int col);
	const QRect &AsciiRect(int row, int col);

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
};



#endif
