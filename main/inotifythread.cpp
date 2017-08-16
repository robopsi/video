#include "inotifythread.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <QDirIterator>

#include "global_value.h"

#define EVENT_NUM 12

char *event_str[EVENT_NUM] =
{
    (char*)"IN_ACCESS",
    (char*)"IN_MODIFY",
    (char*)"IN_ATTRIB",
    (char*)"IN_CLOSE_WRITE",
    (char*)"IN_CLOSE_NOWRITE",
    (char*)"IN_OPEN",
    (char*)"IN_MOVED_FROM",
    (char*)"IN_MOVED_TO",
    (char*)"IN_CREATE",
    (char*)"IN_DELETE",
    (char*)"IN_DELETE_SELF",
    (char*)"IN_MOVE_SELF"
};

InotifyThread::InotifyThread(QObject *parent):QThread(parent)
{

}

QList<QString> getAllDirPath(const QString& path)
{
    QList<QString> dirPathList;
    QFileInfo pathInfo(path);
    if(pathInfo.isDir()){
        dirPathList.append(pathInfo.absoluteFilePath());
    }

    QDirIterator it(path,QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot);
    while (it.hasNext()){
        QString name = it.next();
        QFileInfo info(name);
        if (info.isDir()){
            if(!dirPathList.contains(info.absoluteFilePath())){
                dirPathList.append(info.absoluteFilePath());
                dirPathList.append(getAllDirPath(name));
            }
        }
    }
    return dirPathList;
}

void InotifyThread::run()
{
    int fd;
    int len;
    int nread;
    char buf[BUFSIZ];
    struct inotify_event *event;
    int i;

    fd = inotify_init();
    if( fd < 0 ){
        fprintf(stderr, "inotify_init failed\n");
        return;
    }

    QList<QString> pathList = getAllDirPath(VIDEO_SEARCH_PATH);
    for(int i=0;i<pathList.size();i++){
        qDebug("Add inotify path: %s",pathList.at(i).toLatin1().data());
        inotify_add_watch(fd, pathList.at(i).toLatin1().data(),
                          IN_CREATE | IN_DELETE | IN_DELETE_SELF);
    }

    inotify_add_watch(fd, VIDEO_SEARCH_PATH.toLatin1().data(),
                               IN_CREATE | IN_DELETE | IN_DELETE_SELF );

    buf[sizeof(buf) - 1] = 0;
    while( (len = read(fd, buf, sizeof(buf) - 1)) > 0 ){
        nread = 0;
        while( len > 0 ){
            event = (struct inotify_event *)&buf[nread];
            for(i=0; i<EVENT_NUM; i++){
                if((event->mask >> i) & 1){
                    if(event->len > 0){
                        fprintf(stdout, "%s --- %s\n", event->name, event_str[i]);
                        if(mainWindow!=NULL){
                            emit mainWindow->beginUpdateMediaResource();
                        }
                    }
                }
            }
            nread = nread + sizeof(struct inotify_event) + event->len;
            len = len - sizeof(struct inotify_event) - event->len;
        }
    }
}
