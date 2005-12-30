
#ifndef MEMORYQTWINDOW_H
#define MEMORYQTWINDOW_H


#include <QWidget>
#include <QEvent>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QColor>
#include <boost/shared_ptr.hpp>
#include "qxmemorymetrics.h"
#include "satori/memory.h"


#include <QAction>
#include <QMenu>


class QxMemory : public QWidget {
	Q_OBJECT

public:
	QxMemory(boost::shared_ptr<Memory> memory, QWidget *parent=0);

protected:
	bool event(QEvent *e);
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);

public slots:
	void Refresh();

private slots:
	void Goto();
	void Find();

private:
	void changeCellPos(int amount);

	// popup menu for this widget
	QMenu *popup_menu;

	// window, cell and font metrics
	int char_width;
	int char_height;
	
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

	// data buffers
	static const uint32_t data_buf_size = 0x400;
	unsigned char data[data_buf_size];
	unsigned char cached_data[data_buf_size];

	/// subwindow type and coordinates for selected cell
	QxMemoryMetrics::SubWindow sub_window;
	int cell_row;
	int cell_col;

	/// flag that tells if a cell is under modification
	bool modifying_cell;

	QColor select_color;
	QColor change_color;

	boost::shared_ptr<Memory> memory;

	unsigned int window_start_addr;
	unsigned int window_end_addr;

	boost::shared_ptr<QxMemoryMetrics> mm;
};


#endif
