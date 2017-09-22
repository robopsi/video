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
    QSettings setting("config.ini",QSettings::IniFormat,0);
    setting.beginGroup("videoConfig");

    // Read play mode.
    int playModeIndex = 0;
    playModeIndex = setting.value("playmode").toInt();
    VideoList *playList = m_middleWid->getListWidget()->getVideoList();
    playList->setPlayMode((PlayMode)playModeIndex);
    m_bottomWid->updatePlayModeIcon(playList->getCurrentPlayMode());

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
    m_bottomWid->updateVolumeSliderValue(m_player->volume());
    m_fullScreenContrlWid->getControlWidget()->updateVolumeSliderValue(m_player->volume());
}

void VideoWidgets::setOriginState()
{
    m_topWid->setPlayingVideoName(str_videoName_default);
    m_middleWid->getContentWidget()->removePositionWidget();
    m_middleWid->getListWidget()->setOriginState();
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
    m_player = m_middleWid->getContentWidget()->getMediaPlayerFormQml();

    connect(m_player,SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),this,SLOT(slot_onMediaStateChanged(QMediaPlayer::MediaStatus)));
    connect(m_player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(slot_onPlayerStateChanged(QMediaPlayer::State)));
    connect(m_player,SIGNAL(currentMediaChanged(QMediaContent)),this,SLOT(slot_onCurrentMediaChanged(QMediaContent)));
    connect(m_player, SIGNAL(positionChanged(qint64)), this, SLOT(slot_onMediaPositionChanged(qint64)));
    connect(m_player, SIGNAL(durationChanged(qint64)), this, SLOT(slot_onDurationChanged(qint64)));
    connect(m_player,SIGNAL(error(QMediaPlayer::Error)),this,SLOT(slot_onErrorOn(QMediaPlayer::Error)));

    connect(m_topWid,SIGNAL(returnClick()),this,SLOT(slot_exit()));

    connect(m_bottomWid,SIGNAL(playPauseClick()),this,SLOT(slot_setPlayPause()));
    connect(m_bottomWid,SIGNAL(nextClick()),this,SLOT(slot_nextVideo()));
    connect(m_bottomWid,SIGNAL(nextLongPressed()),this,SLOT(slot_fastForward()));
    connect(m_bottomWid,SIGNAL(lastClick()),this,SLOT(slot_lastVideo()));
    connect(m_bottomWid,SIGNAL(lastLongPressed()),this,SLOT(slot_fastBackward()));
    connect(m_bottomWid,SIGNAL(openFileClick()),this,SLOT(slot_addVideo()));
    connect(m_bottomWid,SIGNAL(volumeValueChanged(int)),this,SLOT(slot_volumeChanged(int)));
    connect(m_bottomWid,SIGNAL(changeSizeClick()),this,SLOT(slot_fullScreenStyle()));
    connect(m_bottomWid,SIGNAL(refreshClick()),this,SLOT(slot_refreshMediaResource()));
    connect(m_bottomWid,SIGNAL(playModeClick()),this,SLOT(slot_changePlayMode()));

    connect(m_middleWid->getListWidget(),SIGNAL(sig_localTableItemClick(int,int)),this,SLOT(slot_onLocalListItemClick(int,int)));
    connect(m_middleWid->getListWidget(),SIGNAL(tableLongPressed(int)),this,SLOT(sloat_tableLongPressed(int)));
    connect(m_middleWid->getContentWidget(),SIGNAL(surfaceOneClick()),this,SLOT(slot_fullScreenStyle()));
    connect(m_middleWid->getContentWidget(),SIGNAL(surfaceDoubleClick()),this,SLOT(slot_fullScreenStyle()));
    connect(m_middleWid->getContentWidget(),SIGNAL(sig_sliderPositionChanged(int)),this,SLOT(slot_onSliderPositionChanged(int)));

    // Initialize fullscreen style widgets.
    connect(m_fullScreenContrlWid->getControlWidget(),SIGNAL(playPauseClick()),this,SLOT(slot_setPlayPause()));
    connect(m_fullScreenContrlWid->getControlWidget(),SIGNAL(nextClick()),this,SLOT(slot_nextVideo()));
    connect(m_fullScreenContrlWid->getControlWidget(),SIGNAL(lastClick()),this,SLOT(slot_lastVideo()));
    connect(m_fullScreenContrlWid->getControlWidget(),SIGNAL(nextLongPressed()),this,SLOT(slot_fastForward()));
    connect(m_fullScreenContrlWid->getControlWidget(),SIGNAL(lastLongPressed()),this,SLOT(slot_fastBackward()));
    connect(m_fullScreenContrlWid->getControlWidget(),SIGNAL(volumeValueChanged(int)),this,SLOT(slot_volumeChanged(int)));
    connect(m_fullScreenContrlWid->getControlWidget(),SIGNAL(changeSizeClick()),this,SLOT(slot_normalSizeStyle()));
    connect(m_fullScreenContrlWid->getPositionWidget(),SIGNAL(sliderValueChange(int)),this,SLOT(slot_onSliderPositionChanged(int)));
    connect(m_fullScreenContrlWid->getControlWidget(),SIGNAL(playModeClick()),this,SLOT(slot_changePlayMode()));
}

