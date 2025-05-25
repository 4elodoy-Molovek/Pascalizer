#pragma once


#include "ui_Pascalizer_MainWindow.h"
#include "IO_ProcessorInterface.h"

#include <QMainWindow>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>



class Pascalizer_MainWindow : public QMainWindow, public IO_UI_Interface
{
    Q_OBJECT

protected:
   class Pascalizer* pascalizer;

public:
    explicit Pascalizer_MainWindow(class Pascalizer* inPascalizer, QWidget* parent = nullptr);

    void ShowError(std::string errorMessage);

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

    void OnClickedRun(bool checked = false);

    void OnSourceCodeEdited();

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