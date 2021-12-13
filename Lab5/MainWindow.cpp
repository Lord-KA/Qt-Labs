#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
    toolbarSetup();

    menubarSetup();

    area = new CodeEditor();

    statusbar = new QStatusBar(this);
    setStatusBar(statusbar);
     
    findDialog = new FindDialog(this);
    findDialog->setModal(false);
    findDialog->setPlainTextEdit(area);
     
    findReplaceDialog = new FindReplaceDialog(this);
    findReplaceDialog->setModal(false);
    findReplaceDialog->setPlainTextEdit(area);
    
    setCentralWidget(area);
    setMinimumSize(1280, 720);

    setDefaultFilename();

    syntaxHighlighter = new SyntaxHighlighter(area->document(), HighlightingSetup::none, "tomorrow");

    unsavedChanges = false;
    titleUpdate();
    statusbarUpdate();
    connect(area, &QPlainTextEdit::textChanged, this, [this](){statusbarUpdate(); unsavedChanges = true; titleUpdate();});
}

void MainWindow::toolbarSetup()
{
    toolbar = addToolBar("ToolBar");
    toolbar->addAction("New",   this, SLOT(newFile()));
    toolbar->addAction("Open",  this, SLOT(openFile()));
    toolbar->addAction("Undo",  this, [&, this](){area->undo();});
    toolbar->addAction("Redo",  this, [&, this](){area->redo();});
    toolbar->addAction("Copy",  this, [&, this](){area->copy();});
    toolbar->addAction("Paste", this, [&, this](){area->paste();});
    toolbar->addAction("Find",               this, [&, this](){findDialog->show();});
    toolbar->addAction("Find and Replace",   this, [&, this](){findReplaceDialog->show();});
}

void MainWindow::menubarSetup()
{
    /* Main Menu setup */
	mainMenu = menuBar()->addMenu("File");
	actionQuit   = mainMenu->addAction("Quit",    this, SLOT(quit()));
	actionOpen   = mainMenu->addAction("Open",    this, SLOT(openFile()));
	actionNew    = mainMenu->addAction("New",     this, SLOT(newFile()));
	actionSave   = mainMenu->addAction("Save",    this, SLOT(saveFile()));
	actionSaveAs = mainMenu->addAction("Save as", this, SLOT(saveAsFile()));

    /* Edit Menu setup */
    editMenu = menuBar()->addMenu("Edit");
    actionUndo  = editMenu->addAction("Undo",  this, [&, this](){area->undo();});
    actionRedo  = editMenu->addAction("Redo",  this, [&, this](){area->redo();});
    actionCopy  = editMenu->addAction("Copy",  this, [&, this](){area->copy();});
    actionPaste = editMenu->addAction("Paste", this, [&, this](){area->paste();});
    actionFind        = editMenu->addAction("Find",               this, [&, this](){findDialog->show();});
    actionFindReplace = editMenu->addAction("Find and Replace",   this, [&, this](){findReplaceDialog->show();});
    actionSelectAll   = editMenu->addAction("Select All",         this, [&, this](){area->selectAll();});
    
    /* Format Menu setup */
    formatMenu = menuBar()->addMenu("Format");
    actionChangeFont = formatMenu->addAction("Change font", this, SLOT(changeFont()));
    QWidgetAction *actionWrapperBox = new QWidgetAction(formatMenu);
    QCheckBox *wrapperBox = new QCheckBox(formatMenu);
    wrapperBox->setText("Enable wrapping");
    actionWrapperBox->setDefaultWidget(wrapperBox);
    connect(wrapperBox, &QCheckBox::stateChanged, this, [this, wrapperBox]()
            {
                area->setLineWrapMode((QPlainTextEdit::LineWrapMode)!wrapperBox->checkState());
            });
    formatMenu->addAction(actionWrapperBox);

    /* View Menu setup */
    viewMenu = menuBar()->addMenu("View");
    actionChangeBackgroundColor = viewMenu->addAction("Change background color", this, SLOT(changeBackgroundColor()));
    QWidgetAction *actionHideLineNumBox = new QWidgetAction(formatMenu);
    QCheckBox *hideLineNumBox = new QCheckBox(formatMenu);
    hideLineNumBox->setText("Hide line numbers");
    actionHideLineNumBox->setDefaultWidget(hideLineNumBox);
    connect(hideLineNumBox, &QCheckBox::stateChanged, this, [this, hideLineNumBox]()
            {
                if (hideLineNumBox->checkState())
                    area->hideLineNumberArea();
                else
                    area->showLineNumberArea();
                area->updateLineNumberAreaWidth();
            });
    viewMenu->addAction(actionHideLineNumBox);
    viewMenu->addAction(toolbar->toggleViewAction());
    /* Highlighting setup */            //TODO
    QActionGroup *highlightingGroup = new QActionGroup(viewMenu);
    for (int i = 0; i < static_cast<int>(HighlightingSetup::CNT); ++i) {
        QAction *action = new QAction(HighlightingSetups[i], highlightingGroup);
        connect(action, &QAction::triggered, this, [this, i](){this->syntaxHighlighter->setSyntax(static_cast<HighlightingSetup>(i));});
        action = highlightingGroup->addAction(viewMenu->addAction(HighlightingSetups[i], this, [this, i](){syntaxHighlighter->setSyntax(static_cast<HighlightingSetup>(i));}));
        highlightingGroup->addAction(action);
        if (i == 0)
            action->setChecked(true);
    }
    highlightingGroup->setVisible(true);
}