QFileInfoList VideoWidgets::findAllVideoFiles(const QString &serachPath)
{
    return m_middleWid->getListWidget()->findVideoFiles(serachPath);
}

void VideoWidgets::updateUiByRes(QFileInfoList fileInfoList)
{
    m_middleWid->getListWidget()->updateResUi(fileInfoList);
    if(m_player->currentMedia().canonicalUrl().toString()!="")
    {
        slot_onCurrentMediaChanged(m_player->currentMedia());
    }
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
    m_middleWid->getListWidget()->updatePlayingItemStyle(content);
    m_topWid->setPlayingVideoName(content.canonicalUrl().fileName());
}

void VideoWidgets::slot_onLocalListItemClick(int row, int)
{
    m_player->stop();
    QUrl url= m_middleWid->getListWidget()->getVideoList()->getUrlAt(row);
    if(m_player->isAvailable()){
        m_player->setMedia(url);
        m_player->play();
    }
}

void VideoWidgets::sloat_tableLongPressed(int row){
    QMessageBox box(QMessageBox::Warning,"question","Sure you want to remove the record ?");
    box.setStandardButtons (QMessageBox::Yes|QMessageBox::Cancel);
    if(box.exec() == QMessageBox::Yes)
    {
        slot_deleteTableItem(row);
    }
}

void VideoWidgets::slot_deleteTableItem(int row)
{
    m_middleWid->getListWidget()->deleteItem(row);
    m_middleWid->getListWidget()->updatePlayingItemStyle(m_player->currentMedia());
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
    if(m_middleWid->getContentWidget()->getCurrentSizeState() == FullScreenSize){
        m_fullScreenContrlWid->slot_showControlView();
    }
}

void VideoWidgets::slot_nextVideo(bool isEndofMedia)
{
    m_player->stop();
    VideoList *playList = m_middleWid->getListWidget()->getVideoList();
    if(m_player->isAvailable()){
        m_player->setMedia(playList->getNextVideoUrl());
        m_player->play();
    }
    if(m_middleWid->getContentWidget()->getCurrentSizeState() == FullScreenSize && !isEndofMedia){
        m_fullScreenContrlWid->slot_showControlView();
    }
}

void VideoWidgets::slot_lastVideo()
{
    m_player->stop();
    VideoList *playList = m_middleWid->getListWidget()->getVideoList();
    if(m_player->isAvailable()){
        m_player->setMedia(playList->getPreVideoUrl());
        m_player->play();
    }
    if(m_middleWid->getContentWidget()->getCurrentSizeState() == FullScreenSize){
        m_fullScreenContrlWid->slot_showControlView();
    }
}

void VideoWidgets::slot_fastForward()
{
    if(m_player->state()==QMediaPlayer::PlayingState||
            m_player->state()==QMediaPlayer::PausedState){
        m_player->setPosition(m_player->position()+5000);
    }
    if(m_middleWid->getContentWidget()->getCurrentSizeState() == FullScreenSize){
        m_fullScreenContrlWid->slot_showControlView();
    }
}

void VideoWidgets::slot_fastBackward()
{
    if(m_player->state()==QMediaPlayer::PlayingState||
            m_player->state()==QMediaPlayer::PausedState){
        m_player->setPosition(m_player->position()-5000);
    }
    if(m_middleWid->getContentWidget()->getCurrentSizeState() == FullScreenSize){
        m_fullScreenContrlWid->slot_showControlView();
    }
}

