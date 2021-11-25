#include "SyntaxHighlighter.hpp"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent, QString fileType, QString theme, int std)
    : QSyntaxHighlighter(parent), fileType(fileType), stdVersion(std)
{
    commentStartExpression = QRegularExpression("");
    commentEndExpression = QRegularExpression("");
    setupSyntaxHighlighter(fileType, theme);
}

void SyntaxHighlighter::setupSyntaxHighlighter(QString fileType, QString theme)
{
    setColorValues(theme);

    setSyntax(fileType, stdVersion);
}

void SyntaxHighlighter::setSyntax(QString newFileType, int newStdVersion)
{
    fileType   = newFileType;
    stdVersion = newStdVersion;
    setupKeywordPatterns(fileType, stdVersion);
    setLangRules();
}

void SyntaxHighlighter::setTheme(QString filetype, QString theme)
{
    setupSyntaxHighlighter(filetype, theme);
    rehighlight();
}

void SyntaxHighlighter::setLangRules()
{
    HighlightingRule rule;

    //Functions
    functionFormat.setForeground(functionsColor);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    keywordFormat.setForeground(keywordColor);
    keywordFormat.setFontWeight(QFont::Bold);

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

void SyntaxHighlighter::setupKeywordPatterns(QString fileType, int stdVersion)
{
    #ifdef EXTRA_VERBOSE
        std::cerr << "fileType = #" << fileType.toStdString() << "# | stdVersion = " << stdVersion << " \n";
    #endif

    size_t i = 0;
    while (SUPPORTED_FILETYPES[i][0] != '\0') {
        if (fileType == QString(SUPPORTED_FILETYPES[i]))
            break;
        ++i;
    }
    if (SUPPORTED_FILETYPES[i][0] == '\0') 
        std::cerr << "ERROR: unknown fileType provided to SyntaxHighlighter!\n";


    if ((fileType == "cpp" || fileType == "hpp" || fileType == "h") && stdVersion == 99) {
        difKeywordPatterns  << "\\bchar\\b" << "\\bint\\b" << "\\bfloat\\b" << "\\bdouble\\b"
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


    }  else if ((fileType == "cpp" || fileType == "hpp" || fileType == "h") && stdVersion == 14) {
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


    } else if ((fileType == "cpp" || fileType == "hpp" || fileType == "h") && stdVersion == 20) {
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


    } else if ((fileType == "c" || fileType == "h") && stdVersion == 11){
        difKeywordPatterns  << "\\bchar\\b" << "\\bint\\b" << "\\bfloat\\b" << "\\bdouble\\b"
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
    

    } else if ((fileType == "c" || fileType == "h") && stdVersion == 18){
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
    } else {
        std::cerr << "ERROR: bad fileName or stdVersion provided to SyntaxHighlighter!\n";
    }
}

void SyntaxHighlighter::setColorValues(QString theme)
{
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

    } else if (theme == "solarized") {
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
    } else {
        std::cerr << "ERROR: unknown theme provided to SyntaxHighlighter!\n";
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
