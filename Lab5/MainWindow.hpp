#pragma once

#include <QVector>
#include <QPushButton>
#include <QMenuBar>
#include <QCoreApplication>
#include <QContextMenuEvent>
#include <QToolBar>
#include <QPainter>
#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QComboBox>
#include <QFont>
#include <QSpinBox>
#include <QCheckBox>
#include <QWidgetAction>

#include <iostream>
#include <string>
#include <fstream>

#include "TextEditor.hpp"

#include "finddialog.h"
#include "findreplacedialog.h"

static const char DEFAULT_FILENAME[] = "untitled";

static const QFont::StyleHint DEFAULT_FONT = QFont::Times;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);

private:
    void setDefaultFilename();
    void exitApp();

public slots:
    void quit();
    void openFile();
    void newFile();
    void saveFile();
    void saveAsFile();

    void changeFont();

private:
	QMenu *mainMenu, *editMenu, *formatMenu, *viewMenu;
    QAction *actionQuit, *actionOpen, *actionSave, *actionNew, *actionSaveAs;
    QAction *actionUndo, *actionRedo, *actionCopy, *actionPaste, *actionFind, *actionFindReplace, *actionSelectAll;
    QAction *actionChangeFont;

    QToolBar   *toolbar;
    CodeEditor *area;
    FindDialog        *findDialog;
    FindReplaceDialog *findReplaceDialog;
    QString filename;
};

