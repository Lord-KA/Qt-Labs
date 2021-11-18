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

#include <iostream>
#include <string>

#include "TextEditor.hpp"


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);

public slots:
    void openFile();
    void newFile();
    void saveFile();
    void saveAsFile();

private:
	QMenu *mainMenu;
    QAction *actionQuit, *actionOpen, *actionSave, *actionNew, *actionSaveAs;
    QToolBar *toolbar;
    CodeEditor *area;
    QString filename = "dump.txt";
};
