#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedLayout>
#include <QStackedWidget>
#include <videowidgets.h>
#include <base/basewindow.h>
#include <QThread>

#include "MediaNotificationReceiver.h"

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
private:
    bool mediaHasUpdate;
    VideoWidgets *m_videoWid;
    // Thread for media resource update.
    MediaNotificationReceiver m_notificationReceiver;
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

class mediaUpdateThread:public QThread
{
public:
    mediaUpdateThread(QObject *parent ,MainWindow *mainWindow);
private:
    MainWindow *m_mainWindow;
protected:
    void run();
};

#endif // MAINWINDOW_H
