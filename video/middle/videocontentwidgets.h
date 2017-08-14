#ifndef VIDEOMIDDLEWIDGETLEFT_H
#define VIDEOMIDDLEWIDGETLEFT_H

#include "basewidget.h"
#include "videocontentwidget.h"
#include "videoslider.h"
#include "videoquickcontentwidget.h"
#include "videopositionwidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QQuickWidget>
#include <QMediaPlayer>

/**
 * Manager video content widgets.
 *
 * This widgets include position widgets、video surface widget
 * and it deliver a qml QMediaplayer ouside.
 */
class VideoContentWidgets:public BaseWidget
{ 
    Q_OBJECT
public:
    VideoContentWidgets(QWidget *parent);
    ~VideoContentWidgets();
public:
    enum CurrentSizeState
    {
        FullScreenSize,
        NormalSize
    };

    void addPositionWidget();
    void removePositionWidget();
    void onDurationChanged(qint64 duration);
    void onMediaPositionChanged(qint64);
    QMediaPlayer* getMediaPlayerFormQml(){return m_player;}
    VideoQuickContentWidget* getSurfaceWid(){return m_surfaceWid;}
    CurrentSizeState getCurrentSizeState(){return m_currentSizeState;}
    void fullScreenStyle();
    void normalSizeStyle();
private:
    QVBoxLayout *m_layout;
    // QMediaPlayer will deliver to 'videoWidets' for global control,
    // so it is no actual use in here.
    QMediaPlayer *m_player;

    VideoQuickContentWidget *m_surfaceWid;
    VideoPositionWidget *m_positionWid;

    CurrentSizeState m_currentSizeState;

    void initLayout();
signals:
    void sig_sliderPositionChanged(int);
};

#endif // VIDEOMIDDLEWIDGETLEFT_H
