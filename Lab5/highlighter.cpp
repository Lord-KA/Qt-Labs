#include "SyntaxHighlighter.hpp"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent, QString fileType, QString theme, int std)
    : QSyntaxHighlighter(parent), fileType(fileType)
{
    if (std == 99 || std == 14 || std == 20)
    {
        fileType = "cpp";
        this->cppSTD = std;
    }
    if (std == 11 || std == 18)
    {
        fileType = "c";
        this->cSTD = std;
    }

    commentStartExpression = QRegularExpression("");
    commentEndExpression = QRegularExpression("");
    setupSyntaxHighlighter(fileType, theme);
}
void SyntaxHighlighter::setupSyntaxHighlighter(QString fileType, QString theme)
{
    compiledLanguages = QString("c h cpp hpp java").split(" ");
    scriptingLanguages = QString("bat hs js lua php py rb sh vb").split(" ");
    markupLanguages = QString("htm html json xml").split(" ");

    setColorValues(theme);

    if (compiledLanguages.contains(fileType)){
        setSyntax(fileType);
    }
    else if (scriptingLanguages.contains(fileType))
    {
        if (fileType == "py")
        {
            knowedLang = 1;
            setupKeywordPatterns(fileType);
            setScriptingLanguageRules();
        }
        else setScriptingLanguageRules();
    }
    else if (markupLanguages.contains(fileType))
    {
        setMarkupLanguageRules();
    }
    else if (fileType == "asm")
    {
        knowedLang = 1;
        setAsmRules();
    }
    else if (fileType == "css")
    {
        knowedLang = 1;
        setCssRules();
    }
    else if (fileType == "sql")
    {
        knowedLang = 1;
        setSqlRules();
    }
    else
    {
        knowedLang = 0;
        // TODO set non highlight Rules!
    }
}
void SyntaxHighlighter::setSyntax(QString fileType)
{
    if (cSTD !=0 )
    {
        knowedLang = 1;
        setupKeywordPatterns(fileType, cSTD);
        setCompiledLanguageRules();
    }
    else if (fileType == "cpp" || fileType == "hpp" || fileType == "h")
    {
        knowedLang = 1;
        setupKeywordPatterns(fileType, cppSTD);
        setCompiledLanguageRules();
    }
    else if (fileType == "c")
    {
        knowedLang = 1;
        cSTD = 11;
        setupKeywordPatterns(fileType, cSTD);
        setCompiledLanguageRules();
    }
    else{
        knowedLang = 0;
    }
}

void SyntaxHighlighter::setTheme(QString filetype, QString theme){
    setupSyntaxHighlighter(filetype, theme);
    rehighlight();
}

void SyntaxHighlighter::setCompiledLanguageRules(){
    HighlightingRule rule;

    //Functions
    functionFormat.setForeground(functionsColor);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    keywordFormat.setForeground(keywordColor);
    keywordFormat.setFontWeight(QFont::Bold);
    if(!knowedLang)
    {
        QStringList keywordPatterns;
        keywordPatterns << "\\bchar\\b" << "\\bint\\b" << "\\bfloat\\b" << "\\bdouble\\b"
                        << "\\bstruct\\b" << "\\benum\\b" << "\\bvoid\\b" << "\\bshort\\b"
                        << "\\blong\\b" << "\\btrue\\b" << "\\bfalse\\b" << "\\bboolean\\b"
                        << "\\bnull\\b" << "\\bthis\\b" << "\\bfinal\\b"
                        << "\\band\\b" << "\\bor\\b" << "\\bxor\\b"
                        << "\\bconst\\b" << "\\bstatic\\b" << "\\bsigned\\b" << "\\bunsigned\\b"
                        << "\\bimport\\b" << "\\bnamespace\\b" << "\\breturn\\b" << "\\busing\\b"
                        << "\\bfor\\b" << "\\bwhile\\b" << "\\bif\\b" << "\\belse\\b"
                        << "\\bcase\\b" << "\\bswitch\\b" << "\\bdo\\b" << "\\bunion\\b"
                        << "\\bnew\\b" << "\\bclass\\b" << "\\bprivate\\b" << "\\bprotected\\b"
                        << "\\bpublic\\b" << "\\bvirtual\\b" << "\\bslots\\b" << "\\bvolatile\\b"
                        << "\\babstract\\b" << "\\bextends\\b" << "\\bimplements\\b" << "\\bsuper\\b"
                        << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                        << "\\btry\\b" << "\\bcatch\\b" << "\\bthrow\\b" << "\\bbreak\\b";
        for (const QString &pattern : keywordPatterns) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }
    }
    else
    {
        for (const QString &pattern : difKeywordPatterns) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }
    }

    numberFormat.setForeground(numColor);
    rule.pattern = QRegularExpression("\\b[0-9\\.]+\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(valueColor);
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(valueColor);
    rule.pattern = QRegularExpression("'.*'");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    formatStringFormat.setForeground(formatStringColor);
    rule.pattern = QRegularExpression("%[sdifFuoxXeEgGaAcpn]+\\b");
    rule.format = formatStringFormat;
    highlightingRules.append(rule);

    headerFileFormat.setForeground(QColor("#ff6d6d"));
    rule.pattern = QRegularExpression("#include.?[<\"].*[>\"]");
    rule.format = headerFileFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setFontItalic(true);
    singleLineCommentFormat.setForeground(commentColor);
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setFontItalic(true);
    multiLineCommentFormat.setForeground(commentColor);

    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}

