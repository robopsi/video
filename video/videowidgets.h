#ifndef VIDEOWIDGETS_H
#define VIDEOWIDGETS_H

#include <QObject>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QThread>

#include "basepushbutton.h"
#include "basewidget.h"
#include "middle/videocontentwidgets.h"
#include "fullscreencontrolwidgets.h"

class MediaLoadThread;

/**
 * The main layout of video widgets.It is made up of 3 positional widgets.
 * This class in charge of the widgets's connecttion(in master control).
 *
 * The layout include top、middle(left、right)、bottom widgets and each has their own
 * layout control and  logic processing.
 */
class VideoWidgets:public BaseWidget
{
    Q_OBJECT
public:
    VideoWidgets(QWidget *parent = 0);
    ~VideoWidgets();

    void savaSetting();
    void saveVolume(int volume);
    void updateVolume(bool volumeAdd);
    void setPlayerPause(){m_player->pause();}
    void updateUiByRes(QFileInfoList);
    void showControlView();

public:
    VideoContentWidgets *m_contentWid;
private:
    // Save normal size of each moudle.
    int top_normal_height;
    int bottom_normal_height;

    QMediaPlayer *m_player;
    QUrl m_onPlayUrl;

    FullScreenControlWidgets *m_fullScreenContrlWid;

    MediaLoadThread *m_mediaLoadThread;

    void initLayout();
    void initPlayerAndConnection();
    // Read the settting of application saved last time.such as volume、playMode.
    void readSetting();
    void setOriginState();
public slots:
    void slot_onCurrentMediaChanged(QMediaContent);
    void slot_exit();
private slots:
    void slot_onMediaStateChanged(QMediaPlayer::MediaStatus);
    void slot_onPlayerStateChanged(QMediaPlayer::State);
    void slot_onMediaPositionChanged(qint64);
    void slot_onDurationChanged(qint64);
    void slot_onSliderPositionChanged(int);
    void slot_onMetaDataChanged(QString,QVariant);

    void slot_onLocalListItemClick(int,int);
    void slot_setPlayPause();
    void slot_nextVideo(bool isEndofMedia = false);
    void slot_lastVideo();
    void slot_addVideo();
    void slot_volumeChanged(int);
    void slot_refreshMediaResource();
    void slot_changePlayMode();
    void slot_fastForward();
    void slot_fastBackward();
    void slot_resizePlayList();

    void slot_onErrorOn(QMediaPlayer::Error);

    void sloat_tableLongPressed(int);
};

class MediaLoadThread: public QThread
{
public:
    MediaLoadThread(QObject *parent, QMediaPlayer *player, QUrl url);
    ~MediaLoadThread();

protected:
    void run();

private:
    QMediaPlayer *player;
    QUrl loadUrl;

};

#endif // VIDEOWIDGETS_H
