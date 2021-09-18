#include <random>
#include <QPropertyAnimation>
#include <QTimer>

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> dist(1, 1000000);

class MyWindow : public QWidget {
 private:
	static constexpr size_t hight = 25;
	static constexpr size_t width = 50;

	QPushButton *quitBtn;
	QPushButton *Btn2;    
	QPushButton *Btn3;    
	QPushButton *Btn4;    

	size_t Btn2_x = 100;
	size_t Btn2_y = 80;
	size_t Btn3_x = 240;
	size_t Btn3_y = 80;
	size_t Btn4_x = 340;
	size_t Btn4_y = 80;

 public:
	MyWindow(QWidget *parent = nullptr)
    		: QWidget(parent) {

		quitBtn = new QPushButton("Quit", this);
		quitBtn->setGeometry(430, 0, width, hight);

		Btn2 = new QPushButton("Btn2", this);
		Btn2->setGeometry(Btn2_x, Btn2_y, width, hight);

		Btn3 = new QPushButton("Btn3", this);
		Btn3->setGeometry(Btn3_x, Btn3_y, width, hight);

		Btn4 = new QPushButton("Btn4", this);
		Btn4->setGeometry(Btn4_x, Btn4_y, width, hight);
	
		connect(quitBtn, &QPushButton::clicked, qApp, &QApplication::quit);
	}
	
	void Timer(){
		QTimer *timer = new QTimer(this);
		connect(timer, &QTimer::timeout, this, &MyWindow::RandomMoveBtn2);
		connect(timer, &QTimer::timeout, this, &MyWindow::RandomMoveBtn3);
		connect(timer, &QTimer::timeout, this, &MyWindow::RandomMoveBtn4);
		timer->start(5000);
		
	}

	void RandomMoveBtn2() {
		size_t new_x = dist(rng) % 431;
		size_t new_y = dist(rng) % 616;
		
		QPropertyAnimation *animation = new QPropertyAnimation(Btn2, "geometry");
		animation->setDuration(1000);
		animation->setStartValue(QRect(Btn2_x, Btn2_y, width, hight));
		animation->setEndValue(QRect(new_x, new_y, width, hight));
		animation->start();
		
		Btn2_x = new_x;
		Btn2_y = new_y;
	}

	void RandomMoveBtn3() {
		size_t new_x = dist(rng) % 431;
		size_t new_y = dist(rng) % 616;
		
		QPropertyAnimation *animation = new QPropertyAnimation(Btn3, "geometry");
		animation->setDuration(1000);
		animation->setStartValue(QRect(Btn3_x, Btn3_y, width, hight));
		animation->setEndValue(QRect(new_x, new_y, width, hight));
		animation->start();
		
		Btn3_x = new_x;
		Btn3_y = new_y;
	}
	void RandomMoveBtn4() {
		size_t new_x = dist(rng) % 431;
		size_t new_y = dist(rng) % 616;
		
		QPropertyAnimation *animation = new QPropertyAnimation(Btn4, "geometry");
		animation->setDuration(1000);
		animation->setStartValue(QRect(Btn4_x, Btn4_y, width, hight));
		animation->setEndValue(QRect(new_x, new_y, width, hight));
		animation->start();
		
		Btn4_x = new_x;
		Btn4_y = new_y;
	}


};
