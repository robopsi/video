#include "mainwindow.h"
#include <QApplication>
#include "ueventthread.h"
#include "inotifythread.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showFullScreen();
    ueventThread thread;
    thread.start();

    inotifyThread thread2;
    thread2.start();

    return a.exec();
}
