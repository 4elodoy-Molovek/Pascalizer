#include <QtCore/QCoreApplication>
#include <QtWidgets/QApplication>
#include "Pascalizer_MainWindow.h"
#include "Pascalizer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Pascalizer* pascalizer = new Pascalizer();

    Pascalizer_MainWindow window(pascalizer);
    window.show();

    int result = a.exec();
    delete pascalizer;

    return result;
}
