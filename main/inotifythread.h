#ifndef INOTIFYTHREAD_H
#define INOTIFYTHREAD_H

#include <QThread>
#include <QMap>

class InotifyThread:public QThread
{
public:
    InotifyThread(QObject *parent=0);
    ~InotifyThread();
protected:
    void run();
};

#endif // INOTIFYTHREAD_H
