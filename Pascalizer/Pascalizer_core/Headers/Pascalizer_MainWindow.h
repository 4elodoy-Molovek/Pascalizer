#pragma once

#include <QMainWindow>
#include "ui_Pascalizer_MainWindow.h"

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>



class Pascalizer_MainWindow : public QMainWindow 
{
    Q_OBJECT

protected:
   class Pascalizer* pascalizer;

public:
    explicit Pascalizer_MainWindow(class Pascalizer* inPascalizer, QWidget* parent = nullptr);

    void ShowError(std::string errorMessage);

    void UpdateSourceCode();

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