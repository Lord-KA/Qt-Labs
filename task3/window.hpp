#include <QPropertyAnimation>
#include <QTimer>
#include <QWidget>
#include <QPushButton>
#include <QApplication>
#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QToolBar>
#include <QMenuBar>
#include <QMessageBox>


static const size_t start_btnV_x = 0;
static const size_t start_btnV_y = 0;

static const size_t start_btnH_x = 2;
static const size_t start_btnH_y = 2;

static const size_t start_label_x = 1;
static const size_t start_label_y = 1;

static const size_t grid_x_size = 3;
static const size_t grid_y_size = 3;


class MyWindow : public QMainWindow {
private:
     size_t btnV_x = start_btnV_x;
     size_t btnV_y = start_btnV_y;
     
     size_t btnH_x = start_btnH_x;
     size_t btnH_y = start_btnH_y;

     size_t label_x = start_label_x;
     size_t label_y = start_label_x;


     QGridLayout *layout;
     QToolBar *toolbar;
     QMenuBar *menubar;
 
public:
	MyWindow(QMainWindow *parent = nullptr)
    		: QMainWindow(parent) {
    
        setCentralWidget (new QWidget (this));
        layout = new QGridLayout();
        centralWidget()->setLayout(layout);

        QPushButton *btnV = new QPushButton("Vertical");
        QLabel *label = new QLabel("MyPaint");
        QPushButton *btnH = new QPushButton("Horisontal");

        layout->addWidget(btnV, btnV_x, btnV_y);
        layout->addWidget(label, label_x, label_y);
        layout->addWidget(btnH, btnH_x, btnH_y);


        toolbar = new QToolBar();
        addToolBar(toolbar);

        QPushButton *btnMoveR = new QPushButton("->");
        QPushButton *btnMoveL = new QPushButton("<-");
        QPushButton *btnRestorePos = new QPushButton("X");
        toolbar->addWidget(btnMoveL);
        toolbar->addWidget(btnRestorePos);
        toolbar->addWidget(btnMoveR);

        
        menubar = menuBar();
        
        QMenu *Menubar_File = menubar->addMenu("File");
        QAction* Menubar_Action_Quit = Menubar_File->addAction("Quit");

        QMenu *Menubar_Options = menubar->addMenu("Options");
        QAction* Menubar_Action_MoveR   = Menubar_Options->addAction("Move right");
        QAction* Menubar_Action_MoveL   = Menubar_Options->addAction("Move left");
        QAction* Menubar_Action_MoveH   = Menubar_Options->addAction("Move horisontal");
        QAction* Menubar_Action_MoveV   = Menubar_Options->addAction("Move vertical");
        QAction* Menubar_Action_Restore = Menubar_Options->addAction("Restore");

        QMenu *Menubar_About = menubar->addMenu("About");
        QAction* Menubar_Action_About   = Menubar_About->addAction("About app");
        QAction* Menubar_Action_AboutQt = Menubar_About->addAction("About Qt");

   	    connect(btnH, &QPushButton::clicked, this, &MyWindow::Horisontal);
   	    connect(btnV, &QPushButton::clicked, this, &MyWindow::Vertical);

   	    connect(btnMoveR, &QPushButton::clicked, this, &MyWindow::MoveR);
   	    connect(btnMoveL, &QPushButton::clicked, this, &MyWindow::MoveL);
   	    connect(btnRestorePos, &QPushButton::clicked, this, &MyWindow::RestorePos);

        connect(Menubar_Action_Quit,  &QAction::triggered, this, &QApplication::quit);

   	    connect(Menubar_Action_MoveR,   &QAction::triggered, this, &MyWindow::MoveR);
   	    connect(Menubar_Action_MoveL,   &QAction::triggered, this, &MyWindow::MoveL);
   	    connect(Menubar_Action_MoveH,   &QAction::triggered, this, &MyWindow::Horisontal);
   	    connect(Menubar_Action_MoveV,   &QAction::triggered, this, &MyWindow::Vertical);
   	    connect(Menubar_Action_Restore, &QAction::triggered, this, &MyWindow::RestorePos);

   	    connect(Menubar_Action_About,   &QAction::triggered, this, &MyWindow::ShowAbout);
   	    connect(Menubar_Action_AboutQt,   &QAction::triggered, this, &MyWindow::ShowAboutQt);
  
	}


    void Vertical() {
        setWidgetPos(layout, btnV_x, btnV_y, grid_x_size - btnV_x - 1, btnV_y);
        setWidgetPos(layout, btnH_x, btnH_y, grid_x_size - btnH_x - 1, btnH_y);
    }

    void Horisontal() {
        setWidgetPos(layout, btnV_x, btnV_y, btnV_x, grid_y_size - btnV_y - 1);
        setWidgetPos(layout, btnH_x, btnH_y, btnH_x, grid_y_size - btnH_y - 1);
    }

    void MoveR() {    
        setWidgetPos(layout, btnV_x, btnV_y, btnV_x, (btnV_y + 1) % grid_y_size);
        setWidgetPos(layout, btnH_x, btnH_y, btnH_x, (btnH_y + 1) % grid_y_size);
        setWidgetPos(layout, label_x, label_y, label_x, (label_y + 1) % grid_y_size);
    }

    void MoveL() {              //TODO ask if is is better than `long long int btn*_y`
        size_t new_bntV_y = (btnV_y - 1) % grid_y_size;
        size_t new_bntH_y = (btnH_y - 1) % grid_y_size;
        size_t new_lable_y = (label_y - 1) % grid_y_size;
        if (btnV_y == 0)
            new_bntV_y = grid_y_size - 1;
        if (btnH_y == 0)
            new_bntH_y = grid_y_size - 1;
        if (label_y == 0)
            new_lable_y = grid_y_size - 1;

        setWidgetPos(layout, btnV_x, btnV_y, btnV_x, new_bntV_y);
        setWidgetPos(layout, btnH_x, btnH_y, btnH_x, new_bntH_y);
        setWidgetPos(layout, label_x, label_y, label_x, new_lable_y);
    }

    void RestorePos() {
       setWidgetPos(layout, btnV_x, btnV_y, start_btnV_x, start_btnV_y);
       setWidgetPos(layout, btnH_x, btnH_y, start_btnH_x, start_btnH_y);
       setWidgetPos(layout, label_x, label_y, start_label_x, start_label_y);
    }

    void ShowAbout() {
        QMessageBox::about(this, "Boring App", "Made by Lord-KA (and he is ashamed of it)");
    }
    
    void ShowAboutQt() {
        QMessageBox::about(this, "Qt", "Qt 5.15 made by 'The Qt Company'");
    }

    void setWidgetPos(QGridLayout *layout, size_t &old_x, size_t &old_y, size_t new_x, size_t new_y) {
        auto widget = layout->itemAtPosition(old_x, old_y)->widget();
        layout->removeWidget(widget);
        layout->addWidget(widget, new_x, new_y);
        old_x = new_x;
        old_y = new_y;
    }
};
