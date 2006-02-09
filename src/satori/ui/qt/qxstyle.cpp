
#include "qxstyle.h"

#include <QPoint>

#include <QLinearGradient>


/**
 * Set the default palette for this style.
 *
 * @param palette			palette to initialize
 */
void QxStyle::polish(QPalette &palette) {
	//QColor background_color(40, 40, 40);
	//QColor foreground_color(173, 173, 173);
	QColor color;

	// colors for gradient widgets
	gradient_color_start.setRgb(120, 120, 120);
	gradient_color_stop.setRgb(40, 40, 40);

	//palette = QPalette(QColor(40, 40, 40));

	// active color group
	color.setRgb(173, 173, 173);
	palette.setColor(QPalette::Active, QPalette::Foreground, color);
	
	color.setRgb(10, 10, 10);
	palette.setColor(QPalette::Active, QPalette::Background, color);

	color.setRgb(173, 173, 173);
	palette.setColor(QPalette::Active, QPalette::Button, color);

	color.setRgb(0, 0, 0);
	palette.setColor(QPalette::Active, QPalette::ButtonText, color);

	color.setRgb(20, 20, 20);
	palette.setColor(QPalette::Active, QPalette::Light, color);

	color.setRgb(60, 60, 60);
	palette.setColor(QPalette::Active, QPalette::Midlight, color);

	color.setRgb(30, 30, 30);
	palette.setColor(QPalette::Active, QPalette::Mid, color);

	color.setRgb(8, 8, 8);
	palette.setColor(QPalette::Active, QPalette::Dark, color);

	color.setRgb(5, 5, 5);
	palette.setColor(QPalette::Active, QPalette::Shadow, color);

	// inactive color group
	color.setRgb(10, 10, 10);
	palette.setColor(QPalette::Inactive, QPalette::Background, color);

	color.setRgb(173, 173, 173);
	palette.setColor(QPalette::Inactive, QPalette::Foreground, color);

	// disabled color group
}


void QxStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
	
	switch (element) {
	case PE_IndicatorArrowUp:
	case PE_IndicatorArrowDown:
	case PE_IndicatorArrowLeft:
	case PE_IndicatorArrowRight: {
		painter->save();

		// points for arrow
		QPoint points[4];
		points[0] = QPoint(-3, 2);
		points[1] = QPoint(3, 2);
		points[2] = QPoint(0, -3);
		points[3] = QPoint(-3, 2);
		
		// position painter in the middle
		painter->translate(option->rect.x()+option->rect.width()/2,
						   option->rect.y()+option->rect.height()/2);

		// rotade depending on the orientation of the arrow
		switch (element) {
		case PE_IndicatorArrowDown:
			painter->rotate(180);
			break;
		case PE_IndicatorArrowLeft:
			painter->rotate(90);
			break;
		case PE_IndicatorArrowRight:
			painter->rotate(270);
			break;
		default:
			break;
		}


		//
		//if (option->state & State_Sunken)
		//	painter->translate(pixelMetric(PM_ButtonShiftHorizontal),
		//					   pixelMetric(PM_ButtonShiftVertical));



		// draw arrow
		painter->setPen(option->palette.color(QPalette::Dark));
		painter->drawPolyline(points, 4);

		painter->restore();
		break;
	}
	default:
		QWindowsStyle::drawPrimitive(element, option, painter, widget);
	}
}


void QxStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
	QBrush brush;

	int x, y, width, height;
	option->rect.getRect(&x, &y, &width, &height);

	QLinearGradient gradient(x, y, x, y+height);
	gradient.setColorAt(0, gradient_color_start);
	gradient.setColorAt(1, gradient_color_stop);

	switch (element) {
	case CE_ScrollBarSlider:
		painter->save();
		
		// draw gradient background
		painter->fillRect(option->rect, QBrush(gradient));

		// draw frame and three horizontal lines in the middle
		painter->setPen(option->palette.foreground().color().dark(110));
		painter->drawRect(x, y, width-1, height-1);
		painter->drawLine(x+4, y+height/2, x+width-4, y+height/2);
		painter->drawLine(x+4, y+height/2-2, x+width-4, y+height/2-2);
		painter->drawLine(x+4, y+height/2+2, x+width-4, y+height/2+2);

		painter->restore();
		break;
	default:
		QWindowsStyle::drawControl(element, option, painter, widget);
	}
}



int QxStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const {
	switch (metric) {
	case PM_ScrollBarExtent:
		return 14;
		break;
	case PM_ScrollBarSliderMin:
		return 40;
		break;
	default:
		return QWindowsStyle::pixelMetric(metric, option, widget);
	}
}
