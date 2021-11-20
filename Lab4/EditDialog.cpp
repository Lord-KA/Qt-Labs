#include "EditDialog.hpp"


EditDialog::EditDialog(Figure* f, QWidget* parent)
	: QDialog(parent), figure(f)
{

	QFrame* frame = new QFrame(this);
	frame->setFrameStyle(QFrame::Box);

	QLabel* lableW  = new QLabel(tr("Width"), frame);
	QLabel* lableH  = new QLabel(tr("Height"), frame);
	QLabel* lableX  = new QLabel(tr("X"), frame);
	QLabel* lableR  = new QLabel(tr("R"), frame);
	QLabel* lableP  = new QLabel(tr("P"), frame);
	QLabel* lableQ  = new QLabel(tr("Q"), frame);
	QLabel* lableDirection  = new QLabel(tr("Direction of rotation"), frame);
	QLabel* lableRotation   = new QLabel(tr("Rotation"), frame);
	QLabel* lablePerimeter  = new QLabel(tr("Perimeter"), frame);
	QLabel* lableSquare     = new QLabel(tr("Square"), frame);

	direction = new QComboBox(frame);
	direction->addItem(tr("Counterclockwise"));
	direction->addItem(tr("Clockwise"));

	width = new QSpinBox(frame);
	height = new QSpinBox(frame);
	x = new QSpinBox(frame);
	r = new QSpinBox(frame);
	p = new QSpinBox(frame);
	q = new QSpinBox(frame);
	rotation = new QSpinBox(frame);

	perimeterLine = new QLineEdit(frame);
	squareLine = new QLineEdit(frame);
	perimeterLine->setReadOnly(true);
	squareLine->setReadOnly(true);

	 width->setRange(60, 300);
	height->setRange(24, width->maximum() - 1);

	x->setRange(1, height->maximum() / 3 - 1);
	r->setRange(1, height->maximum() / 3 - 1);
	p->setRange(1, height->maximum() / 3 - 1);
	q->setRange(1,  width->maximum() / 4 - 1);

	rotation->setRange(0, 359);

	width->setValue(f->width);
	height->setValue(f->height);
	x->setValue(f->X);
	r->setValue(f->R);
	p->setValue(f->P);
	q->setValue(f->Q);
	rotation->setValue(f->Angle);
	squareLine->setText(QString::number(f->sqr()));
	perimeterLine->setText(QString::number(f->per()));

	QGridLayout* fl = new QGridLayout(frame);
	fl->addWidget(lableW, 0, 0);
	fl->addWidget(lableH, 1, 0);
	fl->addWidget(lableX, 2, 0);
	fl->addWidget(lableR, 3, 0);
	fl->addWidget(lableP, 4, 0);
	fl->addWidget(lableQ, 5, 0);
	fl->addWidget(lableDirection, 8, 0);
	fl->addWidget(lableRotation, 9, 0);
	fl->addWidget(lablePerimeter, 10, 0);
	fl->addWidget(lableSquare, 11, 0);
	fl->addWidget(width, 0, 1);
	fl->addWidget(height, 1, 1);
	fl->addWidget(x, 2, 1);
	fl->addWidget(r, 3, 1);
	fl->addWidget(p, 4, 1);
	fl->addWidget(q, 5, 1);
	fl->addWidget(direction, 8, 1);
	fl->addWidget(rotation, 9, 1);
	fl->addWidget(perimeterLine, 10, 1);
	fl->addWidget(squareLine, 11, 1);
	frame->setLayout(fl);

	QPushButton* accbutt = new QPushButton(tr("Accept"), this);
	connect(accbutt, SIGNAL(clicked()), this, SLOT(accept()));

	QGridLayout* l = new QGridLayout(this);
	l->addWidget(frame, 0, 0);
	l->addWidget(accbutt, 1, 0);
	setLayout(l);

	setModal(true);
}

EditDialog::~EditDialog() {}

void EditDialog::SetFigure(Figure *f)
{
	figure = f;
	if (figure)
	{
		width->setValue(f->width);
		height->setValue(f->height);
		x->setValue(f->X);
		r->setValue(f->R);
		p->setValue(f->P);
		q->setValue(f->Q);
		rotation->setValue(f->Angle);
		if (direction->currentText() != f->direction)
			direction->setCurrentIndex((direction->currentIndex() + 1) % 2);
		squareLine->setText(QString::number(f->sqr()));
		perimeterLine->setText(QString::number(f->per()));

		height->setRange(24, width->value() - 1);

		x->setRange(1, height->value() / 3 - 1);
		r->setRange(1, height->value() / 3 - 1);
		p->setRange(1, height->value() / 3 - 1);
		q->setRange(1,  width->value() / 4 - 1);
	}
}
