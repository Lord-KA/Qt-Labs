#include "Slider.hpp"


Slider::Slider(Figure* figure, QWidget *parent)
	: QDialog(parent), figure(figure)
{
	AngleSlider = new QSlider(Qt::Horizontal,this);
	AngleSlider->show();
	
    AngleSlider->setMaximum(180);
	AngleSlider->setMinimum(-180);
	AngleSlider->setValue(0);

	layout = new QHBoxLayout(this);
	layout->addWidget(AngleSlider);
	setLayout(layout);
	connect(AngleSlider, &QSlider::valueChanged, this, &Slider::changeAngle);
}

Slider::~Slider()
{
    if (AngleSlider)
        delete AngleSlider;
    if (layout)
        delete layout;
}

void Slider::changeAngle()
{
	figure->Angle = AngleSlider->value() + 180;
	figure->update();
}
