
#include <QtGlobal>
#include <QPainter>
#include <QFontMetrics>
#include "qxmemory.h"


/**
 *
 */
QxMemory::QxMemory(boost::shared_ptr<Memory> memory, QWidget *parent) : QWidget(parent) {
	/*
	// create popup menu
	popup_menu = new QMenu(this);
	QAction *action;

	action = new QAction("Go to", popup_menu);
	connect(action, SIGNAL(triggered()), this, SLOT(Goto()));
	popup_menu->addAction(action);
	
	action = new QAction("Refresh", popup_menu);
	connect(action, SIGNAL(triggered()), this, SLOT(Refresh()));
	popup_menu->addAction(action);

	popup_menu->addSeparator();

	action = new QAction("Find", popup_menu);
	connect(action, SIGNAL(triggered()), this, SLOT(Find()));
	popup_menu->addAction(action);
	*/
	
	// grab memory object
	this->memory = memory;

	// determine window metrics
	mm = boost::shared_ptr<QxMemoryMetrics>(new QxMemoryMetrics(fontMetrics()));

	// set policy for keyboard focus
	setFocusPolicy(Qt::ClickFocus);

	// set widget colors
	change_color.setRgb(255, 187, 62);
	select_color.setRgb(40, 40, 140);

	memcpy(data+18, "hello world", 12);

	// deselect cells
	sub_window = QxMemoryMetrics::NoWindow;
	cell_row = -1;
	cell_col = -1;

	// 
	modifying_cell = false;

	// set widget size
	//int window_width = h_indent*2 + addr_cell_width + hex_cell_width*16 +
	//				   hex_mid_space + block_space*2 + ascii_cell_width*16;
	//setMinimumSize(window_width, 500);
	//setMaximumSize(window_width, 600);
	setMinimumSize(500, 500);
	setMaximumSize(500, 600);

	window_start_addr = 0x0000;
	//window_end_addr = qMin(memory->End(), static_cast<uint32_t>
	//				(16*((height()-h_indent*2)/cell_height))+16);
	window_end_addr = 0x0200;
}


/**
 * Event handling. We override the default to intercept all keyboard keys and
 * send them to keyPressEvent(). The default event() processes the tab key
 * before calling keyPressEvent(). All other events are dispatched through
 * the default event() handler.
 */
bool QxMemory::event(QEvent *event) {
	if (event->type() == QEvent::KeyPress) {
		keyPressEvent(static_cast<QKeyEvent *>(event));
		return true;
	}
	
	return QWidget::event(event);
}


void QxMemory::paintEvent(QPaintEvent * /*event*/) {
	QString str;
	QPainter painter(this);

	int addr = 0x0100;
	unsigned int a = qMin(memory->End()-addr, window_end_addr);

	qDebug("%x %x", addr, a);

	for (unsigned int row=0; row<=(a/16); ++row){
		// draw address
		str = QString("%1").arg(addr+row*0x10, 4, 16, QLatin1Char('0'));
		painter.drawText(mm->AddrCell(row), str);

		for (unsigned int col=0; col<16; ++col) {
			unsigned char c = data[row*16 + col];

			// draw changed data bold and in a different color
			if (c != cached_data[row*16 + col]) {
				painter.save();
				painter.setPen(change_color);

				QFont f = font();
				f.setBold(true);
				painter.setFont(f);
			}
				
			// draw hex data
			str = QString("%1").arg(c, 2, 16, QLatin1Char('0'));
			painter.drawText(mm->HexCell(row, col), str);

			// draw ascii data
			str = QString("%1").arg((char)(QChar(c).isPrint() ? c : '.'));
			painter.drawText(mm->AsciiCell(row, col), str);
		
			if (c != cached_data[row*16 + col])
				painter.restore();
		}
	}

	// draw selected hex cell
	if (sub_window == QxMemoryMetrics::HexWindow) {
		painter.setBackground(QBrush(select_color));
		painter.drawRoundRect(mm->HexCell(cell_row, cell_col), 45, 45);
	
	// draw selected ascii cell
	} else if (sub_window == QxMemoryMetrics::AsciiWindow) {
		painter.setBackground(QBrush(select_color));
		painter.drawRoundRect(mm->AsciiCell(cell_row, cell_col), 45, 45);
	}
}


void QxMemory::mousePressEvent(QMouseEvent *event) {
	// calculate row and column of the cell that received a click (if any)
	if (event->button() == Qt::LeftButton) {
		sub_window = mm->CellIndex(event->x(), event->y(), cell_row, cell_col);
		modifying_cell = false;

	// display popup menu
	} else if (event->button() == Qt::RightButton) {
		popup_menu->exec(event->globalPos());
	
	// other mouse events go to default handler
	} else {
		event->ignore();
		QWidget::mousePressEvent(event);
	}
	
	event->accept();
	update();
}


/**
 * Overloaded keyboard event handling method.
 */
void QxMemory::keyPressEvent(QKeyEvent *event) {
	// tab key switches between hex and ascii subwindows
	if (event->key() == Qt::Key_Tab) {
		if (sub_window == QxMemoryMetrics::HexWindow)
			sub_window = QxMemoryMetrics::AsciiWindow;
		else if (sub_window == QxMemoryMetrics::AsciiWindow)
			sub_window = QxMemoryMetrics::HexWindow;

		modifying_cell = false;

	// arrow keys change cell position
	} else if (event->key() == Qt::Key_Up) {
		changeCellPos(-16);
	} else if (event->key() == Qt::Key_Down) {
		changeCellPos(16);
	} else if (event->key() == Qt::Key_Right) {
		changeCellPos(1);
	} else if (event->key() == Qt::Key_Left) {
		changeCellPos(-1);

	// write characters to memory, if they turn out to be valid.. since this
	// clause doesn't check event->key() it must be the last one accepting keys
	} else if (event->count() == 1) {
		char c = 0;
		bool ok = false;

		// write in hex cell (characters [0-9,a-f,A-F])
		if (sub_window == QxMemoryMetrics::HexWindow) {
			char d = data[cell_row*16 + cell_col];
			c = event->text().toInt(&ok, 16);
			if (ok) {
				if (!modifying_cell) {
					c = (c<<4) | (d&0x0F);
					modifying_cell = true;
				} else {
					c = c | (d&0xF0);
					modifying_cell = false;
				}
			}

		// write printable characters in ascii cell
		} else if (sub_window == QxMemoryMetrics::AsciiWindow) {
			c = event->text().at(0).cell();
			if (QChar(c).isPrint()) {
				ok = true;
			}
		}
				
		// write key to data buffer
		if (ok) {
			data[cell_row*16 + cell_col] = c;

			// advance cell position if we're done modifying cell
			if (!modifying_cell)
				changeCellPos(1);
		}

	// all other keys are ignored
	} else {
		event->ignore();
		return;
	}

	event->accept();
	update();
}


void QxMemory::changeCellPos(int amount) {
	cell_col += amount;

	if (cell_col > 15) {
		cell_row += cell_col / 16;
		cell_col = cell_col % 16;
	} else if (cell_col < 0) {
		cell_row -= (qAbs(cell_col+1) / 16) + 1;
		cell_col = 15 - (qAbs(cell_col+1) % 16);
	}

	modifying_cell = false;
}


void QxMemory::Refresh() {
	qDebug("Refresh");
	memory->Read(0, 0x100, (char *)data);
	update();
}


void QxMemory::Goto() {
	qDebug("Goto");
}


void QxMemory::Find() {
	qDebug("Find");
}
