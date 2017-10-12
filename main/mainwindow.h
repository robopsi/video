#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedLayout>
#include <QStackedWidget>
#include <videowidgets.h>
#include <base/basewindow.h>
#include <QThread>

#include "MediaNotificationReceiver.h"

class MediaUpdateThread;

/**
 * The main window of application.
 *
 * Used for global control.such as keypress response、media update、
 * initial interface etc.
 */
class MainWindow : public BaseWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow(){}
protected:
    void keyPressEvent(QKeyEvent *event);
    // Used for disable or enable application when car-reverse event comes.
    void disableApplication();
    void enableApplication();
public:
    VideoWidgets* getVideoWidget(){return m_videoWid;}
    void exitApplication();

private:
    bool mediaHasUpdate;
    VideoWidgets *m_videoWid;

    MediaNotificationReceiver m_notificationReceiver;
    MediaUpdateThread *mediaUpdateThread;

private:
    void initData();
    void initLayout();
    void initConnection();

public slots:
    void slot_updateMedia();
private slots:
    // Slot in order to update media resource.
    void slot_setUpdateFlag();
    void slot_updateUiByRes(QFileInfoList videoFileList);

signals:
    void beginUpdateMediaResource();
    void updateUiByRes(QFileInfoList videoFileList);
};

class MediaUpdateThread:public QThread
{
public:
    MediaUpdateThread(QObject *parent ,MainWindow *mainWindow);
    ~MediaUpdateThread(){}

    void waitForThreadFinished();

private:
    MainWindow *m_mainWindow;

    // List of search suffix when search video resource.
    QList<QString> m_searchSuffixList;
    QFileInfoList findVideoFiles(const QString& path);

protected:
    void run();
};

#endif // MAINWINDOW_H
