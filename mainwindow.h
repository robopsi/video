#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedLayout>
#include <QStackedWidget>
#include <videowidgets.h>
#include <base/basewindow.h>
#include <QThread>

class MainWindow : public baseWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QStackedLayout *m_mainlyout;
    QStackedWidget *m_stackedWid;
    videoWidgets *m_wid;
protected:
    void keyPressEvent(QKeyEvent *event);
private:
    bool mediaHasUpdate;

private:

    void initLayout();

private slots:
    void slot_appQuit();

public slots:
    void slot_returnanimation();
    void slot_updateMedia1();
    void slot_updateMedia2();
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
