#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char* argv[])
{
    //#ifdef Q_OS_ANDROID
    //    QFontDatabase::addApplicationFont("assets:/fonts/simsun.ttf");

    //#endif
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
