#ifndef GSLIDER_HPP
#define GSLIDER_HPP

#include <QDialog>
#include <QSlider>
#include <QHBoxLayout>

#include "Figure.hpp"


class Slider : public QDialog
{
	Q_OBJECT

public:
	Slider(class Figure *f, QWidget *parent);
	~Slider();
	void changeAngle();

private:
	QSlider     *AngleSlider = nullptr;
    QHBoxLayout *layout      = nullptr;
	class Figure *figure     = nullptr;
};

#endif
