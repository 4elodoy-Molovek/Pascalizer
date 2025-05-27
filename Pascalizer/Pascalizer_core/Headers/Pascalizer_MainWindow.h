#pragma once


#include "ui_Pascalizer_MainWindow.h"
#include "IO_ProcessorInterface.h"

#include <QMainWindow>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QTextEdit>
#include <QStringList>



class Pascalizer_MainWindow : public QMainWindow, public IO_UI_Interface
{
    Q_OBJECT

protected:
   class Pascalizer* pascalizer;

   class ConsoleWidget* console;

public:
    explicit Pascalizer_MainWindow(class Pascalizer* inPascalizer, QWidget* parent = nullptr);

    void ShowError(std::string errorMessage);

    void SetupConsole();

    void UpdateSourceCode();

    // IO Interface
    // 
    // Call for outputing data using IO interface
    virtual void OutputString(const std::string& output) override;

    // Call for user input for I/O instructions
    virtual void ReceiveUserInput() override;

    // Called when the UI should be updated based on the actions inside the program
    virtual void Update() override;

    virtual void AddError(const std::string& error) override;

    virtual void ClearErrors() override;


signals:

public slots:

    void OnClickedOpen(bool checked = false);
    void OnClickedSave(bool checked = false);
    void OnClickedSaveAs(bool checked = false);
    void OnClickedNew(bool checked = false);

    void OnClickedRun(bool checked = false);

    void OnSourceCodeEdited();

    void OnConsoleEntered(const QString& command);

private:
    Ui::PascalizerMainWindow ui;
};


// ==========================
// Syntax Highlighting

class SyntaxHighlighter : public QSyntaxHighlighter 
{
    Q_OBJECT

public:
    SyntaxHighlighter(QTextDocument* parent = nullptr);

protected:
    void highlightBlock(const QString& text) override;

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> rules;

    QTextCharFormat keywordFormat;
    QTextCharFormat commentFormat;
    QTextCharFormat stringFormat;
};


// ===========================
// Console Widget
class ConsoleWidget : public QTextEdit
{
    Q_OBJECT

public:
    explicit ConsoleWidget(QWidget * parent = nullptr);

    void printOutput(const QString& text);
    void printPrompt(const QString& prompt = "> ");

signals:
    void commandEntered(const QString& command);

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    int inputStartPos;
    QString currentPrompt;

    void insertPrompt();
    QString getCurrentInput() const;
};