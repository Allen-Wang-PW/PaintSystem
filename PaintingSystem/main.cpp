#include "mainwindow.h"
#include <QApplication>
#include "paintdialog.h"
#include "threedwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //threeDWindow threeD;
    //threeD.show();

    return a.exec();
}
