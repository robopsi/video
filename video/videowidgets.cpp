#include "videowidgets.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSettings>
#include <QMessageBox>
#include <QDir>
#include <QInputDialog>

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
    m_bottomWid->m_VolWidget->m_slider_vol->setValue(m_player->volume());
}

void VideoWidgets::setOriginState()
{
    m_topWid->setPlayingVideoName(str_videoName_default);
    m_middleWid->m_contentWid->removePositionWidget();
    m_middleWid->m_listWid->setOriginState();
}

void VideoWidgets::initLayout()
{
    m_stackedLayout = new QStackedLayout;

    m_fullScreenContrlWid = new FullScreenControlWidgets(this);

    // Nomal size state layout.
    QVBoxLayout *narmalStateLayout = new QVBoxLayout;

    m_topWid = new VideoTopWidgets(this);
    m_middleWid = new VideoMiddleWidgets(this);
    m_bottomWid = new VideoBottomWidgets(this);

    narmalStateLayout->addWidget(m_topWid);
    narmalStateLayout->addWidget(m_middleWid);
    narmalStateLayout->addWidget(m_bottomWid);
    narmalStateLayout->setContentsMargins(0,0,0,0);
    narmalStateLayout->setSpacing(0);

    QWidget *normalStateWid = new QWidget(this);
    normalStateWid->setLayout(narmalStateLayout);
    normalStateWid->setWindowFlags(Qt::FramelessWindowHint);
    normalStateWid->setAttribute(Qt::WA_TranslucentBackground, true);

    m_stackedLayout->addWidget(normalStateWid);
    m_stackedLayout->addWidget(m_fullScreenContrlWid);
    m_stackedLayout->setStackingMode(QStackedLayout::StackAll);
    m_stackedLayout->setCurrentIndex(0);

    setLayout(m_stackedLayout);
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
    connect(m_bottomWid->m_btnChangeSize,SIGNAL(clicked(bool)),this,SLOT(slot_fullScreenStyle()));
    connect(m_bottomWid->m_btnRefresh,SIGNAL(clicked(bool)),this,SLOT(slot_refreshMediaResource()));

    connect(m_middleWid->m_contentWid->getSurfaceWid(),SIGNAL(contentOneClick()),this,SLOT(slot_fullScreenStyle()));
//    connect(m_middleWid->m_contentWid->getSurfaceWid(),SIGNAL(contentDoubleClick()),this,SLOT(slot_fullScreenStyle()));
    connect(m_middleWid->m_contentWid,SIGNAL(sig_sliderPositionChanged(int)),this,SLOT(slot_onSliderPositionChanged(int)));

    connect(m_topWid->m_btnreturn,SIGNAL(clicked(bool)),this,SLOT(slot_returnClick()));

    // Initialize fullscreen style widgets.
    connect(m_fullScreenContrlWid->getControlWidget()->m_btnPlayPause,SIGNAL(clicked(bool)),this,SLOT(slot_setPlayPause()));
    connect(m_fullScreenContrlWid->getControlWidget()->m_btnNext,SIGNAL(clicked(bool)),this,SLOT(slot_nextVideo()));
    connect(m_fullScreenContrlWid->getControlWidget()->m_btnLast,SIGNAL(clicked(bool)),this,SLOT(slot_lastVideo()));
    connect(m_fullScreenContrlWid->getControlWidget()->m_VolWidget,SIGNAL(sig_valueChanged(int)),this,SLOT(slot_volumeChanged(int)));
    connect(m_fullScreenContrlWid->getControlWidget()->m_btnChangeSize,SIGNAL(clicked(bool)),this,SLOT(slot_normalSizeStyle()));

    connect(m_fullScreenContrlWid->getPositionWidget()->getSlider(),SIGNAL(sig_sliderPositionChanged(int)),this,SLOT(slot_onSliderPositionChanged(int)));

}

void VideoWidgets::slot_onErrorOn(QMediaPlayer::Error)
{
    m_player->setMedia(NULL);
    if(QMessageBox::Yes == QMessageBox::critical(mainWindow,"Video Error","It has encountered an error.",
                                                 QMessageBox::Yes | QMessageBox::Yes))
    {
        slot_normalSizeStyle();
    }
    setOriginState();
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
        m_bottomWid->setPlayingStyle();
        m_fullScreenContrlWid->getControlWidget()->setPlayingStyle();
        break;
    case QMediaPlayer::PausedState:
        m_bottomWid->setPauseStyle();
        m_fullScreenContrlWid->getControlWidget()->setPauseStyle();
        break;
    default:
        m_bottomWid->setPauseStyle();
        m_fullScreenContrlWid->getControlWidget()->setPauseStyle();
        break;
    }
}

void VideoWidgets::slot_onCurrentMediaChanged(QMediaContent content)
{
    m_middleWid->m_listWid->updatePlayingItemStyle(content);
    m_topWid->setPlayingVideoName(m_middleWid->m_listWid->getCurrentVideoName());
}