void MainWindow::statusbarUpdate()
{
    QTextCursor cursor = area->textCursor();
    size_t line = cursor.blockNumber() + 1;
    size_t pos  = cursor.positionInBlock() + 1;
    cursor.movePosition(QTextCursor::End);
    size_t lineCount  = cursor.blockNumber() + 1;
    size_t charsCount = cursor.position() + 1;
    size_t wordCount  = area->toPlainText().split(QRegExp("(\\s|\\n|\\r)+"),
                                                  QString::SkipEmptyParts).count();


    #ifdef EXTRA_VERBOSE
        fprintf(stderr, "line = %lu, pos = %lu\n", line, pos);
    #endif

    std::tm *now = std::localtime(&lastSaveTimestamp);
    QString message =          QString::number(line) +          ":" + QString::number(pos) + 
                     " | "   + QString::number(now->tm_hour) +  ":" + QString::number(now->tm_min) +   ":" + QString::number(now->tm_sec) +
                     " | l:" + QString::number(lineCount) + " | w:" + QString::number(wordCount) + " | c:" + QString::number(charsCount) + 
                     " |Kb:" + QString::number(charsCount / 1024);
    statusbar->showMessage(message);
}

void MainWindow::titleUpdate()
{
    QString title = filename;
    if (unsavedChanges) 
        title += "*";
    setWindowTitle(title);
}

void MainWindow::changeBackgroundColor()
{
    QColorDialog *dialog = new QColorDialog(QColor("white"), this);
    dialog->exec();

    QPalette p = area->palette();
    p.setColor(QPalette::Active, QPalette::Base, dialog->selectedColor());
    area->setPalette(p);
    area->setBackgroundVisible(false);
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
    } else if (reply == QMessageBox::Close) {
        exitApp();
    }
}

void MainWindow::setDefaultFilename()
{
    int num = 1;
    std::ifstream file;
    do {
        file.close();
        filename = DEFAULT_FILENAME + QString::number(num) + "." + DEFAULT_FILETYPE;
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
    lastSaveTimestamp = std::time(0);
    statusbarUpdate();
    unsavedChanges = false;
    titleUpdate();

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
    lastSaveTimestamp = std::time(0);
    statusbarUpdate();
    unsavedChanges = false;
    titleUpdate();

    #ifdef EXTRA_VERBOSE
        std::cerr << "Saving File As to " << filename.toStdString() << "!\n";
    #endif
 
    saveFile();
}    

void MainWindow::openFile() 
{
    filename = QFileDialog::getOpenFileName(this, "Open", ".");
    lastSaveTimestamp = std::time(0);
    statusbarUpdate();
    unsavedChanges = false;
    titleUpdate();

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
    lastSaveTimestamp = std::time(0);
    statusbarUpdate();
    unsavedChanges = false;
    titleUpdate();

    #ifdef EXTRA_VERBOSE
        std::cerr << "Creating new File " << filename.toStdString() << " !\n";
    #endif
}  
