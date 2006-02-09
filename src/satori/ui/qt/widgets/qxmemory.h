
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

	boost::shared_ptr<Memory> memory;
	boost::shared_ptr<QxMemoryMetrics> mm;

	// popup menu for this widget
	QMenu *popup_menu;

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

	unsigned int window_start_addr;
	unsigned int window_end_addr;
};


#endif
