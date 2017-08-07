#include "mainwindow.h"
#include "global_value.h"

MainWindow::MainWindow(QWidget *parent) :BaseWindow(parent),mediaHasUpdate(false)
{
    // Init global main class of 'MainWindow' for other widgets invokes.
    mainWindow = this;

    initLayout();
    initConnection();
    slot_updateMedia();
}

MainWindow::~MainWindow()
{
    delete m_videoWid;
}

void MainWindow::initLayout(){
    QVBoxLayout *mainLayout = new QVBoxLayout;
    m_videoWid = new VideoWidgets(this);
    mainLayout->addWidget(m_videoWid);
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);
}

void MainWindow::initConnection()
{
    connect(m_videoWid->m_topWid->m_btnmini,SIGNAL(clicked(bool)),this,SLOT(showMinimized()));
    connect(m_videoWid->m_topWid->m_btnexit,SIGNAL(clicked(bool)),this,SLOT(slot_appQuit()));
//    connect(m_videoWid->m_topWid->m_btnreturn,SIGNAL(clicked(bool)),this,SLOT(slot_appQuit()));

    // Update media resource when receive signals from 'uevent' or 'inotify'.
    connect(this,SIGNAL(beginUpdateMediaResource()),this,SLOT(slot_setUpdateFlag()));
    connect(this,SIGNAL(updateUiByRes(QFileInfoList)),this,SLOT(slot_updateUiByRes(QFileInfoList)));
}

void MainWindow::slot_setUpdateFlag()
{
    /*
     * This operation setted because that inotify event send no more one siganl.
     * So set a 2 seconds duration to ignore theres no-use siganls.
     * Note: it is expected to optimize.
     */
    if(!mediaHasUpdate)
    {
        mediaHasUpdate = true;
        QTimer::singleShot(2000,this,SLOT(slot_updateMedia()));
    }
}

void MainWindow::slot_updateMedia()
{
    qDebug()<<"Carmachine: Update media resource.";
    mediaUpdateThread *thread = new mediaUpdateThread(this,this);
    thread->start();
    mediaHasUpdate =false;
}

void MainWindow::slot_updateUiByRes(QFileInfoList videoFileList)
{
    m_videoWid->m_middleWid->m_listWid->updateResUi(videoFileList);
    if(m_videoWid->getPlayer()->currentMedia().canonicalUrl().toString()!="")
    {
        m_videoWid->slot_onCurrentMediaChanged(m_videoWid->getPlayer()->currentMedia());
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<"Received ketpress event with key value:"<<event->key();
    switch(event->key())
    {
    case Qt::Key_VolumeDown:
        m_videoWid->updateVolume(false);;
        break;
    case Qt::Key_VolumeUp:
        m_videoWid->updateVolume(true);
        break;
    case Qt::Key_PowerOff:
        // when key_power enter
        m_videoWid->setPlayerPause();
        QTimer::singleShot(100, this, SLOT(slot_standby()));
        break;
    default:
        break;
    }
    QWidget::keyPressEvent(event);
}

void MainWindow::slot_standby()
{
    system("echo mem > /sys/power/state");
}

void MainWindow::slot_appQuit()
{
    m_videoWid->savaSetting();
    this->close();
}

mediaUpdateThread::mediaUpdateThread(QObject *parent,MainWindow *mainWindow):QThread(parent)
{
    m_mainWindow = mainWindow;
    qRegisterMetaType<QFileInfoList>("QFileInfoList");
    qRegisterMetaType<QMap<QString,QImage>>("QMap<QString,QImage>");
}
void mediaUpdateThread::run()
{
    QFileInfoList videoFileList = m_mainWindow->getVideoWidget()->m_middleWid->m_listWid->findVideoFiles(VIDEO_SEARCH_PATH);
    emit m_mainWindow->updateUiByRes(videoFileList);
}
