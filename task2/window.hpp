#include <QPropertyAnimation>
#include <QTimer>
#include <QWidget>
#include <QPushButton>
#include <QApplication>

class MainWindow : public QWidget {
 private:
	static constexpr size_t hight = 25;
	static constexpr size_t width = 50;

 public:
	MainWindow(QWidget *parent = nullptr)
    		: QWidget(parent) {

        
        }   

};
