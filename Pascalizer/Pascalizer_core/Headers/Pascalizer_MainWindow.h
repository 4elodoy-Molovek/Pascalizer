#ifndef PASCALIZER_MAINWINDOW_H
#define PASCALIZER_MAINWINDOW_H

#include <QMainWindow>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class Pascalizer;

QT_BEGIN_NAMESPACE
namespace Ui {
    class Pascalizer_MainWindow;
}
QT_END_NAMESPACE

class Pascalizer_MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void OnClickedOpen(bool checked = false);
    void OnClickedSave(bool checked = false);
    void OnClickedSaveAs(bool checked = false);
    void OnClickedRun(bool checked = false);
    void OnSourceCodeEdited();

private:
    void ShowError(std::string errorMessage);
    void UpdateSourceCode();

public:
    // explicit Pascalizer_MainWindow(QWidget* parent = nullptr);
    explicit Pascalizer_MainWindow(Pascalizer* inPascalizer, QWidget* parent = nullptr);
    ~Pascalizer_MainWindow();

private:
    Ui::Pascalizer_MainWindow* ui;
    Pascalizer* pascalizer;
};

#endif // PASCALIZER_MAINWINDOW_H


struct HighlightingRule 
{
    QRegularExpression pattern;
    QTextCharFormat format;
};

class SyntaxHighlighter : public QSyntaxHighlighter
{
public:
    SyntaxHighlighter(QTextDocument* parent);

protected:
    void highlightBlock(const QString& text) override;

private:
    QVector<HighlightingRule> rules;
    QTextCharFormat keywordFormat;
    QTextCharFormat stringFormat;
    QTextCharFormat commentFormat;
};