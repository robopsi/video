#include "videowidgets.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSettings>
#include <QMessageBox>
#include <QDir>
#include <QInputDialog>
#include <QMediaMetaData>

#include "player/videoinfoutil.h"
#include "player/videolist.h"
#include "global_value.h"

VideoWidgets::VideoWidgets(QWidget *parent): BaseWidget(parent)
  ,m_mediaLoadThread(0)
{
    setStyleSheet("QLabel{color:white;}");

    initLayout();
    initPlayerAndConnection();
    readSetting();
    setOriginState();
}

void VideoWidgets::readSetting()
{
    QSettings setting("config.ini",QSettings::IniFormat,0);
    setting.beginGroup("videoConfig");

    // Read play mode.
    int playModeIndex = 0;
    playModeIndex = setting.value("playmode").toInt();
    VideoList *playList = m_fullScreenContrlWid->getListWidget()->getVideoList();
    playList->setPlayMode((PlayMode)playModeIndex);
    m_fullScreenContrlWid->getControlWidget()->updatePlayModeIcon(playList->getCurrentPlayMode());

    // Set volume saved in configration file first.
    QFile *volumnFile;
    QDir settingsDir("/data/");
    if(settingsDir.exists()){
        volumnFile = new QFile("/data/volumn");
    }else{
        volumnFile = new QFile("/etc/volumn");
    }

    volumnFile->open(QFile::ReadOnly | QIODevice::Truncate);
    QByteArray readAll= volumnFile->readAll();
    QString volumnString(readAll);
    long volumnInt= volumnString.toInt();

    m_player->setVolume(volumnInt);
    m_fullScreenContrlWid->getControlWidget()->updateVolumeSliderValue(m_player->volume());
}

void VideoWidgets::setOriginState()
{
    m_fullScreenContrlWid->slot_showControlView(false);
    m_fullScreenContrlWid->getTopWidget()->setPlayingVideoName(str_videoName_default);
    m_fullScreenContrlWid->showPlayList();
    m_fullScreenContrlWid->getListWidget()->setOriginState();
}

void VideoWidgets::initLayout()
{
    QStackedLayout *mlayout = new QStackedLayout;

    m_fullScreenContrlWid = new FullScreenControlWidgets(this);

    // Nomal size state layout.
    QVBoxLayout *narmalStateLayout = new QVBoxLayout;

    m_contentWid = new VideoContentWidgets(this);

    narmalStateLayout->addWidget(m_contentWid);
    narmalStateLayout->setContentsMargins(0,0,0,0);
    narmalStateLayout->setSpacing(0);

    QWidget *normalStateWid = new QWidget(this);
    normalStateWid->setLayout(narmalStateLayout);
    normalStateWid->setWindowFlags(Qt::FramelessWindowHint);
    normalStateWid->setAttribute(Qt::WA_TranslucentBackground, true);

    mlayout->addWidget(m_fullScreenContrlWid);
    mlayout->addWidget(normalStateWid);
    mlayout->setStackingMode(QStackedLayout::StackAll);

    setLayout(mlayout);
}

