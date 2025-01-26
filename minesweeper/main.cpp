#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(argc > 1 && strcmp(argv[1], "dbg") == 0, nullptr);
    w.show();
    return a.exec();
}
