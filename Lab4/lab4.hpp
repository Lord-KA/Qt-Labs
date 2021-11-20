#ifndef LAB_4_HPP
#define LAB_4_HPP

#include "Figure.hpp"

#include <QVector>
#include <QPushButton>
#include <QMenuBar>
#include <QCoreApplication>
#include <QContextMenuEvent>
#include <QToolBar>
#include <QActionGroup>
#include <QPainter>
#include <QMainWindow>


class lab4 : public QMainWindow
{
	Q_OBJECT

public:
	lab4(QWidget *parent = nullptr);
	void activateDel();

public slots:
	void Add();
	void Delete();

protected:
	void contextMenuEvent(QContextMenuEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);

private:
	QMenu *mainMenu;
	QAction *actionQuit, *actionFigure45, *actionFigure55, *actionAdd, *actionDelete;
    QToolBar *toolbar;
    
    std::vector<Figure> Figures;
};

#endif
