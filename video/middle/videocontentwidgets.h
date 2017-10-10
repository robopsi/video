#ifndef VIDEOMIDDLEWIDGETLEFT_H
#define VIDEOMIDDLEWIDGETLEFT_H

#include "basewidget.h"
#include "videocontentwidget.h"
#include "videoslider.h"
#include "videoquickcontentwidget.h"

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
    QMediaPlayer* getMediaPlayerFormQml(){return m_player;}
    VideoQuickContentWidget* getSurfaceWid(){return m_surfaceWid;}
private:
    QVBoxLayout *m_layout;
    // QMediaPlayer will deliver to 'videoWidets' for global control,
    // so it is no actual use in here.
    QMediaPlayer *m_player;
    VideoQuickContentWidget *m_surfaceWid;

    void initLayout();
    void initConnection();
signals:
    void surfaceOneClick();
    void surfaceDoubleClick();
};

#endif // VIDEOMIDDLEWIDGETLEFT_H
