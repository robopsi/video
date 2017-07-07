#include "mainwindow.h"
#include "global_value.h"
#include "ueventthread.h"
#include "inotifythread.h"

MainWindow::MainWindow(QWidget *parent) :baseWindow(parent),mediaHasUpdate(false)

{
    mainwid=this;
    initLayout();

    connect(m_wid->m_topWid->m_btnmini,SIGNAL(clicked(bool)),this,SLOT(showMinimized()));
    connect(m_wid->m_topWid->m_btnexit,SIGNAL(clicked(bool)),this,SLOT(slot_appQuit()));
    connect(m_wid->m_topWid->m_btnreturn,SIGNAL(clicked(bool)),this,SLOT(slot_returnanimation()));
    connect(this,SIGNAL(beginUpdateMediaResource()),this,SLOT(slot_updateMedia1()));
    connect(this,SIGNAL(updateUiByRes(QFileInfoList)),this,SLOT(slot_updateUiByRes(QFileInfoList)));
    slot_updateMedia2();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initLayout(){
   QVBoxLayout *mainLayout = new QVBoxLayout;
    m_wid = new videoWidgets(this);
    mainLayout->addWidget(m_wid);
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);
}
void MainWindow::slot_appQuit()
{
    m_wid->savaSetting();
    this->close();
}
void MainWindow::slot_returnanimation()
{
     this->close();
}
void MainWindow::slot_updateMedia1()
{
    if(!mediaHasUpdate)
    {
        mediaHasUpdate = true;
        QTimer::singleShot(2000,this,SLOT(slot_updateMedia2()));
    }
}
void MainWindow::slot_updateMedia2()
{
    qDebug()<<"Carmachine: Update media resource.";
    mediaUpdateThread *thread = new mediaUpdateThread(this,this);
    thread->start();
    mediaHasUpdate =false;
}
mediaUpdateThread::mediaUpdateThread(QObject *parent,MainWindow *mainWindow):QThread(parent)
{
    m_mainWindow = mainWindow;
    qRegisterMetaType<QFileInfoList>("QFileInfoList");
    qRegisterMetaType<QMap<QString,QImage>>("QMap<QString,QImage>");
}
void mediaUpdateThread::run()
{
        QFileInfoList videoFileList = m_mainWindow->m_wid->m_middleWid->m_rightWid->findVideoFiles(VIDEO_SEARCH_PATH);
    emit m_mainWindow->updateUiByRes(videoFileList);
}

void MainWindow::slot_updateUiByRes(QFileInfoList videoFileList)
{
    m_wid->m_middleWid->m_rightWid->updateResUi(videoFileList);

       if(m_wid->getPlayer()->currentMedia().canonicalUrl().toString()!="")
       {
           m_wid->slot_onCurrentMediaChanged(m_wid->getPlayer()->currentMedia());
       }
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<"key value is:"<<event->key();
    switch(event->key())
    {
    // update musicPlayer and videoPlayer's volume by Key
    case Qt::Key_VolumeDown:

        m_wid->updateVolume(false);;

        QWidget::keyPressEvent(event);
        break;
    case Qt::Key_VolumeUp:

        m_wid->updateVolume(true);

        QWidget::keyPressEvent(event);
        break;
    case Qt::Key_PowerOff:   // when key_power enter

        m_wid->setPlayerPause();

        QTimer::singleShot(100, this, SLOT(slot_standby()));
        break;
    default:
        break;
    }
}
void MainWindow::slot_standby()
{
    system("echo mem > /sys/power/state");
}
