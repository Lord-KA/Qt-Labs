#pragma once

#include <QSyntaxHighlighter>
#include <QString>
#include <QTextDocument>
#include <QRegularExpression>

#include <iostream>

static const size_t MAX_FILETYPE_LEN = 10;
static const char SUPPORTED_THEMES[][MAX_FILETYPE_LEN] = {"monokai", "tomorrow", "tomorrowNight", "solarized"};
static const char SUPPORTED_FILETYPES[][MAX_FILETYPE_LEN] = {"c", "h", "cpp", "hpp", ""};
static const int  SUPPORTED_STDVERSIONS[] = {99, 11, 14, 20, 18};

class SyntaxHighlighter : QSyntaxHighlighter {
public:
    SyntaxHighlighter(QTextDocument *parent, QString fileType, QString theme, int stdVersion);
    
    void setupSyntaxHighlighter(QString fileType, QString theme);

    void setSyntax(QString fileType, int stdVersion);
    void setTheme (QString fileType, QString theme);

    void setLangRules();

    void setupKeywordPatterns(QString fileType, int stdVersion);

    void setColorValues(QString theme);

    void highlightBlock(const QString &text);


private:
    QString fileType, theme;
    int stdVersion;

    QStringList languages;
    QRegularExpression commentStartExpression, commentEndExpression;
    bool commonTextColorIsWhite = 1;

    QStringList difKeywordPatterns;

    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
 
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat keywordPatterns;
    QTextCharFormat keywordPatterns_C;
    QTextCharFormat keywordPatterns_Python;
    QTextCharFormat headerFileFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat formatStringFormat;
    QTextCharFormat operatorFormat;
    QTextCharFormat phpVarFormat;
    QTextCharFormat rubyVarFormat;
    QTextCharFormat tagFormat;
    QTextCharFormat valueFormat;
    QTextCharFormat attributeFormat;
    QTextCharFormat idFormat;

    QColor keywordColor;
    QColor keyword2Color;
    QColor functionsColor;
    QColor valueColor;
    QColor numColor;
    QColor operatorColor;
    QColor formatStringColor;
    QColor commentColor;
    QColor varColor;
    QColor tagColor;
    QColor htmlAttributesColor;
    QColor cssClassesIDsColor;
    QColor cssAttributesColor;

};
