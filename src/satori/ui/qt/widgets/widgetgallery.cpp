
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>
#include "widgetgallery.h"


WidgetGallery::WidgetGallery(QWidget *parent) : QWidget(parent) {
	// checkbox
	QCheckBox *checkbox_on = new QCheckBox("On", this);
	QCheckBox *checkbox_on_hover = new QCheckBox("On Hover", this);
	QCheckBox *checkbox_off = new QCheckBox("Off", this);
	QCheckBox *checkbox_off_hover = new QCheckBox("Off Hover", this);
	QCheckBox *checkbox_disabled = new QCheckBox("Disabled", this);

	checkbox_on->setChecked(true);
	checkbox_on_hover->setChecked(true);
	checkbox_disabled->setEnabled(false);

	QVBoxLayout *checkbox_layout = new QVBoxLayout;
	checkbox_layout->addWidget(checkbox_on);
	checkbox_layout->addWidget(checkbox_on_hover);
	checkbox_layout->addWidget(checkbox_off);
	checkbox_layout->addWidget(checkbox_off_hover);
	checkbox_layout->addWidget(checkbox_disabled);

	QGroupBox *checkbox_group = new QGroupBox("QCheckBox");
	checkbox_group->setLayout(checkbox_layout);

	// radiobuttons
	QRadioButton *radio_on = new QRadioButton("On", this);
	QRadioButton *radio_on_hover = new QRadioButton("On Hover", checkbox_group);
	QRadioButton *radio_off = new QRadioButton("Off", this);
	QRadioButton *radio_off_hover = new QRadioButton("Off Hover", this);
	QRadioButton *radio_disabled = new QRadioButton("Disabled", this);

	radio_on->setChecked(true);
	radio_on_hover->setChecked(true);
	radio_disabled->setEnabled(false);

	QVBoxLayout *radio_layout = new QVBoxLayout;
	radio_layout->addWidget(radio_on);
	radio_layout->addWidget(radio_on_hover);
	radio_layout->addWidget(radio_off);
	radio_layout->addWidget(radio_off_hover);
	radio_layout->addWidget(radio_disabled);

	QGroupBox *radio_group = new QGroupBox("QRadioButton");
	radio_group->setLayout(radio_layout);

	// buttons
	QLabel *button_label = new QLabel("Normal", this);
	QLabel *button_flat_label = new QLabel("Flat", this);
	QPushButton *button_off = new QPushButton("Button", this);
	QPushButton *button_hover = new QPushButton("Button", this);
	QPushButton *button_pressed = new QPushButton("Button", this);
	QPushButton *button_disabled = new QPushButton("Button", this);
	QPushButton *button_flat_off = new QPushButton("Button", this);
	QPushButton *button_flat_hover = new QPushButton("Button", this);
	QPushButton *button_flat_pressed = new QPushButton("Button", this);
	QPushButton *button_flat_disabled = new QPushButton("Button", this);

	QLabel *button_off_label = new QLabel("Off", this);
	QLabel *button_hover_label = new QLabel("Hover", this);
	QLabel *button_pressed_label = new QLabel("Pressed", this);
	QLabel *button_disabled_label = new QLabel("Disabled", this);

	button_flat_off->setFlat(true);
	button_flat_hover->setFlat(true);
	button_flat_pressed->setFlat(true);
	button_flat_disabled->setFlat(true);
	
	button_pressed->setDown(true);
	button_flat_pressed->setDown(true);
	button_disabled->setDisabled(true);
	button_flat_disabled->setDisabled(true);

	QGridLayout *button_layout = new QGridLayout;
	button_layout->addWidget(button_label, 0, 0);
	button_layout->addWidget(button_off, 1, 0);
	button_layout->addWidget(button_hover, 2, 0);
	button_layout->addWidget(button_pressed, 3, 0);
	button_layout->addWidget(button_disabled, 4, 0);
	
	button_layout->addWidget(button_flat_label, 0, 1);
	button_layout->addWidget(button_flat_off, 1, 1);
	button_layout->addWidget(button_flat_hover, 2, 1);
	button_layout->addWidget(button_flat_pressed, 3, 1);
	button_layout->addWidget(button_flat_disabled, 4, 1);
	
	button_layout->addWidget(button_off_label, 1, 2);
	button_layout->addWidget(button_hover_label, 2, 2);
	button_layout->addWidget(button_pressed_label, 3, 2);
	button_layout->addWidget(button_disabled_label, 4, 2);

	QGroupBox *button_group = new QGroupBox("QPushButton", this);
	button_group->setLayout(button_layout);

	// main layout
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(button_group);
	layout->addWidget(checkbox_group);
	layout->addWidget(radio_group);

	setLayout(layout);
}
