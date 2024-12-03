#include "mainwindow.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QIcon i(":/src/images/Head.png");
    w.setWindowTitle("QSnake");
    w.setWindowIcon(i);
    w.show();
    return a.exec();
}
