#include "sidebars.hpp"

#include <QVBoxLayout>

// ---------------------------

#include "QSidePanel/PanelLeftSide.hpp"
#include "QSidePanel/PanelRightSide.hpp"

// ---------------------------

#include <QTextEdit>
#include <QLabel>

// ---------------------------

#include "QSidePanel/math.hpp"

#include <QScroller>

#include "QSidePanel/side_panel_helpers.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    btn_open  = new QPushButton("Open",  this);
    btn_close = new QPushButton("Close", this);

    btn_open ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    btn_close->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    {
        QHBoxLayout* lay = new QHBoxLayout;
        lay->addWidget(btn_open);
        lay->addWidget(btn_close);

        QWidget* proxy = new QWidget;
        proxy->setLayout(lay);
        this->setCentralWidget(proxy);
    }

    // -------------------------------------------------------------------------

    // Left panel with simple label in it
    {
        auto* panel_left = new PanelLeftSide(this);
        panel_left->setOpenEasingCurve (QEasingCurve::Type::OutElastic);
        panel_left->setCloseEasingCurve(QEasingCurve::Type::InElastic);
        panel_left->init();


        QLabel* label = new QLabel("Left");
        label->setAlignment(Qt::AlignCenter);

        panel_left->setWidgetResizable(true);
        panel_left->setWidget(label);
    }

    // Right panel with buttons list in it
    auto* panel_right = new PanelRightSide(this);
    {
        panel_right->setOpenEasingCurve (QEasingCurve::Type::OutExpo);
        panel_right->setCloseEasingCurve(QEasingCurve::Type::InExpo);
        panel_right->setPanelSize(200);
        panel_right->init();


        QVBoxLayout* lay = new QVBoxLayout;
        {
            QLabel* label = new QLabel("Right", this);
            label->setAlignment(Qt::AlignCenter);
            lay->addWidget(label);

            for(int i = 0; i < 20; ++i)
            {
                QPushButton* btn = new QPushButton("Button " + QString::number(i), this);
                btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                btn->setMinimumHeight(60);

                lay->addWidget(btn);
            }
        }

        QWidget* proxy = new QWidget(this);
        proxy->setLayout(lay);

        panel_right->setWidgetResizable(true);
        panel_right->setWidget(proxy);

        // Extra behavior (for example) - scrolling by gestures. Notice, that
        // QScroller binded to `panel_right->viewport()`, not to `panel_right`
        {
            QScroller::grabGesture(panel_right->viewport(), QScroller::ScrollerGestureType::LeftMouseButtonGesture);

            auto* scroller = QScroller::scroller(panel_right->viewport());
            {
                // Improve buttons pressing. But still not good
                QScrollerProperties props = scroller->scrollerProperties();
                props.setScrollMetric(QScrollerProperties::ScrollMetric::MousePressEventDelay, 0);
                scroller->setScrollerProperties(props);
            }
        }
    }

    /*
    // Nested panels, which parented to button
    {
        auto* panel_top_in_widget = new PanelTopSide(btn_close);
        {
            panel_top_in_widget->getOpenedRect = [panel_top_in_widget](const QRect& parent_rect) -> QRect {
                return q_sp::rect_opened_full_top(panel_top_in_widget->getHandlerSize().height(), parent_rect);
            };
            panel_top_in_widget->init();

            QLabel* label = new QLabel("Over Button", this);
            label->setAlignment(Qt::AlignCenter);

            panel_top_in_widget->setWidgetResizable(true);
            panel_top_in_widget->setWidget(label);

        }

        auto panel_nested_1 = new PanelLeftSide(panel_top_in_widget);
        {
            panel_nested_1->getOpenedRect = [panel_nested_1](const QRect& parent_rect) -> QRect {
                return q_sp::rect_opened_half_left(panel_nested_1->getHandlerSize().width(), parent_rect);
            };
            panel_nested_1->init();

            QLabel* label = new QLabel("Left nested", this);
            label->setAlignment(Qt::AlignCenter);

            panel_nested_1->setWidgetResizable(true);
            panel_nested_1->setWidget(label);
        }

        auto panel_nested_2 = new PanelBottomSide(panel_nested_1);
        {
            panel_nested_2->getOpenedRect = [panel_nested_2](const QRect& parent_rect) -> QRect {
                return q_sp::rect_opened_half_bottom(panel_nested_2->getHandlerSize().height(), parent_rect);
            };
            panel_nested_2->init();

            QLabel* label = new QLabel("Bottom nested", this);
            label->setAlignment(Qt::AlignCenter);

            panel_nested_2->setWidgetResizable(true);
            panel_nested_2->setWidget(label);
        }

        auto panel_nested_3 = new PanelLeftSide(panel_nested_2);
        {
            panel_nested_3->getOpenedRect = [panel_nested_3](const QRect& parent_rect) -> QRect {
                return q_sp::rect_opened_half_left(panel_nested_3->getHandlerSize().width(), parent_rect);
            };
            panel_nested_3->init();

            QLabel* label = new QLabel("Left nested", this);
            label->setAlignment(Qt::AlignCenter);

            panel_nested_3->setWidgetResizable(true);
            panel_nested_3->setWidget(label);
        }
    }
*/
    // -------------------------------------------------------------------------

    connect(btn_open, &QAbstractButton::clicked, panel_right, [panel_right] {
        panel_right->openPanel();
    });

    connect(btn_close, &QAbstractButton::clicked, panel_right, [panel_right] {
        panel_right->closePanel();
    });

    connect(panel_right, &SidePanel::stateChanged, this, [this](SidePanelState state)
    {
        this->setWindowTitle( QString::fromUtf8( to_str(state) ) );
    });

    // -------------------------------------------------------------------------

    this->resize(800, 600);
}

MainWindow::~MainWindow()
{

}

