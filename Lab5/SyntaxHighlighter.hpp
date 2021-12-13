#pragma once

#include <QSyntaxHighlighter>
#include <QString>
#include <QTextDocument>
#include <QRegularExpression>

#include <iostream>

static const size_t MAX_FILETYPE_LEN = 100;
static const char SUPPORTED_THEMES[][MAX_FILETYPE_LEN] = {"monokai", "tomorrow", "tomorrowNight", "solarized"};

enum class HighlightingSetup {
    none = 0,
    cpp99,
    cpp14,
    cpp20,
    c11,
    c18,
    CNT,
};

static const char HighlightingSetups[][MAX_FILETYPE_LEN] = {
        "none",
        "C++99",
        "C++14",
        "C++20",
        "C11",
        "C18",
    };

class SyntaxHighlighter : QSyntaxHighlighter {
public:
    SyntaxHighlighter(QTextDocument *parent, HighlightingSetup setup, QString theme);
    
    void setupSyntaxHighlighter(HighlightingSetup setup, QString theme);

    void setSyntax(HighlightingSetup setup);
    void setTheme (HighlightingSetup setup, QString theme);

    void setLangRules();

    void setupKeywordPatterns();

    void setColorValues(QString theme);

    void highlightBlock(const QString &text);


private:
    HighlightingSetup setup;
    QString theme;

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

    friend class MainWindow;
};
