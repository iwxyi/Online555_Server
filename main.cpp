#include "mainwindow.h"
#include <QApplication>
#include "globals.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    initGlobal();

    MainWindow w;
    w.show();

    return a.exec();
}