void SyntaxHighlighter::setScriptingLanguageRules(){

    HighlightingRule rule;
    //Functions
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(functionsColor);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    keywordFormat.setForeground(keyword2Color);
    keywordFormat.setFontWeight(QFont::Bold);
    if(!knowedLang)
    {
        QStringList keywordPatterns;
        keywordPatterns << "\\bdef\\b" << "\\bnull\\b" << "\\bimport\\b" << "\\breturn\\b"
                        << "\\bbreak\\b" << "\\bdel\\b" << "\\brequire\\b"
                        << "\\bfor\\b" << "\\bforeach\\b" << "\\bif\\b" << "\\belse\\b"
                        << "\\bin\\b" << "\\bdo\\b" << "\\bwhile\\b" << "\\bnot\\b"
                        << "\\band\\b" << "\\bor\\b" << "\\bwith\\b" << "\\bas\\b"
                        << "\\bclass\\b" << "\\bprivate\\b" << "\\bpublic\\b" << "\\bnew\\b"
                        << "\\bprint\\b" << "\\becho\\b" << "\\btry\\b" << "\\bexcept\\b"
                        << "\\bend\\b";
        for (const QString &pattern : keywordPatterns) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }
    }
    else
    {
        for (const QString &pattern : difKeywordPatterns) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }
    }

    operatorFormat.setForeground(operatorColor);
    rule.pattern = QRegularExpression("\\b[+-*/=<>]+\\b");
    rule.format = operatorFormat;
    highlightingRules.append(rule);

    operatorFormat.setForeground(operatorColor);
    rule.pattern = QRegularExpression("\\s[+-*/=<>]+\\s");
    rule.format = operatorFormat;
    highlightingRules.append(rule);

    phpVarFormat.setForeground(varColor);
    rule.pattern = QRegularExpression("\\$\\w+\\b");
    rule.format = phpVarFormat;
    highlightingRules.append(rule);

    numberFormat.setForeground(numColor);
    rule.pattern = QRegularExpression("\\b[0-9\\.]+\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(valueColor);
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(valueColor);
    rule.pattern = QRegularExpression("'.*'");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setFontItalic(true);
    if(fileType != "py")
    {
        singleLineCommentFormat.setFontItalic(true);
        singleLineCommentFormat.setForeground(commentColor);
        rule.pattern = QRegularExpression("//[^\n]*");
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);
    }
    else
    {
        rule.pattern = QRegularExpression("[from]?.*import.?.*");
        rule.format = headerFileFormat;
        highlightingRules.append(rule);
    }

    singleLineCommentFormat.setForeground(commentColor);
    rule.pattern = QRegularExpression("#[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setFontItalic(true);
    multiLineCommentFormat.setForeground(commentColor);

    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}

