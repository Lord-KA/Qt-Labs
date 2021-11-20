#ifndef GFIGURE_HPP
#define GFIGURE_HPP

#include <QWidget>
#include <QMenu>
#include <QVariantAnimation>
#include <QPainter>
#include <QMouseEvent> 
#include <QContextMenuEvent>
#include <QPixmap>

#include <cmath>  
#include <numeric>
#include <cassert>
#include <random>

#include "Slider.hpp"
#include "EditDialog.hpp"


enum class figureType {
    none = 0,
    figure45,
    figure55
};


class Figure : public QWidget
{
	Q_OBJECT
    friend class Slider;
    friend class EditDialog;
    friend class lab4;


public:

	explicit Figure(QWidget* parent = nullptr);
	
    Figure(Figure &&other);
    Figure& operator=(const Figure &other);

	void Deselect();

	qreal sqr() const;
	qreal per() const;

	QString dir() const;
	QString& dir();

	void SetFigure45();
	void SetFigure55();

	void ShowSlider();
	void Moving();
	void Del();
	void Edit();
	bool isDeleted();
	bool isSelected();

	// void CalcSquare();
	// void CalcPerimeter();

protected:
	void paintEvent(QPaintEvent* event);
	void contextMenuEvent(QContextMenuEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);

signals:
	void signalBack();

private:
	bool Selected = false;
    bool Deleted  = false;

    figureType type = figureType::none;

    size_t width, height;
    	
    size_t X, R, P, Q, Angle;

	QMenu   *contMenu;
	QAction *delAction, *editAction, *moveAction, *rotateAction;
	QString  direction = "Clockwise";
};


#endif
