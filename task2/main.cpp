#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include "window.hpp"
#include "QSidePanel/PanelLeftSide.hpp"
#include "QSidePanel/PanelRightSide.hpp"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    MainWindow window;

    window.show();

    return app.exec();
}

