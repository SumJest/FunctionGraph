#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QStringList path = QCoreApplication::libraryPaths();
    path.append(".");
    path.append("imageformats");
    path.append("platforms");
    path.append("sqldrivers");
    QCoreApplication::setLibraryPaths(path);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
