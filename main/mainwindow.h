#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedLayout>
#include <QStackedWidget>
#include <videowidgets.h>
#include <base/basewindow.h>
#include <QThread>

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
    ~MainWindow();
protected:
    void keyPressEvent(QKeyEvent *event);
public:
    VideoWidgets* getVideoWidget(){return m_videoWid;}
private:
    bool mediaHasUpdate;
    VideoWidgets *m_videoWid;
private:
    void initLayout();
    void initConnection();

public slots:
    void slot_appQuit();
    void slot_updateMedia();
private slots:
    // Slot in order to update media resource.
    void slot_setUpdateFlag();
    void slot_updateUiByRes(QFileInfoList videoFileList);

    void slot_standby();
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