void VideoWidgets::initPlayerAndConnection()
{
    m_player = m_contentWid->getMediaPlayerFormQml();

    connect(m_player,SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),this,SLOT(slot_onMediaStateChanged(QMediaPlayer::MediaStatus)));
    connect(m_player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(slot_onPlayerStateChanged(QMediaPlayer::State)));
    connect(m_player,SIGNAL(currentMediaChanged(QMediaContent)),this,SLOT(slot_onCurrentMediaChanged(QMediaContent)));
    connect(m_player, SIGNAL(positionChanged(qint64)), this, SLOT(slot_onMediaPositionChanged(qint64)));
    connect(m_player, SIGNAL(durationChanged(qint64)), this, SLOT(slot_onDurationChanged(qint64)));
    connect(m_player,SIGNAL(error(QMediaPlayer::Error)),this,SLOT(slot_onErrorOn(QMediaPlayer::Error)));
    connect(m_player,SIGNAL(metaDataChanged(QString,QVariant)),this,SLOT(slot_onMetaDataChanged(QString,QVariant)));

    // Initialize fullscreen style widgets.
    connect(m_fullScreenContrlWid->getTopWidget(),SIGNAL(returnClick()),this,SLOT(slot_exit()));
    connect(m_fullScreenContrlWid->getListWidget(),SIGNAL(sig_localTableItemClick(int,int)),this,SLOT(slot_onLocalListItemClick(int,int)));
    connect(m_fullScreenContrlWid->getListWidget(),SIGNAL(tableLongPressed(int)),this,SLOT(sloat_tableLongPressed(int)));
    connect(m_fullScreenContrlWid,SIGNAL(sig_sliderPositionChanged(int)),this,SLOT(slot_onSliderPositionChanged(int)));
    connect(m_fullScreenContrlWid->getControlWidget(),SIGNAL(playPauseClick()),this,SLOT(slot_setPlayPause()));
    connect(m_fullScreenContrlWid->getControlWidget(),SIGNAL(nextClick()),this,SLOT(slot_nextVideo()));
    connect(m_fullScreenContrlWid->getControlWidget(),SIGNAL(lastClick()),this,SLOT(slot_lastVideo()));
    connect(m_fullScreenContrlWid->getControlWidget(),SIGNAL(nextLongPressed()),this,SLOT(slot_fastForward()));
    connect(m_fullScreenContrlWid->getControlWidget(),SIGNAL(lastLongPressed()),this,SLOT(slot_fastBackward()));
    connect(m_fullScreenContrlWid->getControlWidget(),SIGNAL(volumeValueChanged(int)),this,SLOT(slot_volumeChanged(int)));
    connect(m_fullScreenContrlWid->getPositionWidget(),SIGNAL(sliderValueChange(int)),this,SLOT(slot_onSliderPositionChanged(int)));
    connect(m_fullScreenContrlWid->getControlWidget(),SIGNAL(playModeClick()),this,SLOT(slot_changePlayMode()));
    connect(m_fullScreenContrlWid->getControlWidget(),SIGNAL(playListClick()),this,SLOT(slot_resizePlayList()));
}

void VideoWidgets::showControlView()
{
    m_fullScreenContrlWid->slot_showControlView(m_player->state() != QMediaPlayer::StoppedState);
}

void VideoWidgets::updateUiByRes(QFileInfoList fileInfoList)
{
    m_fullScreenContrlWid->getListWidget()->updateResUi(fileInfoList);
    if(m_player->currentMedia().canonicalUrl().toString() != ""){
        slot_onCurrentMediaChanged(m_player->currentMedia());
    }
}

void VideoWidgets::slot_onErrorOn(QMediaPlayer::Error)
{
    setOriginState();

    QMessageBox *messageBox = new QMessageBox(QMessageBox::Critical, "Video Error",
                                              "It has encountered an error.", QMessageBox::Yes, mainWindow);
    messageBox->setAttribute(Qt::WA_DeleteOnClose);
    QTimer::singleShot(2500, messageBox, SLOT(close()));
    messageBox->exec();
}

void VideoWidgets::slot_onMetaDataChanged(QString,QVariant)
{
}

void VideoWidgets::slot_onMediaStateChanged(QMediaPlayer::MediaStatus status)
{
    switch(status){
    case QMediaPlayer::EndOfMedia:
        slot_nextVideo(true);
        break;
    default:
        break;
    }
}

void VideoWidgets::slot_onPlayerStateChanged(QMediaPlayer::State state)
{
    switch(state)
    {
    case QMediaPlayer::PlayingState:
        m_fullScreenContrlWid->getControlWidget()->setPlayingStyle();
        break;
    case QMediaPlayer::PausedState:
        m_fullScreenContrlWid->getControlWidget()->setPauseStyle();
        break;
    default:
        m_fullScreenContrlWid->getControlWidget()->setPauseStyle();
        break;
    }
}

void VideoWidgets::slot_onCurrentMediaChanged(QMediaContent content)
{
    m_fullScreenContrlWid->getListWidget()->updatePlayingItemStyle(content);
    m_fullScreenContrlWid->getTopWidget()->setPlayingVideoName(content.canonicalUrl().fileName());
}

