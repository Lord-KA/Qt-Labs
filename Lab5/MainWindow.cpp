#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	mainMenu     = menuBar()->addMenu(tr("&File"));
	actionQuit   = mainMenu->addAction(tr("&Quit"),    this, SLOT(quit()));
	actionOpen   = mainMenu->addAction(tr("&Open"),    this, SLOT(openFile()));
	actionNew    = mainMenu->addAction(tr("&New"),     this, SLOT(newFile()));
	actionSave   = mainMenu->addAction(tr("&Save"),    this, SLOT(saveFile()));
	actionSaveAs = mainMenu->addAction(tr("&Save as"), this, SLOT(saveAsFile()));

    editMenu     = menuBar()->addMenu("&Edit");
    actionUndo   = editMenu->addAction("Undo",   this, [&, this](){this->area->undo();});
    actionRedo   = editMenu->addAction("Redo",   this, [&, this](){this->area->redo();});
    actionCopy   = editMenu->addAction("Copy",   this, [&, this](){this->area->copy();});
    actionPaste  = editMenu->addAction("Paste",  this, [&, this](){this->area->paste();});

    area = new CodeEditor();
    
    setCentralWidget(area);

	setFixedSize(1280, 720);

    setDefaultFilename();
}

void MainWindow::exitApp()
{
    #ifdef EXTRA_VERBOSE
        std::cerr << "Exiting the application!\n";
    #endif
    
    exit(0);
}

void MainWindow::quit()
{
    #ifdef EXTRA_VERBOSE
        std::cerr << "Quit Dialog!\n";
    #endif
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Exit", "Exit without saving?",
                                                                QMessageBox::Save   | 
                                                                QMessageBox::Cancel |
                                                                QMessageBox::Close    );
    if (reply == QMessageBox::Save) {
        saveFile();
        exitApp();
    } else if (reply == QMessageBox::Close)
        exitApp();
}

void MainWindow::setDefaultFilename()
{
    int num = 1;
    std::ifstream file;
    do {
        file.close();
        filename = DEFAULT_FILENAME + QString::number(num);
        file.open(filename.toStdString(), std::ifstream::in);
        ++num;
    } while (file.good());
}

void MainWindow::saveFile() 
{
    QString text = area->toPlainText();

    if (filename.isEmpty()) {
        setDefaultFilename();
    }

    #ifdef EXTRA_VERBOSE
        std::cerr << "Saving File to " << filename.toStdString() << "!\n";
    #endif

    QFile fout(filename);
    fout.open(QIODevice::ReadWrite | QIODevice::Text);
    fout.write(text.toUtf8());
    fout.write("\n");
    fout.close();
}

void MainWindow::saveAsFile()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    QStringList fileNames = {};
    if (dialog.exec())
        fileNames = dialog.selectedFiles();
    filename = fileNames[0];

    #ifdef EXTRA_VERBOSE
        std::cerr << "Saving File As to " << filename.toStdString() << "!\n";
    #endif
 
    saveFile();
}    

void MainWindow::openFile() 
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    QStringList fileNames = {};
    if (dialog.exec())
        fileNames = dialog.selectedFiles();
    filename = fileNames[0];

    #ifdef EXTRA_VERBOSE
        std::cerr << "Opening File " << filename.toStdString() << "!\n";
    #endif
 
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        area->setPlainText(file.readAll());
        file.close();
    }
}      

void MainWindow::newFile()  
{
   
    setDefaultFilename();

    #ifdef EXTRA_VERBOSE
        std::cerr << "Creating new File " << filename.toStdString() << " !\n";
    #endif
}  
