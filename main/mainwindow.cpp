#include "mainwindow.h"
#include "global_value.h"
#include "focusswitchmanager.h"

#include <QDir>
#include <QDirIterator>

MainWindow::MainWindow(QWidget *parent):BaseWindow(parent),
    mediaHasUpdate(false),
    mediaUpdateThread(0)
{
    initData();
    initLayout();
    initConnection();
    slot_updateMedia();
}

void MainWindow::initData()
{
    // Initialize global main class of 'MainWindow' for other widgets invokes.
    mainWindow = this;
    // Start media source update thread.
    // Uevent for usb and inotify for file modify.

    m_notificationReceiver.receive();
}

void MainWindow::initLayout(){
    QVBoxLayout *mainLayout = new QVBoxLayout;

    m_videoWid = new VideoWidgets(this);
    mainLayout->addWidget(m_videoWid);
    mainLayout->setContentsMargins(0,0,0,0);

    setLayout(mainLayout);
}

void MainWindow::initConnection()
{
    // Update media resource when receive signals from 'uevent' or 'inotify'.
    connect(this,SIGNAL(beginUpdateMediaResource()),this,SLOT(slot_setUpdateFlag()));
    connect(this,SIGNAL(updateUiByRes(QFileInfoList)),this,SLOT(slot_updateUiByRes(QFileInfoList)));
    connect(&m_notificationReceiver,SIGNAL(mediaNotification(MediaNotification*)),this,SLOT(slot_setUpdateFlag()));
}

void MainWindow::slot_setUpdateFlag()
{
    /*
     * This operation setted because that inotify event send no more one siganl.
     * So set a 500ms duration to ignore theres no-use siganls.
     * Note: it is expected to optimize.
     */
    if(!mediaHasUpdate){
        mediaHasUpdate = true;
        QTimer::singleShot(500,this,SLOT(slot_updateMedia()));
    }
}

void MainWindow::slot_updateMedia()
{
    qDebug()<<"Update media resource.";
    if (mediaUpdateThread) {
        delete mediaUpdateThread;
        mediaUpdateThread = 0;
    }

    mediaUpdateThread = new MediaUpdateThread(this,this);
    mediaUpdateThread->start();
    mediaHasUpdate = false;
}

void MainWindow::slot_updateUiByRes(QFileInfoList videoFileList)
{
    m_videoWid->updateUiByRes(videoFileList);
}

void MainWindow::disableApplication()
{
    qDebug("disable video application.");
    m_videoWid->setPlayerPause();
    this->setVisible(false);
}

void MainWindow::enableApplication()
{
    qDebug("enable video application.");
    this->setVisible(true);
}

void MainWindow::exitApplication()
{
    if(mediaUpdateThread && mediaUpdateThread->isRunning())
        mediaUpdateThread->waitForThreadFinished();

    this->close();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<"Received keypress event with key value:"<<event->key();
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
        break;
    case Qt::Key_Left:
        qDebug("Key_Left");
        FocusSwitchManager::getInstance()->focusPreviousChild();
        break;
    case Qt::Key_Right:
        qDebug("Key_Right");
        FocusSwitchManager::getInstance()->focusNextChild();
        break;
    case Qt::Key_Up:
        qDebug("Key_Up");
        FocusSwitchManager::getInstance()->focusAboveChild();
        break;
    case Qt::Key_Down:
        qDebug("Key_Down");
        FocusSwitchManager::getInstance()->focusBelowChild();
        break;
    case Qt::Key_Enter:
        qDebug("Key_Enter");
        FocusSwitchManager::getInstance()->clickCurrentWidget();
        break;
    case Qt::Key_Return:
        m_videoWid->slot_exit();
        break;
    default:
        break;
    }

    m_videoWid->showControlView();
    QWidget::keyPressEvent(event);
}

MediaUpdateThread::MediaUpdateThread(QObject *parent,MainWindow *mainWindow):QThread(parent)
{
    m_mainWindow = mainWindow;
    qRegisterMetaType<QFileInfoList>("QFileInfoList");

    m_searchSuffixList.append("mp4");
    m_searchSuffixList.append("avi");
    m_searchSuffixList.append("rm");
    m_searchSuffixList.append("rmvb");
    m_searchSuffixList.append("wmv");
    m_searchSuffixList.append("mkv");
    m_searchSuffixList.append("asf");
    m_searchSuffixList.append("mov");
    m_searchSuffixList.append("ts");
    m_searchSuffixList.append("mpg");
    m_searchSuffixList.append("mpg");
    m_searchSuffixList.append("m2ts");
    m_searchSuffixList.append("trp");
    m_searchSuffixList.append("flv");
    m_searchSuffixList.append("WEBM");
    m_searchSuffixList.append("3GP");
    m_searchSuffixList.append("Vob");
    m_searchSuffixList.append("MPG");
}

void MediaUpdateThread::waitForThreadFinished()
{
    requestInterruption();
    quit();
    wait();
}

QFileInfoList MediaUpdateThread::findVideoFiles(const QString &path)
{
    QFileInfoList videoFiles;

    QDirIterator it(path, QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot);
    while (it.hasNext() && !isInterruptionRequested()){
        QString name = it.next();
        QFileInfo info(name);
        if (info.isDir()){
            videoFiles.append(findVideoFiles(name));
        }
        else{
            for(int i = 0; i < m_searchSuffixList.count(); i++){
                if(info.suffix().compare(m_searchSuffixList.at(i), Qt::CaseInsensitive) == 0){
                    videoFiles.append(info);
                }
            }
        }
    }
    return videoFiles;
}

void MediaUpdateThread::run()
{
    QFileInfoList videoFileList = findVideoFiles(VIDEO_SEARCH_PATH);
    if (!isInterruptionRequested())
        emit m_mainWindow->updateUiByRes(videoFileList);
}
