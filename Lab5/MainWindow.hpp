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

#include <iostream>
#include <string>
#include <fstream>

#include "TextEditor.hpp"

static const char DEFAULT_FILENAME[] = "untitled";

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	QMenu *mainMenu, *editMenu;
    QAction *actionQuit, *actionOpen, *actionSave, *actionNew, *actionSaveAs;
    QAction *actionUndo, *actionRedo, *actionCopy, *actionPaste;
    QToolBar *toolbar;
    CodeEditor *area;
    QString filename = "";

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
};

