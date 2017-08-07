#include "videowidgets.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSettings>
#include <QMessageBox>

#include "player/videolist.h"
#include "global_value.h"

VideoWidgets::VideoWidgets(QWidget *parent): BaseWidget(parent)
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
    // Get volume value.
    int vol=setting.value("volume").toInt();
    if(vol==0)
        vol=80;
    m_player->setVolume(vol);
    m_bottomWid->m_VolWidget->m_slider_vol->setValue(m_player->volume());
    setting.endGroup();
}

void VideoWidgets::setOriginState()
{
    m_topWid->setPlayingVideoName(str_videoName_default);
    m_middleWid->m_contentWid->removePositionWidget();
    m_middleWid->m_listWid->setOriginState();
}


void VideoWidgets::initLayout()
{
    QVBoxLayout *vmainlyout = new QVBoxLayout;

    m_topWid = new VideoTopWidgets(this);
    m_middleWid = new VideoMiddleWidgets(this);
    m_bottomWid = new VideoBottomWidgets(this);
    m_bottomWid->setVisible(false);

    vmainlyout->addWidget(m_topWid);
    vmainlyout->addWidget(m_middleWid);
    vmainlyout->addWidget(m_bottomWid);
    vmainlyout->setContentsMargins(0,0,0,0);
    vmainlyout->setSpacing(0);

    setLayout(vmainlyout);
}

void VideoWidgets::initPlayerAndConnection()
{
    m_player = m_middleWid->m_contentWid->getMediaPlayerFormQml();

    connect(m_player,SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),this,SLOT(slot_onMediaStateChanged(QMediaPlayer::MediaStatus)));
    connect(m_player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(slot_onPlayerStateChanged(QMediaPlayer::State)));
    connect(m_player,SIGNAL(currentMediaChanged(QMediaContent)),this,SLOT(slot_onCurrentMediaChanged(QMediaContent)));
    connect(m_player, SIGNAL(positionChanged(qint64)), this, SLOT(slot_onMediaPositionChanged(qint64)));
    connect(m_player, SIGNAL(durationChanged(qint64)), this, SLOT(slot_onDurationChanged(qint64)));

    connect(m_player,SIGNAL(error(QMediaPlayer::Error)),this,SLOT(slot_onErrorOn(QMediaPlayer::Error)));

    connect(m_middleWid->m_listWid->m_localTable,SIGNAL(cellClicked(int,int)),this,SLOT(slot_onLocalListItemDoubleClick(int,int)));
    connect(m_bottomWid->m_btnPlayPause,SIGNAL(clicked(bool)),this,SLOT(slot_setPlayPause()));
    connect(m_bottomWid->m_btnNext,SIGNAL(clicked(bool)),this,SLOT(slot_nextVideo()));
    connect(m_bottomWid->m_btnLast,SIGNAL(clicked(bool)),this,SLOT(slot_lastVideo()));
    connect(m_bottomWid->m_btnOpenFile,SIGNAL(clicked(bool)),this,SLOT(slot_addVideo()));
    connect(m_bottomWid->m_VolWidget,SIGNAL(sig_valueChanged(int)),this,SLOT(slot_volumeChanged(int)));

    connect(m_middleWid->m_contentWid->getSurfaceWid(),SIGNAL(contentOneClick()),this,SLOT(slot_setPlayPause()));
    connect(m_middleWid->m_contentWid->getSurfaceWid(),SIGNAL(contentDoubleClick()),this,SLOT(slot_onContentDoubleClick()));
    connect(m_middleWid->m_contentWid,SIGNAL(sig_sliderPositionChanged(int)),this,SLOT(slot_onSliderPositionChanged(int)));

    connect(m_topWid->m_btnreturn,SIGNAL(clicked(bool)),this,SLOT(slot_returnClick()));
}

void VideoWidgets::slot_onErrorOn(QMediaPlayer::Error)
{
    m_player->setMedia(NULL);
    setOriginState();
}


void VideoWidgets::slot_onMediaStateChanged(QMediaPlayer::MediaStatus status)
{
    switch(status){
    case QMediaPlayer::EndOfMedia:
        slot_nextVideo();
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
        m_bottomWid->setPlayingStyle();
        break;
    case QMediaPlayer::PausedState:
        m_bottomWid->setPauseStyle();
        break;
    default:
        m_bottomWid->setPauseStyle();
        break;
    }
}

void VideoWidgets::slot_onCurrentMediaChanged(QMediaContent content)
{
    m_middleWid->m_listWid->updatePlayingItemStyle(content);
    m_topWid->setPlayingVideoName(m_middleWid->m_listWid->getCurrentVideoName());
}

void VideoWidgets::slot_denyPlay()
{
    m_player->setMedia(m_onPlayUrl);
    m_player->play();
}

