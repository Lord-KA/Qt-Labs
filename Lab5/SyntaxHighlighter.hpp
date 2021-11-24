#pragma once

#include <QSyntaxHighlighter>
#include <QString>
#include <QTextDocument>
#include <QRegularExpression>

class SyntaxHighlighter : QSyntaxHighlighter {
public:
    SyntaxHighlighter(QTextDocument *parent, QString fileType, QString theme, int stdVersion);
    
    void setupSyntaxHighlighter(QString fileType, QString theme);


private:
    QString fileType;
    int cSTD, cppSTD;
    QStringList 
};
