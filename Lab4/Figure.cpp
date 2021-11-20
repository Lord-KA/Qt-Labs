#include "Figure.hpp"

static std::mt19937 rnd(179);

Figure::Figure(QWidget* parent)
	:QWidget(parent), Selected(false)
{

    Angle = rnd() % 361; 

	width = rnd() % 200 + 75;
	height = (rnd() % width + 100) % width + 30;

    X = (rnd() % height/3 + 50)  % height/3 + 1;
    Q = (rnd() % width/4  + 100) % width/4  + 1;
    R = (rnd() % height/3 + 50)  % height/3 + 1;
    P = (rnd() % height/3  + 50) % height/3 + 1; 

	contMenu = new QMenu(this);
 
 	   delAction = new QAction(tr("&Delete"));
	  editAction = new QAction(tr("&Edit"));
	  moveAction = new QAction(tr("&Move"));
	rotateAction = new QAction(tr("&Rotate"));

	contMenu->addAction(   delAction);
	contMenu->addAction(  editAction);
	contMenu->addAction(  moveAction);
	contMenu->addAction(rotateAction);

	connect(   delAction, &QAction::triggered, this, &Figure::Del);
	connect(  editAction, &QAction::triggered, this, &Figure::Edit);
	connect(  moveAction, &QAction::triggered, this, &Figure::Moving);
	connect(rotateAction, &QAction::triggered, this, &Figure::ShowSlider);

	std::size_t curX = 100 % (1280 - width);
	std::size_t curY = 100 % (720  - height);

    curY = fmax(curY, 50);

	setGeometry(curX, curY, sqrt(width * width + height * height) + 10, sqrt(width * width + height * height) + 10);
	setMinimumSize(sqrt(width * width + height * height), sqrt(width * width + height * height));
	
    QPalette Pal(palette());
	Pal.setColor(QPalette::Window, Qt::blue);           //TODO make Qt::transparent
	setAutoFillBackground(true);
	setPalette(Pal);

	repaint();
}

Figure::Figure(Figure &&other) 
{
    Selected = other.Selected;
    Deleted  = other.Deleted;
    type = other.type;
    width = other.width;
    height = other.height;
    X = other.X;
    R = other.R;
    P = other.P;
    Q = other.Q;
    Angle = other.Angle;
    contMenu = other.contMenu;
    delAction = other.delAction;
    editAction = other.editAction;
    moveAction = other.moveAction;
    rotateAction = other.rotateAction;
}

Figure& Figure::operator=(const Figure &other) 
{
    Selected = other.Selected;
    Deleted  = other.Deleted;
    type = other.type;
    width = other.width;
    height = other.height;
    X = other.X;
    R = other.R;
    P = other.P;
    Q = other.Q;
    Angle = other.Angle;
    contMenu = other.contMenu;
    delAction = other.delAction;
    editAction = other.editAction;
    moveAction = other.moveAction;
    rotateAction = other.rotateAction;
}

void Figure::Deselect()
{
	Selected = false;
}
void Figure::SetFigure45()
{
	type = figureType::figure45;
}

void Figure::SetFigure55()
{
	type = figureType::figure55;
}

void Figure::ShowSlider()
{
	Slider* obj = new Slider(this,this);
	obj->show();
}

void Figure::Moving()
{
	//QMouseEvent* event = new QMouseEvent(QEvent::MouseMove, pos(), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
	//mouseMoveEvent(event,true);
}

void Figure::Del()
{
	Deleted = true;
	close();
	repaint();
}

void Figure::Edit()             //TODO
{
	EditDialog edit(this, this);
	edit.SetFigure(this);
	if (edit.exec() == edit.Accepted)
	{
		width = edit.width->value();
		height = edit.height->value();
        
        X = edit.x->value();
        R = edit.r->value();
        P = edit.p->value();
		Q = edit.q->value();

		Angle     = edit.rotation->value();
        if (edit.direction->currentText() != direction) {
    		direction = edit.direction->currentText();
            Angle = 360 - Angle;
        }
	}
    
	setMinimumSize(sqrt(width * width + height * height), sqrt(width * width + height * height));

	repaint();
}

bool Figure::isDeleted()
{
	return Deleted;
}

bool Figure::isSelected()
{
	return Selected;
}
/*
void Figure::CalcSquare()
{
	qreal SqRect = width * height;
    
    std::cout << (int)type << '\n'; //TODO
    switch (type) {
    case (figureType::figure45):
	{
                                      //TODO
        using namespace ::figrPrmtr;
		qreal SqA = 0.5 * R1 * R1;
		qreal SqB = 0.5 * R2 * R2;
		qreal SqC = M_PI * R3 * R3 / 4;
		qreal SqD = 0.5 * X * X;
		qreal SqE = M_PI * Q1 * Q1 / 8;
		qreal SqF = M_PI * Q2 * Q2 / 8;
		// Square = SqRect - (SqA + SqB + SqC + SqD + SqE);
        
        break;
	}
    case (figureType::figure55):
	{
        
		qreal SqA = M_PI * R2 * R2;
		qreal SqB = M_PI * R3 * R3;
		qreal SqC = 0.5 * X * X;
		qreal SqD = M_PI * R1 * R1;
		qreal SqE = M_PI * Q1 * Q1 / 8;
		qreal SqF = M_PI * Q2 * Q2 / 8;
		Square = SqRect - (SqA + SqB + SqC + SqD + SqE + SqF);
        
        break;
	}
    default:
        return ;
        // assert(!"ERROR: no such figure type");
    }
}

void Figure::CalcPerimeter()    //TODO
{
}
*/


