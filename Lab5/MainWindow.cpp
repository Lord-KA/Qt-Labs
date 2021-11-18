#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	mainMenu     = menuBar()->addMenu(tr("&File"));
	actionQuit   = mainMenu->addAction(tr("&Quit"),    QCoreApplication::instance(), SLOT(quit()));
	actionOpen   = mainMenu->addAction(tr("&Open"),    this, SLOT(openFile()));
	actionNew    = mainMenu->addAction(tr("&New"),     this, SLOT(newFile()));
	actionSave   = mainMenu->addAction(tr("&Save"),    this, SLOT(saveFile()));
	actionSaveAs = mainMenu->addAction(tr("&Save as"), this, SLOT(saveAsFile()));

    area = new CodeEditor();
    
    setCentralWidget(area);

	setFixedSize(1280, 720);
}

void MainWindow::saveFile() 
{
    #ifdef EXTRA_VERBOSE
        std::cerr << "Saving File to " << filename << "!\n";
    #endif
    QString text = area->toPlainText();
    QFile fout(filename);
    fout.open(QIODevice::Text);
    fout.write(text.toUtf8());
    fout.close();
}

void MainWindow::saveAsFile()
{
    #ifdef EXTRA_VERBOSE
        std::cerr << "Saving File As to " << filename << "!\n";
    #endif
}    //TODO

void MainWindow::openFile() 
{
    #ifdef EXTRA_VERBOSE
        std::cerr << "Opening File " << filename << "!\n";
    #endif
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    QStringList fileNames = {};
    if (dialog.exec())
        fileNames = dialog.selectedFiles();
    filename = fileNames[0];
    QFile file(filename);
    if (file.open(QIODevice::Text)) {
        area->setPlainText(file.readAll());
        file.close();
    }
    /*
    std::ifstream fin;
    fin.open(filename);
    std::string stdText;
    fin >> stdText;
    QString text;
    text.fromStdString(stdText);
    QTextDocument *document = new QTextDocument(area);
    document->setPlainText(text);
    area->setDocument(document);
    */
}      

void MainWindow::newFile()  
{
    #ifdef EXTRA_VERBOSE
        std::cerr << "Creating new File!\n";
    #endif
}      //TODO