void VideoWidgets::slot_onLocalListItemClick(int row, int)
{
    if(m_mediaLoadThread && m_mediaLoadThread->isRunning()){
        return;
    }

    QUrl url= m_fullScreenContrlWid->getListWidget()->getVideoList()->getUrlAt(row);
    if(m_player->isAvailable()){
#ifndef DEVICE_EVB
        // check resolution is correct.
        if(!VideoInfoUtil::isVideoSolutionSuitable(url.path())){
            QMessageBox box(QMessageBox::Critical,"Video Format Error",
                            "video resolution not support.",QMessageBox::Yes);
            m_player->setMedia(NULL);
            setOriginState();
            QTimer::singleShot(2500, &box, SLOT(close()));
            box.exec();
            return;
        }
#endif
        m_fullScreenContrlWid->hidePlayList();
        m_fullScreenContrlWid->slot_showControlView(true);

        if(m_mediaLoadThread){
            delete m_mediaLoadThread;
            m_mediaLoadThread = NULL;
        }

        m_mediaLoadThread = new MediaLoadThread(this,m_player,url);
        m_mediaLoadThread->start();
    }
}

void VideoWidgets::sloat_tableLongPressed(int row){
    QMessageBox box(QMessageBox::Warning,"question","Sure you want to remove the record ?");
    box.setStandardButtons (QMessageBox::Yes|QMessageBox::Cancel);
    if(box.exec() == QMessageBox::Yes){
        m_fullScreenContrlWid->getListWidget()->deleteItem(row);
        m_fullScreenContrlWid->getListWidget()->updatePlayingItemStyle(m_player->currentMedia());
    }
}

void VideoWidgets::slot_setPlayPause()
{
    if(m_player->state()==QMediaPlayer::PlayingState){
        m_player->pause();
    }
    else{
        if(m_player->isAudioAvailable()==true){
            m_player->play();
        }
    }

    m_fullScreenContrlWid->restartHideTimer();
}

void VideoWidgets::slot_nextVideo(bool isEndofMedia)
{
    if(m_mediaLoadThread && m_mediaLoadThread->isRunning()){
        return;
    }

    VideoList *playList = m_fullScreenContrlWid->getListWidget()->getVideoList();
    if(m_player->isAvailable()){
        QUrl url = playList->getNextVideoUrl();
#ifndef DEVICE_EVB
        // check resolution is correct.
        if(!VideoInfoUtil::isVideoSolutionSuitable(url.path())){
            QMessageBox box(QMessageBox::Critical,"Video Format Error",
                            "video resolution not support.",QMessageBox::Yes);
            m_player->setMedia(NULL);
            setOriginState();
            QTimer::singleShot(2500, &box, SLOT(close()));
            box.exec();
            return;
        }
#endif
        m_fullScreenContrlWid->hidePlayList();
        if(!isEndofMedia){
            m_fullScreenContrlWid->slot_showControlView(true);
        }

        if(m_mediaLoadThread){
            delete m_mediaLoadThread;
            m_mediaLoadThread = NULL;
        }

        m_mediaLoadThread = new MediaLoadThread(this,m_player,url);
        m_mediaLoadThread->start();
    }
}

void VideoWidgets::slot_lastVideo()
{
    if(m_mediaLoadThread && m_mediaLoadThread->isRunning()){
        return;
    }

    VideoList *playList = m_fullScreenContrlWid->getListWidget()->getVideoList();
    if(m_player->isAvailable()){
        QUrl url = playList->getPreVideoUrl();
#ifndef DEVICE_EVB
        // check resolution is correct.
        if(!VideoInfoUtil::isVideoSolutionSuitable(url.path())){
            QMessageBox box(QMessageBox::Critical,"Video Format Error",
                            "video resolution not support.",QMessageBox::Yes);
            m_player->setMedia(NULL);
            setOriginState();
            QTimer::singleShot(2500, &box, SLOT(close()));
            box.exec();
            return;
        }
#endif
        m_fullScreenContrlWid->hidePlayList();
        m_fullScreenContrlWid->slot_showControlView(true);

        if(m_mediaLoadThread){
            delete m_mediaLoadThread;
            m_mediaLoadThread = NULL;
        }

        m_mediaLoadThread = new MediaLoadThread(this,m_player,url);
        m_mediaLoadThread->start();
    }
}

