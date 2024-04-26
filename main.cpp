/*--------------------------------------------------------------*\
 * Image Editor in C++ using Qt
 *
 * Developped as a learning Qt's project
 *
 * 2024-04-24                       Raymond NGUYEN THANH
\*--------------------------------------------------------------*/

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
