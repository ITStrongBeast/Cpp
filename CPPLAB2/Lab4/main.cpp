#include "mainwindow.h"
#include "startgame.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    if (strcmp(argv[1], "dbg")) {
        w.peeping();
    }
    w.show();
    return a.exec();
}




















