void VideoWidgets::slot_fastForward()
{
    if (m_player->state() == QMediaPlayer::PlayingState ||
            m_player->state() == QMediaPlayer::PausedState) {
        m_player->setPosition(m_player->position()+5000);
    }

    m_fullScreenContrlWid->restartHideTimer();
}

void VideoWidgets::slot_fastBackward()
{
    if (m_player->state() == QMediaPlayer::PlayingState ||
            m_player->state() == QMediaPlayer::PausedState) {
        m_player->setPosition(m_player->position()-5000);
    }

    m_fullScreenContrlWid->restartHideTimer();
}

void VideoWidgets::slot_resizePlayList()
{
    m_fullScreenContrlWid->hideOrShowPlayList();
    m_fullScreenContrlWid->restartHideTimer();
}

void VideoWidgets::slot_changePlayMode()
{
    VideoList *playList = m_fullScreenContrlWid->getListWidget()->getVideoList();
    playList->changePlayMode();
    m_fullScreenContrlWid->getControlWidget()->updatePlayModeIcon(playList->getCurrentPlayMode());
    m_fullScreenContrlWid->restartHideTimer();
}

void VideoWidgets::slot_refreshMediaResource()
{
    mainWindow->slot_updateMedia();
}

void VideoWidgets::slot_onDurationChanged(qint64 duration)
{
    m_fullScreenContrlWid->getPositionWidget()->onDurationChanged(duration);
}

void VideoWidgets::slot_onMediaPositionChanged(qint64 position)
{
    m_fullScreenContrlWid->getPositionWidget()->onMediaPositionChanged(position);
}

void VideoWidgets::slot_onSliderPositionChanged(int position)
{
    if(position >= 0){
        m_player->setPosition(position);
        m_fullScreenContrlWid->restartHideTimer();
    }
}

void VideoWidgets::slot_addVideo()
{
    m_fullScreenContrlWid->getListWidget()->addVideo();
}

void VideoWidgets::slot_volumeChanged(int value)
{
    m_player->setVolume(value);
    m_fullScreenContrlWid->getControlWidget()->updateVolumeSliderValue(m_player->volume());
    m_fullScreenContrlWid->restartHideTimer();
    saveVolume(value);
}

void VideoWidgets::saveVolume(int volume){

    QDir  settingsDir("/data/");
    QFile *volumeFile;
    if(settingsDir.exists()){
        volumeFile = new QFile("/data/volumn");
    }else{
        volumeFile = new QFile("/etc/volumn");
    }

    if (volumeFile->open(QFile::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(volumeFile);
        out <<volume;
        volumeFile->close();
    }
    delete volumeFile;
}

void VideoWidgets::savaSetting()
{
    QSettings setting("config.ini",QSettings::IniFormat,0);
    setting.beginGroup("videoConfig");
    setting.setValue("playmode",(int)m_fullScreenContrlWid->getListWidget()->getVideoList()->getPlayMode());
    setting.endGroup();

    m_player->pause();
}

void VideoWidgets::slot_exit()
{
    // Release media source and quit application.
    m_player->stop();
    m_player->setMedia(NULL);

    savaSetting();
    mainWindow->exitApplication();
}

void VideoWidgets::updateVolume(bool volumeAdd)
{
    if(volumeAdd){
        if(m_player->volume()<95){
            m_player->setVolume(m_player->volume()+5);
        }else{
            m_player->setVolume(100);
        }
    }else{
        if(m_player->volume()>5){
            m_player->setVolume(m_player->volume()-5);
        }else{
            m_player->setVolume(0);
        }
    }
    m_fullScreenContrlWid->getControlWidget()->updateVolumeSliderValue(m_player->volume());
    saveVolume(m_player->volume());
}

VideoWidgets::~VideoWidgets()
{
}

MediaLoadThread::MediaLoadThread(QObject *parent, QMediaPlayer *player, QUrl url):QThread(parent)
{
    this->player = player;
    this->loadUrl = url;
}

MediaLoadThread::~MediaLoadThread()
{
    requestInterruption();
    quit();
    wait();
}

void MediaLoadThread::run()
{
    player->setMedia(loadUrl);
    player->play();
}
