#ifndef PASCALIZER_MAINWINDOW_H
#define PASCALIZER_MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Pascalizer_MainWindow;
}
QT_END_NAMESPACE

class Pascalizer_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit Pascalizer_MainWindow(QWidget* parent = nullptr);
    ~Pascalizer_MainWindow();

private:
    Ui::Pascalizer_MainWindow* ui;
};

#endif // PASCALIZER_MAINWINDOW_H
