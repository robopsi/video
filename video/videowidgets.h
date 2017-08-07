#ifndef VIDEOWIDGETS_H
#define VIDEOWIDGETS_H

#include <QObject>
#include <QMediaPlayer>
#include <QVideoWidget>

#include "basepushbutton.h"
#include "basewidget.h"
#include "top/videotopwidgets.h"
#include "middle/videomiddlewidgets.h"
#include "bottom/videobottomwidgets.h"
#include "middle/videocontentwidget.h"

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
    void updateVolume(bool volumeAdd);
    void setPlayerPause(){m_player->pause();}
    QMediaPlayer* getPlayer(){return m_player;}
public:
    VideoMiddleWidgets *m_middleWid;
    VideoBottomWidgets *m_bottomWid;
    VideoTopWidgets *m_topWid;
private:
    QMediaPlayer *m_player;
    QUrl m_onPlayUrl;

    void initLayout();
    void initPlayerAndConnection();

    /**
     * Read the settting of application saved last time.such as volume.
     */
    void readSetting();

    /**
     * return the state of initial.
     */
    void setOriginState();
public slots:
     void slot_onCurrentMediaChanged(QMediaContent);
private slots:
    void slot_onMediaStateChanged(QMediaPlayer::MediaStatus);
    void slot_onPlayerStateChanged(QMediaPlayer::State);
    void slot_onMediaPositionChanged(qint64);
    void slot_onDurationChanged(qint64);
    void slot_onSliderPositionChanged(int);

    void slot_onLocalListItemDoubleClick(int,int);
    void slot_setPlayPause();
    void slot_nextVideo();
    void slot_lastVideo();
    void slot_addVideo();
    void slot_volumeChanged(int);
    void slot_returnClick();

    void slot_onContentDoubleClick();

    void slot_denyPlay();
    void slot_onErrorOn(QMediaPlayer::Error);
};

#endif // VIDEOWIDGETS_H