void Figure::paintEvent(QPaintEvent* event)
{
	QPainter p(this);

	resize(width, height);

	QPen pen;
	pen.setColor(Qt::black);
	pen.setWidth(3);

	if (Selected)
	{
		pen.setColor(Qt::darkBlue);
	}

	p.setPen(pen);
	p.translate(QPoint(sqrt(width * width + height * height) / 2, sqrt(width * width + height * height) / 2));

	if (direction == "Clockwise")
		p.rotate(Angle);
	else if (direction == "Counterclockwise")
		p.rotate(360 - Angle);
    else
        assert(!"ERROR: wrong direction string provided!");
    
    switch (type) {
    
    case (figureType::figure45):
		p.drawLine(X - width/2, -height/2, -P/2, -height/2);  // D-E      // Main body
		p.drawLine(P/2, -height/2, width/2 - X, -height/2);  // E-A
		
        p.drawLine(width/2, height/2 - X, width/2, X - height/2);  // A-B
        
        p.drawLine(width/2 - X, height/2, -width/2, height/2);  // B-C
        
        p.drawLine(-width/2, height/2, -width/2, -height/2 + X);  // C-D
        
		p.drawLine(-P/2, -height/2 + P/2, P/2, -height/2 + P/2);     // E-quirks
        p.drawLine(-P/2, -height/2, -P/2, -height/2 + P/2);
        p.drawLine(P/2, -height/2, P/2, -height/2 + P/2);
        
        p.drawLine(width/2 - X, -height/2, width/2, -height/2 + X); // A-quirks
        
        p.drawLine(width/2 - X, height/2 - X, width/2, height/2 - X); // B-quirks
        p.drawLine(width/2 - X, height/2 - X, width/2 - X, height/2);
        
        
        p.drawLine(-width/2 + X, -height/2 + X, -width/2, -height/2 + X); // D-quirks
        p.drawLine(-width/2 + X, -height/2 + X, -width/2 + X, -height/2);
        break;

    case (figureType::figure55):                
        
        p.drawLine(-width/2 + R, -height/2, width/2 - R, -height/2);        // D-A     // Main body
        
        p.drawLine(width/2, -height/2 + R, width/2, height/2 - R);          // A-B
        
        p.drawLine(Q/2, height/2, width/2 - R, height/2);                   // B-F
        p.drawLine(-Q/2, height/2, -width/2 + X, height/2);                 // F-C
        
        p.drawLine(-width/2, height/2 - X, -width/2, -height/2 + R);        // C-D
        
        p.drawLine(-width/2, height/2 - X, -width/2 + X, height/2);         // C-quirks

        p.drawArc((width - Q) / 2 - width/2, height - Q / 2 - height/2, Q, Q, 0, 180 * 16); // F-quirks

        p.drawArc(width/2 - R, height/2 - R, 2 * R, 2 * R, 90 * 16, 90 * 16);       // B-quirks
        
        p.drawArc(width/2 - R, -height/2 - R, 2 * R, 2 * R, 180 * 16, 90 * 16);       // A-quirks
        
        p.drawArc(-width/2 - R, -height/2 - R, 2 * R, 2 * R, 270 * 16, 90 * 16);       // D-quirks
        
        break;
	
    default: 
        assert(!"ERROR: no such figure to draw!");
    }   
}

void Figure::contextMenuEvent(QContextMenuEvent* event)
{
	contMenu->exec(event->globalPos());
}

void Figure::mousePressEvent(QMouseEvent* event)
{
	Selected = true;

	if (event->button() == Qt::LeftButton)
	{
		repaint();
	}

	signalBack();

	repaint();
}

void Figure::mouseMoveEvent(QMouseEvent* e)
{
	//if (flag)
		//setMouseTracking(true);
	move(e->windowPos().x(), e->windowPos().y());

	if (e->windowPos().x() > 1280 - geometry().width())
		move(1280 - geometry().width(), e->windowPos().y());
	if (e->windowPos().y() < 50)
		move(e->windowPos().x(), 50);
	if (e->windowPos().x() < 0)
		move(0, e->windowPos().y());
	if (e->windowPos().y() > 720 - geometry().height())
		move(e->windowPos().x(), 720 - geometry().height());

	if ((e->windowPos().x() >= 1280 - geometry().width()) && (e->windowPos().y() >= 720 - geometry().height()))
		move(1280 - geometry().width(), 720 - geometry().height());

	if ((e->windowPos().x() >= 1280 - geometry().width()) && (e->windowPos().y() <= 50))
		move(1280 - geometry().width(), 50);

	if ((e->windowPos().x() <= 0) && (e->windowPos().y() <= 50))
		move(0, 50);

	if ((e->windowPos().x() <= 0) && (e->windowPos().y() >= 720 - geometry().height()))
		move(0, 720 - geometry().height());


	update();
	//QWidget::repaint();
}

qreal Figure::sqr() const   //TODO
{
	return 0;
}

qreal Figure::per() const   //TODO
{
	return 0;
}

