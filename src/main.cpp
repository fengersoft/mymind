#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>
#include <QProcess>

int main(int argc, char* argv[])
{
    //#ifdef Q_OS_ANDROID
    //    QFontDatabase::addApplicationFont("assets:/fonts/simsun.ttf");

    //#endif
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    int ret = a.exec();
    if (ret == 773)
    {
        QProcess::startDetached(qApp->applicationFilePath());
    }

    return ret;
}