void VideoWidgets::slot_onLocalListItemDoubleClick(int row, int)
{
#ifdef DEVICE_EVB
    m_player->stop();
    QUrl url= m_middleWid->m_listWid->getVideoList()->getUrlAt(row);
    if(m_player->isAvailable()){
        m_player->setMedia(url);
        m_player->play();
    }
#else
    m_player->stop();
    m_player->setMedia(NULL);
    QUrl url= m_middleWid->m_listWid->getVideoList()->getUrlAt(row);
    m_onPlayUrl = url;
    if(m_player->isAvailable()){
        QTimer::singleShot(200,this,SLOT(slot_denyPlay()));
    }
#endif
    m_middleWid->slot_playMode();
    m_bottomWid->setVisible(true);

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
}

void VideoWidgets::slot_nextVideo()
{
#ifdef DEVICE_EVB
    m_player->stop();
    VideoList *m_playList = m_middleWid->m_listWid->getVideoList();
    if(m_player->isAvailable()){
        m_player->setMedia(m_playList->getNextVideoUrl());
        m_player->play();
    }
#else
    m_player->stop();
    m_player->setMedia(NULL);
    VideoList *m_playList = m_middleWid->m_listWid->getVideoList();
    m_onPlayUrl = m_playList->getNextVideoUrl();
    if(m_player->isAvailable()){
        QTimer::singleShot(200,this,SLOT(slot_denyPlay()));
    }
#endif
}

void VideoWidgets::slot_lastVideo()
{
#ifdef DEVICE_EVB
    m_player->stop();
    VideoList *m_playList = m_middleWid->m_listWid->getVideoList();
    if(m_player->isAvailable()){
        m_player->setMedia(m_playList->getPreVideoUrl());
        m_player->play();
    }
#else
    m_player->stop();
    m_player->setMedia(NULL);
    VideoList *m_playList = m_middleWid->m_listWid->getVideoList();
    m_onPlayUrl = m_playList->getPreVideoUrl();
    if(m_player->isAvailable()){
        QTimer::singleShot(200,this,SLOT(slot_denyPlay()));
    }
#endif
}

void VideoWidgets::slot_onContentDoubleClick()
{
    if(m_player->state()==QMediaPlayer::PlayingState||m_player->state()==QMediaPlayer::PausedState){
        //        if(m_middleWid->m_contentWid->getContentWidget()->isFullScreen()){
        //            m_middleWid->m_contentWid->getContentWidget()->setFullScreen(false);
        //        }else{
        //            m_middleWid->m_contentWid->getContentWidget()->setFullScreen(true);
        //        }
    }
}

void VideoWidgets::slot_onDurationChanged(qint64 duration)
{
    m_middleWid->m_contentWid->addPositionWidget();
    m_middleWid->m_contentWid->onDurationChanged(duration);
}

void VideoWidgets::slot_onMediaPositionChanged(qint64 position)
{
    m_middleWid->m_contentWid->onMediaPositionChanged(position);
}

void VideoWidgets::slot_onSliderPositionChanged(int position)
{
    m_player->setPosition(position);
}

void VideoWidgets::slot_addVideo()
{
    m_middleWid->m_listWid->addVideo();
}

void VideoWidgets::slot_volumeChanged(int value)
{
    m_player->setVolume(value);
}

void VideoWidgets::savaSetting()
{
    QSettings setting("config.ini",QSettings::IniFormat,0);
    setting.beginGroup("videoConfig");
    setting.setValue("volume",m_player->volume());
    setting.endGroup();

    m_player->pause();
}

void VideoWidgets::slot_returnClick()
{
    if(m_middleWid->isCurrentPlayMode()){
        m_player->stop();
        m_player->setMedia(NULL);
        setOriginState();
        m_middleWid->slot_listMode();
        m_bottomWid->setVisible(false);
    }else{
        // Release media source and quit application.
        m_player->stop();
        m_player->setMedia(NULL);
        setOriginState();
        mainWindow->slot_appQuit();
    }
}

void VideoWidgets::updateVolume(bool volumeAdd)
{
    if(volumeAdd){
        if(m_player->volume()<95)
        {
            m_player->setVolume(m_player->volume()+5);
        }
        else
        {
            m_player->setVolume(100);
        }
    }else{
        if(m_player->volume()>5)
        {
            m_player->setVolume(m_player->volume()-5);
        }
        else
        {
            m_player->setVolume(0);
        }
    }
    m_bottomWid->m_VolWidget->m_slider_vol->setValue(m_player->volume());
}

VideoWidgets::~VideoWidgets()
{
    delete m_topWid;
    delete m_middleWid;
    delete m_bottomWid;
}


