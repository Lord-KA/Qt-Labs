#include "lab4.hpp"

lab4::lab4(QWidget* parent)
	: QMainWindow(parent)
{
	mainMenu   = menuBar()->addMenu(tr("&File"));
	actionQuit = mainMenu->addAction(tr("&Quit"), QCoreApplication::instance(), SLOT(quit()));

	setFixedSize(1280, 720);
	toolbar = new QToolBar(this);
	actionAdd = toolbar->addAction("&Add");
	actionDelete = toolbar->addAction("&Delete");
	actionDelete->setEnabled(false);
	toolbar->addSeparator();
	actionFigure45 = toolbar->addAction("&Figure 45");
	actionFigure55 = toolbar->addAction("&Figure 55");
	QActionGroup* group = new QActionGroup(this);
	group->addAction(actionFigure45);
	group->addAction(actionFigure55);
	actionFigure45->setCheckable(true);
	actionFigure45->setChecked(true);
	actionFigure55->setCheckable(true);

	connect(actionFigure45, SIGNAL(triggered()), this, SLOT(Figure::SetFigure45()));
	connect(actionFigure55, SIGNAL(trigerred()), this, SLOT(Figure::SetFigure55()));
	connect(actionAdd, SIGNAL(triggered()), this, SLOT(Add()));
	connect(actionDelete, SIGNAL(triggered()), this, SLOT(Delete()));
}

void lab4::Add()
{
	Figures.emplace_back(this);

	if (actionFigure45->isChecked())
		Figures.back().SetFigure45();
	else if (actionFigure55->isChecked())
		Figures.back().SetFigure55();

	connect(&Figures.back(), &Figure::signalBack, this, &lab4::activateDel);
	repaint();
}

void lab4::activateDel()
{
	actionDelete->setEnabled(true);
}

void lab4::Delete()
{
	for (auto &elem : Figures)
	{
		if (elem.isSelected()) 
			elem.Del();
	}
	actionDelete->setEnabled(false);
    auto iter = remove_if(Figures.begin(), Figures.end(), [](const Figure &elem){return elem.Deleted;});
    Figures.erase(iter, Figures.end());
}

void lab4::contextMenuEvent(QContextMenuEvent* event)
{
	mainMenu->exec(event->globalPos());
}

void lab4::mousePressEvent(QMouseEvent* event)
{
	for (auto &elem : Figures)
		elem.Deselect();
	actionDelete->setEnabled(false);
	repaint();
}

void lab4::paintEvent(QPaintEvent* event)
{
	QPainter p(this);
	std::size_t cnt{ 0 };
	for (auto &elem : Figures)
	{
		if (elem.isSelected())
			++cnt;
	}

	for (auto &elem : Figures)
	{
		if (cnt >= 2)
			elem.Deselect();

		if (!elem.isDeleted())
		{
			elem.update();
			elem.show();
		}
	}
}
