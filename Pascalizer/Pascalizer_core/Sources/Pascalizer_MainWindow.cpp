#include "Pascalizer_MainWindow.h"
#include "Pascalizer.h"

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QKeyEvent>


Pascalizer_MainWindow::Pascalizer_MainWindow(Pascalizer* inPascalizer, QWidget* parent) : QMainWindow(parent), pascalizer(inPascalizer)
{
    ui.setupUi(this);  // This connects widgets from the .ui file

    pascalizer->SetUserInterface(this);

    QObject::connect(ui.actionOpen, &QAction::triggered, this, &Pascalizer_MainWindow::OnClickedOpen);
    QObject::connect(ui.actionSave, &QAction::triggered, this, &Pascalizer_MainWindow::OnClickedSave);
    QObject::connect(ui.actionSave_As, &QAction::triggered, this, &Pascalizer_MainWindow::OnClickedSaveAs);
    QObject::connect(ui.actionNew, &QAction::triggered, this, &Pascalizer_MainWindow::OnClickedNew);

    QObject::connect(ui.actionRun, &QAction::triggered, this, &Pascalizer_MainWindow::OnClickedRun);

    QObject::connect(ui.codeEditor, &QPlainTextEdit::textChanged, this, &Pascalizer_MainWindow::OnSourceCodeEdited);

    SetupConsole();
    QObject::connect(console, &ConsoleWidget::commandEntered, this, &Pascalizer_MainWindow::OnConsoleEntered);

    // Attach the syntax highlighter to the plain text edit
    new SyntaxHighlighter(ui.codeEditor->document());
}

void Pascalizer_MainWindow::ShowError(std::string errorMessage)
{
    throw(std::runtime_error(errorMessage));
}

void Pascalizer_MainWindow::SetupConsole()
{
    // Save parent and layout info
    QWidget* placeholder = ui.consoleP;
    QWidget* parent = placeholder->parentWidget();
    QLayout* layout = parent->layout();

    // Create the new console
    ConsoleWidget* consoleNew = new ConsoleWidget(parent);

    // Replace in layout
    if (layout) 
    {
        int index = layout->indexOf(placeholder);
        layout->removeWidget(placeholder);
        placeholder->deleteLater(); // remove the old widget
        layout->addWidget(consoleNew);
    }
    else {
        // Fallback: manually position
        consoleNew->setGeometry(placeholder->geometry());
        placeholder->hide();
    }

    // Optional: keep a reference
    console = consoleNew;

    console->setFontPointSize(25);
}

void Pascalizer_MainWindow::UpdateSourceCode()
{
    ui.codeEditor->setPlainText(QString::fromStdString(pascalizer->GetFileModule().GetSourceCode()));
}



void Pascalizer_MainWindow::OutputString(const std::string& output)
{
    console->printOutput(QString::fromStdString(output));
}

void Pascalizer_MainWindow::ReceiveUserInput()
{
}

void Pascalizer_MainWindow::Update()
{
    ui.executionLogList->clear();

    for (auto& logElement : pascalizer->GetCachedLog())
        ui.executionLogList->addItem(QString::fromStdString(logElement));
}

void Pascalizer_MainWindow::AddError(const std::string& error)
{
    ui.errorList->addItem(QString::fromStdString(error));
}

void Pascalizer_MainWindow::ClearErrors()
{
    ui.errorList->clear();
}

void Pascalizer_MainWindow::ClearEverything()
{
    console->clear();
    ui.errorList->clear();
    ui.executionLogList->clear();
}

void Pascalizer_MainWindow::OnFailedToLauch()
{
    ui.tabWidget->setCurrentIndex(0);
}

void Pascalizer_MainWindow::OnLauched()
{
    console->clear();
    ui.tabWidget->setCurrentIndex(1);
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
        ClearEverything();
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

void Pascalizer_MainWindow::OnClickedNew(bool checked)
{
    // Calling file dialog
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "New Source Code File",
        "", // optional starting directory
        "Text Files (*.txt);;All Files (*)" // file filters
    );

    // User cancelled
    if (fileName.isEmpty())
        return;

    try
    {
        pascalizer->GetFileModule().GetSourceCode() = "";
        pascalizer->GetFileModule().SaveFileAs(fileName.toStdString());
        UpdateSourceCode();
        ClearEverything();
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

void Pascalizer_MainWindow::OnConsoleEntered(const QString& command)
{
    pascalizer->OnUserInputReceived(command.toStdString());
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


// ===========================
// Console Widget

ConsoleWidget::ConsoleWidget(QWidget* parent) : QTextEdit(parent), inputStartPos(0), currentPrompt("> ")
{
    setUndoRedoEnabled(false);
    setAcceptRichText(false);
    setWordWrapMode(QTextOption::WrapAnywhere);
    //setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    setFontPointSize(25);
    insertPrompt();
}

void ConsoleWidget::insertPrompt() 
{
    moveCursor(QTextCursor::End);
    insertPlainText(currentPrompt);
    inputStartPos = textCursor().position();
}

void ConsoleWidget::printOutput(const QString& text) 
{
    moveCursor(QTextCursor::End);
    insertPlainText(text + "\n");
    insertPrompt();
}

void ConsoleWidget::printPrompt(const QString& prompt) 
{
    currentPrompt = prompt;
    insertPrompt();
}

QString ConsoleWidget::getCurrentInput() const 
{
    return toPlainText().mid(inputStartPos);
}

void ConsoleWidget::keyPressEvent(QKeyEvent* event) 
{
    QTextCursor cursor = textCursor();

    if (event->key() == Qt::Key_Backspace && cursor.position() <= inputStartPos) 
    {
        return;  // prevent deleting the prompt
    }

    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) 
    {
        QString input = getCurrentInput();
        moveCursor(QTextCursor::End);
        insertPlainText("\n");
        insertPrompt();

        emit commandEntered(input.trimmed());
        
        return;
    }

    // Prevent editing above the prompt
    if (cursor.position() < inputStartPos &&
        event->key() != Qt::Key_Left &&
        event->key() != Qt::Key_Right &&
        event->key() != Qt::Key_Up &&
        event->key() != Qt::Key_Down) 
    {
        setTextCursor(QTextCursor(document()->findBlock(inputStartPos)));
        return;
    }

    QTextEdit::keyPressEvent(event);
}