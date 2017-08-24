#ifndef VIDEOWIDGETS_H
#define VIDEOWIDGETS_H

#include <QObject>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QStackedLayout>

#include "basepushbutton.h"
#include "basewidget.h"
#include "top/videotopwidgets.h"
#include "middle/videomiddlewidgets.h"
#include "bottom/videobottomwidgets.h"
#include "middle/videocontentwidget.h"
#include "fullscreencontrolwidgets.h"

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
    QFileInfoList findAllVideoFiles(const QString &serachPath);
    void updateUiByRes(QFileInfoList);
public:
    VideoMiddleWidgets *m_middleWid;
    VideoBottomWidgets *m_bottomWid;
    VideoTopWidgets *m_topWid;
private:
    // Save normal size of each moudle.
    int top_normal_height;
    int bottom_normal_height;
    int middle_list_width;

    QMediaPlayer *m_player;
    QUrl m_onPlayUrl;

    QStackedLayout *m_stackedLayout;
    FullScreenControlWidgets *m_fullScreenContrlWid;

    void initLayout();
    void initPlayerAndConnection();
    // Read the settting of application saved last time.such as volume、playMode.
    void readSetting();
    void setOriginState();
public slots:
     void slot_onCurrentMediaChanged(QMediaContent);
private slots:
    void slot_onMediaStateChanged(QMediaPlayer::MediaStatus);
    void slot_onPlayerStateChanged(QMediaPlayer::State);
    void slot_onMediaPositionChanged(qint64);
    void slot_onDurationChanged(qint64);
    void slot_onSliderPositionChanged(int);

    void slot_onLocalListItemClick(int,int);
    void slot_deleteTableItem(int);
    void slot_setPlayPause();
    void slot_nextVideo(bool isEndofMedia = false);
    void slot_lastVideo();
    void slot_addVideo();
    void slot_volumeChanged(int);
    void slot_exit();
    void slot_refreshMediaResource();
    void slot_fullScreenStyle();
    void slot_normalSizeStyle();
    void slot_changePlayMode();
    void slot_fastForward();
    void slot_fastBackward();

    void slot_onErrorOn(QMediaPlayer::Error);
};

#endif // VIDEOWIDGETS_H
