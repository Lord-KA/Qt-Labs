#pragma once

#include <QSyntaxHighlighter>
#include <QString>
#include <QTextDocument>
#include <QRegularExpression>

class SyntaxHighlighter : QSyntaxHighlighter {
public:
    SyntaxHighlighter(QTextDocument *parent, QString fileType, QString theme, int stdVersion);
    
    void setupSyntaxHighlighter(QString fileType, QString theme);

    void setSyntax(QString fileType);
    void setTheme (QString fileType, QString theme);

    void setLangRules();

    void setupKeywordPatterns(QString fileType, int stdVersion);

    void setColorValues(QString theme);

    void highlightBlock(const QString &text);


private:
    QString fileType, theme;
    int cSTD, cppSTD;
    int stdVersion;

    QStringList languages;
    QRegularExpression commentStartExpression, commentEndExpression;
    bool knownLang;
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