void VideoWidgets::slot_onLocalListItemDoubleClick(int row, int)
{
    m_player->stop();
    QUrl url= m_middleWid->m_listWid->getVideoList()->getUrlAt(row);
    if(m_player->isAvailable()){
        m_player->setMedia(url);
        m_player->play();
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
    if(m_middleWid->m_contentWid->getCurrentSizeState() == VideoContentWidgets::FullScreenSize){
        m_fullScreenContrlWid->slot_showControlView();
    }
}

void VideoWidgets::slot_nextVideo(bool isEndofMedia)
{
    m_player->stop();
    VideoList *m_playList = m_middleWid->m_listWid->getVideoList();
    if(m_player->isAvailable()){
        m_player->setMedia(m_playList->getNextVideoUrl());
        m_player->play();
    }
    if(m_middleWid->m_contentWid->getCurrentSizeState() == VideoContentWidgets::FullScreenSize && !isEndofMedia){
        m_fullScreenContrlWid->slot_showControlView();
    }
}

void VideoWidgets::slot_lastVideo()
{
    m_player->stop();
    VideoList *m_playList = m_middleWid->m_listWid->getVideoList();
    if(m_player->isAvailable()){
        m_player->setMedia(m_playList->getPreVideoUrl());
        m_player->play();
    }
    if(m_middleWid->m_contentWid->getCurrentSizeState() == VideoContentWidgets::FullScreenSize){
        m_fullScreenContrlWid->slot_showControlView();
    }
}

void VideoWidgets::slot_fullScreenStyle()
{
    if(m_player->state()==QMediaPlayer::PlayingState||m_player->state()==QMediaPlayer::PausedState){
        if(m_middleWid->m_contentWid->getCurrentSizeState() == VideoContentWidgets::NormalSize){
            // Save normal size of each moudle.
            top_normal_height = m_topWid->height();
            bottom_normal_height = m_bottomWid->height();
            middle_list_width = m_middleWid->m_listWid->width();
            // Change each module's size to fix video surface.
            m_topWid->setFixedHeight(0);
            m_bottomWid->setFixedHeight(0);
            m_middleWid->m_listWid->setFixedWidth(0);

            m_fullScreenContrlWid->slot_hideControlView();
            m_stackedLayout->setCurrentIndex(1);

            m_middleWid->m_contentWid->fullScreenStyle();
        }
    }
    if(m_player->state() == QMediaPlayer::PausedState){
        // Must be optimized ==========
        m_player->play();
        m_player->pause();
    }
}

void VideoWidgets::slot_normalSizeStyle()
{
    if(m_middleWid->m_contentWid->getCurrentSizeState() == VideoContentWidgets::FullScreenSize){
        m_topWid->setFixedHeight(top_normal_height);
        m_bottomWid->setFixedHeight(bottom_normal_height);
        m_middleWid->m_listWid->setFixedWidth(middle_list_width);

        m_stackedLayout->setCurrentIndex(0);

        m_middleWid->m_contentWid->normalSizeStyle();
    }
    if(m_player->state() == QMediaPlayer::PausedState){
        // Must be optimized ==========
        m_player->play();
        m_player->pause();
    }
}

void VideoWidgets::slot_refreshMediaResource()
{
    bool isConfirm;
    QString appendSuffix = QInputDialog::getText(mainWindow,"Add Refresh Suffix",
                                                 "Please input extra file suffix",
                                                 QLineEdit::Normal,
                                                 "",
                                                 &isConfirm);
    if(isConfirm){
        if(!appendSuffix.isEmpty()){
            m_middleWid->m_listWid->addRefreshSuffix(appendSuffix);
        }
        mainWindow->slot_updateMedia();
    }
}

void VideoWidgets::slot_onDurationChanged(qint64 duration)
{
    m_middleWid->m_contentWid->onDurationChanged(duration);
    m_fullScreenContrlWid->getPositionWidget()->onDurationChanged(duration);
}

void VideoWidgets::slot_onMediaPositionChanged(qint64 position)
{
    m_middleWid->m_contentWid->onMediaPositionChanged(position);
    m_fullScreenContrlWid->getPositionWidget()->onMediaPositionChanged(position);
}

void VideoWidgets::slot_onSliderPositionChanged(int position)
{
    m_player->setPosition(position);
    if(m_middleWid->m_contentWid->getCurrentSizeState() == VideoContentWidgets::FullScreenSize){
        m_fullScreenContrlWid->slot_showControlView();
    }
}

void VideoWidgets::slot_addVideo()
{
    m_middleWid->m_listWid->addVideo();
}

void VideoWidgets::slot_volumeChanged(int value)
{
    m_player->setVolume(value);
    if(m_middleWid->m_contentWid->getCurrentSizeState() == VideoContentWidgets::FullScreenSize){
        m_fullScreenContrlWid->slot_showControlView();
    }
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
    // Release media source and quit application.
    m_player->stop();
    m_player->setMedia(NULL);
    setOriginState();
    mainWindow->slot_appQuit();

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
    m_bottomWid->m_VolWidget->m_slider_vol->setValue(m_player->volume());
}

VideoWidgets::~VideoWidgets()
{
}
