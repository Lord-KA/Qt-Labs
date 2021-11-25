#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	mainMenu = menuBar()->addMenu("File");
	actionQuit   = mainMenu->addAction("Quit",    this, SLOT(quit()));
	actionOpen   = mainMenu->addAction("Open",    this, SLOT(openFile()));
	actionNew    = mainMenu->addAction("New",     this, SLOT(newFile()));
	actionSave   = mainMenu->addAction("Save",    this, SLOT(saveFile()));
	actionSaveAs = mainMenu->addAction("Save as", this, SLOT(saveAsFile()));

    editMenu = menuBar()->addMenu("Edit");
    actionUndo  = editMenu->addAction("Undo",  this, [&, this](){this->area->undo();});
    actionRedo  = editMenu->addAction("Redo",  this, [&, this](){this->area->redo();});
    actionCopy  = editMenu->addAction("Copy",  this, [&, this](){this->area->copy();});
    actionPaste = editMenu->addAction("Paste", this, [&, this](){this->area->paste();});
    actionFind        = editMenu->addAction("Find",               this, [&, this](){this->findDialog->show();});
    actionFindReplace = editMenu->addAction("Find and Replace",   this, [&, this](){this->findReplaceDialog->show();});
    actionSelectAll   = editMenu->addAction("Select All",         this, [&, this](){this->area->selectAll();});
    
    formatMenu = menuBar()->addMenu("Format");
    actionChangeFont = formatMenu->addAction("Change font", this, SLOT(changeFont()));
    QWidgetAction *actionCheckBox = new QWidgetAction(formatMenu);
    QCheckBox *wrapperBox = new QCheckBox(formatMenu);
    wrapperBox->setText("Enable wrapping");
    actionCheckBox->setDefaultWidget(wrapperBox);
    connect(wrapperBox, &QCheckBox::stateChanged, this, [this, wrapperBox]()
            {
                this->area->setLineWrapMode((QPlainTextEdit::LineWrapMode)!wrapperBox->checkState());
            });
    formatMenu->addAction(actionCheckBox);

    viewMenu = menuBar()->addMenu("View");  //TODO
    
    area = new CodeEditor();

    findDialog = new FindDialog(this);
    findDialog->setModal(false);
    findDialog->setPlainTextEdit(area);
     
    findReplaceDialog = new FindReplaceDialog(this);
    findReplaceDialog->setModal(false);
    findReplaceDialog->setPlainTextEdit(area);
    
    setCentralWidget(area);

	setFixedSize(1280, 720);

    setDefaultFilename();

    syntaxHighlighter = new SyntaxHighlighter(area->document(), "hpp", "tomorrow", 14);
}

void MainWindow::changeFont()
{
    #ifdef EXTRA_VERBOSE
        std::cerr << "Changing font to ";
    #endif
    QFont currentFont = area->document()->defaultFont();
    int fontSize = currentFont.pointSize();

    QDialog *askFont = new QDialog(this);
    askFont->setLayout(new QHBoxLayout());

    QComboBox *fontBox = new QComboBox(askFont);
    fontBox->addItem("Helvetica");
    fontBox->addItem("Times");
    fontBox->addItem("Courier");
    fontBox->addItem("OldEnglish");
    fontBox->addItem("System");
    fontBox->addItem("Any");

    fontBox->setCurrentIndex((int)currentFont.styleHint());
    QPushButton *buttonOk     = new QPushButton("Ok");
    QPushButton *buttonCancel = new QPushButton("Cancel");

    QSpinBox *sizeBox = new QSpinBox(askFont);
    sizeBox->setMinimum(1);
    sizeBox->setValue(currentFont.pointSize());

    askFont->layout()->addWidget(fontBox);
    askFont->layout()->addWidget(sizeBox);
    askFont->layout()->addWidget(buttonOk);
    askFont->layout()->addWidget(buttonCancel);
    connect(buttonOk, &QPushButton::clicked, this, [&currentFont, &fontBox, &sizeBox, askFont]()
            {
                currentFont.setStyleHint((QFont::StyleHint)fontBox->currentIndex());
                askFont->accept();
                currentFont.setPointSize(sizeBox->value());
            });
    connect(buttonCancel, &QPushButton::clicked, this, [askFont]()
            {
                askFont->reject();
            });
 
    askFont->exec();

    area->document()->setDefaultFont(currentFont);

    #ifdef EXTRA_VERBOSE
        std::cerr << currentFont.styleHint() << "!\n";
    #endif
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
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Exit", "Close without saving?",
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
    filename = QFileDialog::getSaveFileName(this, "Save file as", ".");

    #ifdef EXTRA_VERBOSE
        std::cerr << "Saving File As to " << filename.toStdString() << "!\n";
    #endif
 
    saveFile();
}    

void MainWindow::openFile() 
{
    filename = QFileDialog::getOpenFileName(this, "Open", ".");

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
