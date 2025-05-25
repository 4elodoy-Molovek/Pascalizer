#include "Pascalizer_MainWindow.h"
#include "ui_Pascalizer_MainWindow.h"
#include "Pascalizer.h"

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>


Pascalizer_MainWindow::Pascalizer_MainWindow(Pascalizer* inPascalizer, QWidget* parent) : QMainWindow(parent), pascalizer(inPascalizer)
{
    ui.setupUi(this);  // This connects widgets from the .ui file

    QObject::connect(ui.actionOpen, &QAction::triggered, this, &Pascalizer_MainWindow::OnClickedOpen);
    QObject::connect(ui.actionSave, &QAction::triggered, this, &Pascalizer_MainWindow::OnClickedSave);
    QObject::connect(ui.actionSave_As, &QAction::triggered, this, &Pascalizer_MainWindow::OnClickedSaveAs);

    QObject::connect(ui.actionRun, &QAction::triggered, this, &Pascalizer_MainWindow::OnClickedRun);

    QObject::connect(ui.codeEditor, &QPlainTextEdit::textChanged, this, &Pascalizer_MainWindow::OnSourceCodeEdited);

    // Attach the syntax highlighter to the plain text edit
    new SyntaxHighlighter(ui.codeEditor->document());
}

void Pascalizer_MainWindow::ShowError(std::string errorMessage)
{
    throw(std::runtime_error(errorMessage));
}

void Pascalizer_MainWindow::UpdateSourceCode()
{
    ui.codeEditor->setPlainText(QString::fromStdString(pascalizer->GetFileModule().GetSourceCode()));
}


// Signals
void Pascalizer_MainWindow::OnClickedOpen(bool checked)
{
    // Calling file dialog
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Open Source Code File",
        "", // optional starting directory
        "Text Files (*.txt);;All Files (*)" // file filters
    );

    // User cancelled
    if (fileName.isEmpty())
        return; 

    try
    {
        pascalizer->GetFileModule().LoadFile(fileName.toStdString());
        UpdateSourceCode();
    }

    catch (std::exception e)
    {
        ShowError(e.what());
    }
}

void Pascalizer_MainWindow::OnClickedSave(bool checked)
{
    try
    {
        pascalizer->GetFileModule().SaveFile();
    }

    catch (std::exception e)
    {
        ShowError(e.what());
    }
}

void Pascalizer_MainWindow::OnClickedSaveAs(bool checked)
{
    // Calling file dialog
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save Source Code File",
        "", // optional starting directory
        "Text Files (*.txt);;All Files (*)" // file filters
    );

    // User cancelled
    if (fileName.isEmpty())
        return;

    try
    {
        pascalizer->GetFileModule().SaveFileAs(fileName.toStdString());
    }

    catch (std::exception e)
    {
        ShowError(e.what());
    }
}



void Pascalizer_MainWindow::OnClickedRun(bool checked)
{
    pascalizer->InterpreteCurrentFileSourceCode();
}

void Pascalizer_MainWindow::OnSourceCodeEdited()
{
    pascalizer->GetFileModule().GetSourceCode() = ui.codeEditor->toPlainText().toStdString();
}

// ==========================
// Syntax Highlighting

SyntaxHighlighter::SyntaxHighlighter(QTextDocument* parent) : QSyntaxHighlighter(parent)
{
    // Keywords
    keywordFormat.setForeground(QColor(200, 10, 255));
    keywordFormat.setFontWeight(QFont::Bold);
    const QStringList mainKeywords = 
    {
        "\\bprogram\\b", "\\bbegin\\b", "\\bend\\b", "\\bvar\\b", "\\bconst\\b", "\\bif\\b", "\\belse\\b", "\\bwhile\\b"
    };


    for (const QString& pattern : mainKeywords)
        rules.append({ QRegularExpression(pattern), keywordFormat });

    keywordFormat.setForeground(QColor(255, 100, 0));
    keywordFormat.setFontWeight(QFont::Bold);
    const QStringList typeKeywords =
    {
        "\\bint\\b", "\\bdouble\\b", "\\bstring\\b"
    };


    for (const QString& pattern : typeKeywords)
        rules.append({ QRegularExpression(pattern), keywordFormat });


    // Strings
    stringFormat.setForeground(Qt::green);
    rules.append({ QRegularExpression("\".*\""), stringFormat });

    // Single-line comments
    commentFormat.setForeground(Qt::darkGray);
    commentFormat.setFontItalic(true);
    rules.append({ QRegularExpression("//[^\n]*"), commentFormat });
}

void SyntaxHighlighter::highlightBlock(const QString& text)
{
    for (const HighlightingRule& rule : rules) 
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) 
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