void SyntaxHighlighter::setMarkupLanguageRules(){
    HighlightingRule rule;

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(functionsColor);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    tagFormat.setForeground(tagColor);
    rule.pattern = QRegularExpression("<\\w+\\s+[^>]*>");
    rule.format = tagFormat;
    highlightingRules.append(rule);

    tagFormat.setForeground(tagColor);
    rule.pattern = QRegularExpression("<\\w+>");
    rule.format = tagFormat;
    highlightingRules.append(rule);

    tagFormat.setForeground(tagColor);
    rule.pattern = QRegularExpression("</\\b\\w+\\b>");
    rule.format = tagFormat;
    highlightingRules.append(rule);

    keywordFormat.setForeground(htmlAttributesColor);
    QStringList keywordPatterns;
    keywordPatterns << "\\bclass\\b" << "\\bid\\b" << "\\bhref\\b" << "\\bsrc\\b"
                    << "\\blang\\b" << "\\bcharset\\b" << "\\bname\\b" << "\\bcontent\\b"
                    << "\\brel\\b" << "\\btype\\b";
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    quotationFormat.setForeground(valueColor);
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(valueColor);
    rule.pattern = QRegularExpression("'.*'");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setFontItalic(true);
    multiLineCommentFormat.setForeground(commentColor);

    commentStartExpression = QRegularExpression("<!--");
    commentEndExpression = QRegularExpression("-->");
}