void VideoWidgets::slot_fullScreenStyle()
{
    if(m_player->state()==QMediaPlayer::PlayingState||m_player->state()==QMediaPlayer::PausedState){
        if(m_middleWid->getContentWidget()->getCurrentSizeState() == NormalSize){
            // Save normal size of each moudle.
            top_normal_height = m_topWid->height();
            bottom_normal_height = m_bottomWid->height();
            middle_list_width = m_middleWid->getListWidget()->width();
            // Change each module's size to fix video surface.
            m_topWid->setFixedHeight(0);
            m_bottomWid->setFixedHeight(0);
            m_middleWid->getListWidget()->setFixedWidth(0);

            m_fullScreenContrlWid->slot_hideControlView();
            m_middleWid->getContentWidget()->fullScreenStyle();
            // Update playMode icon by 'videoList'
            m_fullScreenContrlWid->getControlWidget()->updatePlayModeIcon(m_middleWid->getListWidget()->getVideoList()->getCurrentPlayMode());
            m_stackedLayout->setCurrentIndex(1);
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
    if(m_middleWid->getContentWidget()->getCurrentSizeState() == FullScreenSize){
        m_topWid->setFixedHeight(top_normal_height);
        m_bottomWid->setFixedHeight(bottom_normal_height);
        m_middleWid->getListWidget()->setFixedWidth(middle_list_width);

        m_middleWid->getContentWidget()->normalSizeStyle();
        // Update playMode icon by 'videoList'
        m_bottomWid->updatePlayModeIcon(m_middleWid->getListWidget()->getVideoList()->getCurrentPlayMode());
        m_stackedLayout->setCurrentIndex(0);
    }
    if(m_player->state() == QMediaPlayer::PausedState){
        // Must be optimized ==========
        m_player->play();
        m_player->pause();
    }
}

void VideoWidgets::slot_changePlayMode()
{
    VideoList *playList = m_middleWid->getListWidget()->getVideoList();
    playList->changePlayMode();
    m_bottomWid->updatePlayModeIcon(playList->getCurrentPlayMode());
    m_fullScreenContrlWid->getControlWidget()->updatePlayModeIcon(playList->getCurrentPlayMode());
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
            if(appendSuffix.contains(".")){
                QFileInfo fileInfo(appendSuffix);
                appendSuffix=fileInfo.suffix();
            }

            m_middleWid->getListWidget()->addRefreshSuffix(appendSuffix);
        }
        mainWindow->slot_updateMedia();
    }
}

void VideoWidgets::slot_onDurationChanged(qint64 duration)
{
    m_middleWid->getContentWidget()->onDurationChanged(duration);
    m_fullScreenContrlWid->getPositionWidget()->onDurationChanged(duration);
}

void VideoWidgets::slot_onMediaPositionChanged(qint64 position)
{
    m_middleWid->getContentWidget()->onMediaPositionChanged(position);
    m_fullScreenContrlWid->getPositionWidget()->onMediaPositionChanged(position);
}

void VideoWidgets::slot_onSliderPositionChanged(int position)
{
    if(position >= 0){
        m_player->setPosition(position);
        if(m_middleWid->getContentWidget()->getCurrentSizeState() == FullScreenSize){
            m_fullScreenContrlWid->slot_showControlView();
        }
    }
    if(m_middleWid->getContentWidget()->getCurrentSizeState() == FullScreenSize){
        m_fullScreenContrlWid->slot_showControlView();
    }
}

void VideoWidgets::slot_addVideo()
{
    m_middleWid->getListWidget()->addVideo();
}

void VideoWidgets::slot_volumeChanged(int value)
{
    m_player->setVolume(value);
    m_fullScreenContrlWid->getControlWidget()->updateVolumeSliderValue(m_player->volume());
    m_bottomWid->updateVolumeSliderValue(m_player->volume());

    if(m_middleWid->getContentWidget()->getCurrentSizeState() == FullScreenSize){
        m_fullScreenContrlWid->slot_showControlView();
    }
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
    setting.setValue("playmode",(int)m_middleWid->getListWidget()->getVideoList()->getPlayMode());
    setting.endGroup();

    m_player->pause();
}

void VideoWidgets::slot_exit()
{
    // Release media source and quit application.
    m_player->stop();
    m_player->setMedia(NULL);

    savaSetting();
    mainWindow->close();
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
    m_bottomWid->updateVolumeSliderValue(m_player->volume());
    m_fullScreenContrlWid->getControlWidget()->updateVolumeSliderValue(m_player->volume());
    saveVolume(m_player->volume());
}

VideoWidgets::~VideoWidgets()
{
}
