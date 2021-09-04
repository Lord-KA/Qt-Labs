#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include "window.hpp"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    MyWindow window;

    window.resize(480, 640);
    window.setWindowTitle("My window");
    window.show();

    // window.RandomMoveBtn2();
    // window.RandomMoveBtn3();
    window.Timer();

    return app.exec();
}