void SyntaxHighlighter::setAsmRules(){
    HighlightingRule rule;

    functionFormat.setForeground(functionsColor);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_<>-@]+:");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    numberFormat.setForeground(numColor);
    rule.pattern = QRegularExpression("\\b[0-9a-fx\\.]+\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    keywordFormat.setForeground(keyword2Color);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bcall\\b" << "\\bpush\\b" << "\\bpop\\b" << "\\blea\\b"
                    << "\\bret\\b" << "\\bcmp\\b" << "\\bmov\\b" << "\\bint\\b"
                    << "\\bjmp\\b" << "\\bje\\b" << "\\bjne\\b" << "\\bjl\\b"
                    << "\\bjg\\b" << "\\bjge\\b" << "\\bjle\\b" << "\\bjz\\b"
                    << "\\bjnz\\b" << "\\bjb\\b" << "\\bja\\b" << "\\bjae\\b"
                    << "\\bjbe\\b" << "\\bjs\\b" << "\\bjns\\b" << "\\bleave\\b"
                    << "\\bxor\\b" << "\\bnot\\b" << "\\band\\b" << "\\bor\\b"
                    << "\\bsal\\b" << "\\bsar\\b" << "\\bshl\\b" << "\\bshr\\b"
                    << "\\bnop\\b" << "\\btest\\b" << "\\bxchg\\b"
                    << "\\badd\\b" << "\\bsub\\b" << "\\bmul\\b" << "\\bdiv\\b";

    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    quotationFormat.setForeground(valueColor);
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(valueColor);
    rule.pattern = QRegularExpression("'.*'");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setFontItalic(true);
    singleLineCommentFormat.setForeground(commentColor);
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(commentColor);
    rule.pattern = QRegularExpression(";[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setFontItalic(true);
    multiLineCommentFormat.setForeground(commentColor);

    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}

void SyntaxHighlighter::setCssRules(){
    HighlightingRule rule;
    keywordFormat.setForeground(keywordColor);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bhtml\\b" << "\\bbody\\b" << "\\bp\\b" << "\\ba\\b" << "\\bhr\\b"
                    << "\\bimg\\b" << "\\bheader\\b" << "\\bfooter\\b" << "\\bh1\\b"
                    << "\\bh2\\b" << "\\bh3\\b" << "\\bh4\\b" << "\\bh5\\b" << "\\bh6\\b"
                    << "\\bul\\b" << "\\bol\\b" << "\\bli\\b" << "\\bmain\\b" << "\\bnav\\b"
                    << "\\bmenu\\b" << "\\bmenuitem\\b" << "\\bq\\b" << "\\btable\\b" << "\\btd\\b";
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    numberFormat.setForeground(numColor);
    rule.pattern = QRegularExpression("\\b[0-9\\.]+\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    valueFormat.setForeground(valueColor);
    rule.pattern = QRegularExpression("\\b[0-9\\.]+[pt|px]+\\b");
    rule.format = valueFormat;
    highlightingRules.append(rule);

    attributeFormat.setForeground(cssAttributesColor);
    rule.pattern = QRegularExpression("\\w+:");
    rule.format = attributeFormat;
    highlightingRules.append(rule);

    attributeFormat.setForeground(cssAttributesColor);
    rule.pattern = QRegularExpression("\\w+-\\w+:");
    rule.format = attributeFormat;
    highlightingRules.append(rule);

    idFormat.setForeground(cssClassesIDsColor);
    rule.pattern = QRegularExpression("#\\w+\\b");
    rule.format = idFormat;
    highlightingRules.append(rule);

    classFormat.setForeground(cssClassesIDsColor);
    rule.pattern = QRegularExpression("\\.\\w+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(valueColor);
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(valueColor);
    rule.pattern = QRegularExpression("'.*'");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}

void SyntaxHighlighter::setSqlRules(){
    HighlightingRule rule;

    keywordFormat.setForeground(keywordColor);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bSELECT\\b" << "\\bFROM\\b" << "\\bWHERE\\b" << "\\bAND\\b" << "\\bOR\\b"
                    << "\\bDELETE\\b" << "\\bORDER\\b" << "\\bUNION\\b" << "\\bUPDATE\\b"
                    << "\\bINSERT\\b" << "\\bINTO\\b" << "\\bVALUES\\b" << "\\bCREATE\\b"
                    << "\\bTABLE\\b" << "\\bAS\\b" << "\\bBY\\b" << "\\bFOR\\b"
                    << "\\bIF\\b" << "\\bNOT\\b" << "\\bEXISTS\\b";
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    quotationFormat.setForeground(valueColor);
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(valueColor);
    rule.pattern = QRegularExpression("'.*'");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}

void SyntaxHighlighter::setupKeywordPatterns(QString fileType, int stdVersion)
{
    if ((fileType == "cpp" || fileType == "hpp" || fileType == "h") && stdVersion == 99)
    {
     difKeywordPatterns << "\\bchar\\b" << "\\bint\\b" << "\\bfloat\\b" << "\\bdouble\\b"
                        << "\\bstruct\\b" << "\\benum\\b" << "\\bvoid\\b" << "\\bshort\\b"
                        << "\\blong\\b" << "\\btrue\\b" << "\\bfalse\\b" << "\\bboolean\\b"
                        << "\\bthis\\b" << "\\bfriend\\b" <<"\\bconstexpr\\b"

                        << "\\bconst\\b" << "\\bstatic\\b" << "\\bsigned\\b" << "\\bunsigned\\b"
                        << "\\bnamespace\\b" << "\\breturn\\b" << "\\busing\\b"
                        << "\\bfor\\b" << "\\bwhile\\b" << "\\bif\\b" << "\\belse\\b"
                        << "\\bcase\\b" << "\\bswitch\\b" << "\\bdo\\b" << "\\bunion\\b"

                        << "\\bnew\\b" << "\\bclass\\b" << "\\bprivate\\b" << "\\bprotected\\b"
                        << "\\bpublic\\b" << "\\bvirtual\\b" << "\\bextern\\b" << "\\bvolatile\\b"
                        << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                        << "\\btry\\b" << "\\bcatch\\b" << "\\bthrow\\b" << "\\bbreak\\b"
                        << "\\bgoto\\b" << "\\bregister\\b" << "\\binline\\b"

                            << "\\band\\b" << "\\bbitor\\b" << "\\bor\\b" << "\\bxor\\b"
                            << "\\bor_eq\\b" << "\\band_eq\\b" << "\\bbitand\\b" << "\\bcompl\\b"
                            << "\\bxor_eq\\b" << "\\bnot\\b" << "\\bnot_eq\\b"

                        << "\\basm\\b" << "\\bauto\\b"
                        << "\\bbool\\b" << "\\bcontinue\\b"
                        << "\\bdefault\\b" << "\\bdelete\\b" << "\\bdynamic_cast\\b"
                        << "\\bexplicit\\b" << "\\bexport\\b" << "\\bmutable\\b"
                        << "\\boperator\\b" << "\\breinterpret_cast\\b" << "\\btypeid\\b"
                        << "\\bstatic_cast\\b" << "\\bwchar_t\\b" << "\\bfinal\\b" << "\\boverride\\b";
    }
    else if ((fileType == "cpp" || fileType == "hpp" || fileType == "h") && stdVersion == 14)
    {
         difKeywordPatterns << "\\bchar\\b" << "\\bint\\b" << "\\bfloat\\b" << "\\bdouble\\b"
                            << "\\bstruct\\b" << "\\benum\\b" << "\\bvoid\\b" << "\\bshort\\b"
                            << "\\blong\\b" << "\\btrue\\b" << "\\bfalse\\b" << "\\bboolean\\b"
                            << "\\bnullptr\\b" << "\\bthis\\b" << "\\bfriend\\b" <<"\\bconstexpr\\b"

                            << "\\bconst\\b" << "\\bstatic\\b" << "\\bsigned\\b" << "\\bunsigned\\b"
                            << "\\bnamespace\\b" << "\\breturn\\b" << "\\busing\\b"
                            << "\\bfor\\b" << "\\bwhile\\b" << "\\bif\\b" << "\\belse\\b"
                            << "\\bcase\\b" << "\\bswitch\\b" << "\\bdo\\b" << "\\bunion\\b"

                            << "\\bnew\\b" << "\\bclass\\b" << "\\bprivate\\b" << "\\bprotected\\b"
                            << "\\bpublic\\b" << "\\bvirtual\\b" << "\\bextern\\b" << "\\bvolatile\\b"
                            << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                            << "\\btry\\b" << "\\bcatch\\b" << "\\bthrow\\b" << "\\bbreak\\b"
                            << "\\bgoto\\b" << "\\binline\\b" << "\\bthread_local\\b"

                            << "\\band\\b" << "\\bbitor\\b" << "\\bor\\b" << "\\bxor\\b"
                            << "\\bor_eq\\b" << "\\band_eq\\b" << "\\bbitand\\b" << "\\bcompl\\b"
                            << "\\bxor_eq\\b" << "\\bnot\\b" << "\\bnot_eq\\b"

                            << "\\balignas\\b" << "\\balignof\\b" << "\\basm\\b" << "\\bauto\\b"
                            << "\\bbool\\b" << "\\bchar16_t\\b" << "\\bchar32_t\\b" << "\\bcontinue\\b"
                            << "\\bdecltype\\b" << "\\bdefault\\b" << "\\bdelete\\b" << "\\bdynamic_cast\\b"
                            << "\\bexplicit\\b" << "\\bexport\\b" << "\\bmutable\\b" << "\\bnoexcept\\b"
                            << "\\boperator\\b" << "\\breinterpret_cast\\b" << "\\btypeid\\b" << "\\bstatic_assert\\b"
                            << "\\bstatic_cast\\b" << "\\bwchar_t\\b" << "\\bfinal\\b" << "\\boverride\\b"
                            << "\\bint8_t\\b" << "\\bint16_t\\b" << "\\bint32_t\\b" << "\\bint64_t\\b"
                            << "\\bint_fast8_t\\b" << "\\bint_fast16_t\\b" << "\\bint_fast32_t\\b" << "\\bint_fast64_t\\b"
                            << "\\bint_least8_t\\b" << "\\bint_least16_t\\b" << "\\bint_least32_t\\b" << "\\bint_least64_t\\b"
                            << "\\buint8_t\\b" << "\\buint16_t\\b" << "\\buint32_t\\b" << "\\buint64_t\\b"
                            << "\\buint_fast8_t\\b" << "\\buint_fast16_t\\b" << "\\buint_fast32_t\\b" << "\\buint_fast64_t\\b"
                            << "\\buint_least8_t\\b" << "\\buint_least16_t\\b" << "\\buint_least32_t\\b" << "\\buint_least64_t\\b"
                            << "\\bintmax_t\\b" << "\\bwintptr_t\\b" << "\\buintmax_t\\b" << "\\buintptr_t\\b";


    }
    else if ((fileType == "cpp" || fileType == "hpp" || fileType == "h") && stdVersion == 20)
    {
         difKeywordPatterns << "\\bchar\\b" << "\\bint\\b" << "\\bfloat\\b" << "\\bdouble\\b"
                            << "\\bstruct\\b" << "\\benum\\b" << "\\bvoid\\b" << "\\bshort\\b"
                            << "\\blong\\b" << "\\btrue\\b" << "\\bfalse\\b" << "\\bboolean\\b"
                            << "\\bnullptr\\b" << "\\bthis\\b" << "\\bfriend\\b" <<"\\bconstexpr\\b"

                            << "\\bconst\\b" << "\\bstatic\\b" << "\\bsigned\\b" << "\\bunsigned\\b"
                            << "\\bnamespace\\b" << "\\breturn\\b" << "\\busing\\b"
                            << "\\bfor\\b" << "\\bwhile\\b" << "\\bif\\b" << "\\belse\\b"
                            << "\\bcase\\b" << "\\bswitch\\b" << "\\bdo\\b" << "\\bunion\\b"

                            << "\\bnew\\b" << "\\bclass\\b" << "\\bprivate\\b" << "\\bprotected\\b"
                            << "\\bpublic\\b" << "\\bvirtual\\b" << "\\bextern\\b" << "\\bvolatile\\b"
                            << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                            << "\\btry\\b" << "\\bcatch\\b" << "\\bthrow\\b" << "\\bbreak\\b"
                            << "\\bgoto\\b"<< "\\binline\\b"<< "\\bthread_local\\b"

                            << "\\balignas\\b" << "\\balignof\\b" << "\\basm\\b" << "\\bauto\\b"
                            << "\\bbool\\b" << "\\bchar16_t\\b" << "\\bchar32_t\\b" << "\\bcontinue\\b"
                            << "\\bdecltype\\b" << "\\bdefault\\b" << "\\bdelete\\b" << "\\bdynamic_cast\\b"
                            << "\\bexplicit\\b" << "\\bexport\\b" << "\\bmutable\\b" << "\\bnoexcept\\b"
                            << "\\boperator\\b" << "\\breinterpret_cast\\b" << "\\btypeid\\b" << "\\bstatic_assert\\b"
                            << "\\bstatic_cast\\b" << "\\bwchar_t\\b" << "\\bfinal\\b" << "\\boverride\\b"

                            << "\\band\\b" << "\\bbitor\\b" << "\\bor\\b" << "\\bxor\\b"
                            << "\\bor_eq\\b" << "\\band_eq\\b" << "\\bbitand\\b" << "\\bcompl\\b"
                            << "\\bxor_eq\\b" << "\\bnot\\b" << "\\bnot_eq\\b"

                            // C++20:
                            << "\\bchar8_t \\b" << "\\bconcept\\b" << "\\bconsteval\\b" << "\\bconstinit\\b"
                            << "\\bco_await\\b" << "\\bco_return \\b" << "\\bco_yield\\b" << "\\brequires\\b"
                            << "\\bstatic_cast\\b" << "\\bwchar_t\\b" << "\\bfinal\\b" << "\\boverride\\b"
                            << "\\bstatic_cast\\b" << "\\bwchar_t\\b" << "\\bfinal\\b" << "\\boverride\\b";
    }
    else if (stdVersion == 11){
     difKeywordPatterns << "\\bchar\\b" << "\\bint\\b" << "\\bfloat\\b" << "\\bdouble\\b"
                        << "\\bstruct\\b" << "\\benum\\b" << "\\bvoid\\b" << "\\bshort\\b"
                        << "\\blong\\b"
                        << "\\bconst\\b" << "\\bstatic\\b" << "\\bsigned\\b" << "\\bunsigned\\b"
                        << "\\breturn\\b"
                        << "\\bfor\\b" << "\\bwhile\\b" << "\\bif\\b" << "\\belse\\b"
                        << "\\bcase\\b" << "\\bswitch\\b" << "\\bdo\\b" << "\\bunion\\b"
                        << "\\bvolatile\\b" <<"\\bextern\\b" << "\\bgoto\\b" << "\\bregister\\b"
                        << "\\btypedef\\b" <<"\\bsizeof\\b" << "\\brestrict\\b" << "\\binline\\b"
                            << "\\band\\b" << "\\bbitor\\b" << "\\bor\\b" << "\\bxor\\b"
                            << "\\bor_eq\\b" << "\\band_eq\\b" << "\\bbitand\\b" << "\\bcompl\\b"
                            << "\\bxor_eq\\b" << "\\bnot\\b" << "\\bnot_eq\\b"
                        // from C99:
                        << "\\brestrict\\b" << "\\binline\\b"
                        << "\\b_Complex\\b" << "\\b_Bool\\b" << "\\b_Imaginary\\b"

                            << "\\bint8_t\\b" << "\\bint16_t\\b" << "\\bint32_t\\b" << "\\bint64_t\\b"
                            << "\\bint_fast8_t\\b" << "\\bint_fast16_t\\b" << "\\bint_fast32_t\\b" << "\\bint_fast64_t\\b"
                            << "\\bint_least8_t\\b" << "\\bint_least16_t\\b" << "\\bint_least32_t\\b" << "\\bint_least64_t\\b"
                            << "\\buint8_t\\b" << "\\buint16_t\\b" << "\\buint32_t\\b" << "\\buint64_t\\b"
                            << "\\buint_fast8_t\\b" << "\\buint_fast16_t\\b" << "\\buint_fast32_t\\b" << "\\buint_fast64_t\\b"
                            << "\\buint_least8_t\\b" << "\\buint_least16_t\\b" << "\\buint_least32_t\\b" << "\\buint_least64_t\\b"
                            << "\\bintmax_t\\b" << "\\bwintptr_t\\b" << "\\buintmax_t\\b" << "\\buintptr_t\\b";

    }
    else if (stdVersion == 18){
         difKeywordPatterns << "\\bchar\\b" << "\\bint\\b" << "\\bfloat\\b" << "\\bdouble\\b"
                            << "\\bstruct\\b" << "\\benum\\b" << "\\bvoid\\b" << "\\bshort\\b"
                            << "\\blong\\b"
                            << "\\bconst\\b" << "\\bstatic\\b" << "\\bsigned\\b" << "\\bunsigned\\b"
                            << "\\breturn\\b"

                            << "\\band\\b" << "\\bbitor\\b" << "\\bor\\b" << "\\bxor\\b"
                            << "\\bor_eq\\b" << "\\band_eq\\b" << "\\bbitand\\b" << "\\bcompl\\b"
                            << "\\bxor_eq\\b" << "\\bnot\\b" << "\\bnot_eq\\b"

                            << "\\bfor\\b" << "\\bwhile\\b" << "\\bif\\b" << "\\belse\\b"
                            << "\\bcase\\b" << "\\bswitch\\b" << "\\bdo\\b" << "\\bunion\\b"
                            << "\\bvolatile\\b" <<"\\bextern\\b" << "\\bgoto\\b" << "\\binline\\b"
                            << "\\btypedef\\b" <<"\\bsizeof\\b" << "\\brestrict\\b" << "\\bregister\\b"
                            // from C18:
                            << "\\b_Alignas\\b" << "\\b_Alignof\\b" << "\\b_Atomic\\b" << "\\b_Bool\\b"
                            << "\\b_Complex\\b" << "\\b_Generic\\b" << "\\b_Imaginary\\b" << "\\b_Noreturn\\b"
                            << "\\b_Static_assert\\b" << "\\b_Thread_local\\b";
        }
    else if (fileType == "py"){
        difKeywordPatterns     << "\\band\\b" << "\\bas\\b" << "\\bassert\\b" << "\\bbreak\\b"
                               << "\\bcontinue\\b" << "\\bdef\\b" << "\\bdel\\b"
                               << "\\belif\\b" << "\\belse\\b" << "\\bexcept\\b"
                               << "\\bfinally\\b" << "\\bfor\\b" << "\\bfrom\\b" << "\\bglobal\\b"
                               << "\\bif\\b" << "\\bimport\\b" << "\\bin\\b" << "\\bis\\b"
                               << "\\blambda\\b" << "\\bnot\\b" << "\\bor\\b" << "\\bpass\\b"
                               << "\\bprint\\b" << "\\braise\\b" << "\\breturn\\b" << "\\btry\\b"
                               << "\\bwhile\\b" << "\\bwith\\b" << "\\byield\\b"
                               << "\\bFalse\\b" << "\\bawait\\b" << "\\bNone\\b" << "\\bTrue\\b"
                               << "\\basync\\b" << "\\bnonlocal\\b" << "\\bclass\\b";
    }
    else if (fileType == "java"){

        difKeywordPatterns << "\\bchar\\b" << "\\bint\\b" << "\\bfloat\\b" << "\\bdouble\\b"
                        << "\\bstruct\\b" << "\\benum\\b" << "\\bvoid\\b" << "\\bshort\\b"
                        << "\\blong\\b" << "\\btrue\\b" << "\\bfalse\\b" << "\\bboolean\\b"
                        << "\\bnull\\b" << "\\bthis\\b" << "\\bfinal\\b"
                        << "\\band\\b" << "\\bor\\b" << "\\bxor\\b"
                        << "\\bconst\\b" << "\\bstatic\\b" << "\\bsigned\\b" << "\\bunsigned\\b"
                        << "\\bimport\\b" << "\\bnamespace\\b" << "\\breturn\\b" << "\\busing\\b"
                        << "\\bfor\\b" << "\\bwhile\\b" << "\\bif\\b" << "\\belse\\b"
                        << "\\bcase\\b" << "\\bswitch\\b" << "\\bdo\\b" << "\\bunion\\b"
                        << "\\bnew\\b" << "\\bclass\\b" << "\\bprivate\\b" << "\\bprotected\\b"
                        << "\\bpublic\\b" << "\\bvirtual\\b" << "\\bslots\\b" << "\\bvolatile\\b"
                        << "\\babstract\\b" << "\\bextends\\b" << "\\bimplements\\b" << "\\bsuper\\b"
                        << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                        << "\\btry\\b" << "\\bcatch\\b" << "\\bthrow\\b" << "\\bbreak\\b";
    }
}

void SyntaxHighlighter::setColorValues(QString theme){
    if (theme == "monokai"){
        // monokai
        commonTextColorIsWhite = 1;
        keywordColor = QColor(102, 217, 239);
        keyword2Color = QColor(249, 38, 114);
        functionsColor = QColor(166, 226, 46);
        valueColor = QColor(230, 218, 117);
        numColor = QColor(174, 130, 255);
        operatorColor = QColor(249, 38, 114);
        formatStringColor = QColor(174, 130, 255);
        commentColor = QColor(178, 179, 191);
        varColor = QColor(102, 217, 239);
        tagColor = QColor(249, 38, 114);
        htmlAttributesColor = QColor(166, 226, 46);
        cssClassesIDsColor = QColor(166, 226, 46);
        cssAttributesColor = QColor(102, 217, 239);
    } else if (theme == "tomorrow"){
        // tomorrow
        commonTextColorIsWhite = 0;
        keywordColor = QColor(135, 88, 166);
        keyword2Color = QColor(135, 88, 166);
        functionsColor = QColor(66, 114, 173);
        valueColor = QColor(112, 138, 0);
        numColor = QColor(245, 135, 32);
        operatorColor = QColor(77, 77, 76);
        formatStringColor = QColor(199, 40, 40);
        commentColor = QColor(144, 143, 140);
        varColor = QColor(199, 40, 40);
        tagColor = QColor(199, 40, 40);
        htmlAttributesColor = QColor(245, 135, 32);
        cssClassesIDsColor = QColor(62, 153, 158);
        cssAttributesColor = QColor(77, 77, 76);
    } else if (theme == "tomorrowNight"){
        // tomorrow night
        commonTextColorIsWhite = 1;
        keywordColor = QColor(177, 149, 186);
        keyword2Color = QColor(177, 149, 186);
        functionsColor = QColor(128, 162, 189);
        valueColor = QColor(182, 189, 106);
        numColor = QColor(222, 146, 95);
        operatorColor = QColor(197, 199, 198);
        formatStringColor = QColor(222, 146, 95);
        commentColor = QColor(149, 150, 149);
        varColor = QColor(204, 102, 102);
        tagColor = QColor(204, 102, 102);
        htmlAttributesColor = QColor(222, 146, 95);
        cssClassesIDsColor = QColor(138, 189, 181);
        cssAttributesColor = QColor(197, 199, 198);
    } else {
        // Solarized
        commonTextColorIsWhite = 0;
        keywordColor = QColor(181, 137, 0);
        keyword2Color = QColor(133, 153, 0);
        functionsColor = QColor(88, 110, 117);
        valueColor = QColor(42, 161, 152);
        numColor = QColor(42, 161, 152);
        operatorColor = QColor(181, 137, 0);
        formatStringColor = QColor(220, 50, 47);
        commentColor = QColor(178, 179, 191);
        varColor = QColor(38, 139, 210);
        tagColor = QColor(38, 139, 210);
        htmlAttributesColor = QColor(181, 137, 0);
        cssClassesIDsColor = QColor(133, 153, 0);
        cssAttributesColor = QColor(77,171,171);
    }
}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    if (commentStartExpression != QRegularExpression("") && commentEndExpression != QRegularExpression("")) {
        int startIndex = 0;
        if (previousBlockState() != 1)
            startIndex = text.indexOf(commentStartExpression);

        while (startIndex >= 0) {
            QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
            int endIndex = match.capturedStart();
            int commentLength = 0;
            if (endIndex == -1) {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            }
            else
            {
                commentLength = endIndex - startIndex
                                + match.capturedLength();
            }
            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
        }
    }
}
