/*
 * Student information:
 * - Name: Trung Van
 * - Email: trung.van@tuni.fi
 * - ID: H293914
*/
#include "mainwindow.hh"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
