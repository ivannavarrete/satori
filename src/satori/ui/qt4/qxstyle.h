
#ifndef QXSTYLE_H
#define QXSTYLE_H


#include <QWindowsStyle>
#include <QStyleOption>
#include <QPainter>


class QxStyle : public QWindowsStyle {
	Q_OBJECT

public:
	virtual void polish(QPalette &palette);
	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption
				*option, QPainter *painter, const QWidget *widget = 0) const;
	virtual void drawControl(ControlElement element, const QStyleOption *option,
							QPainter *painter, const QWidget *widget = 0) const;
	virtual int pixelMetric(PixelMetric metric, const QStyleOption *option = 0,
											const QWidget *widget = 0) const;

private:
	QColor gradient_color_start;
	QColor gradient_color_stop;
};

#endif
