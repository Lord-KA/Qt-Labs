#ifndef EDIT_HPP
#define EDIT_HPP

#include "Figure.hpp"

#include <QDialog>
#include <QSpinBox>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include <QFrame>
#include <QComboBox>
#include <QLineEdit>


class EditDialog : public QDialog
{
	Q_OBJECT

public:
	EditDialog(class Figure* f, QWidget* parent = nullptr);
	~EditDialog();

	void SetFigure(class Figure* f);

	QSpinBox *width, *height, *x, *p, *r, *q, *rotation;
	QComboBox *direction;
	QLineEdit *perimeterLine, *squareLine;

private:
	class Figure* figure;
};

#endif
