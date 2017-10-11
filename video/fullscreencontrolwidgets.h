#ifndef FULLSCREENCONTROLWIDGETS_H
#define FULLSCREENCONTROLWIDGETS_H

#include "basewidget.h"
#include "video/middle/videopositionwidget.h"
#include "bottom/videobottomwidgets.h"
#include "top/videotopwidgets.h"
#include "middle/videolistwidgets.h"

class VideoBottomWidgets;
class VideoPositionWidget;

/**
 * Control widget when video surface in fullScreen.
 * It contains a sliderBar and control function of player.
 */
class FullScreenControlWidgets:public BaseWidget
{
    Q_OBJECT
public:
    FullScreenControlWidgets(QWidget *parent);
    ~FullScreenControlWidgets();

    VideoBottomWidgets* getControlWidget(){return m_controlWid;}
    VideoPositionWidget* getPositionWidget(){return m_positionWid;}
    VideoTopWidgets* getTopWidget(){return m_topWid;}
    VideoListWidgets* getListWidget(){return m_listWid;}

    void removePositionWidget();
    void hidePlayList();
    void showPlayList();
    void hideOrShowPlayList();
    void restartHideTimer();
private:
    // Hide control Widget if 3 seconds pass and no more action.
    QTimer *m_timer;
    bool m_mediaOn;

    QWidget *m_contentWid;
    VideoPositionWidget *m_positionWid;
    VideoBottomWidgets *m_controlWid;
    VideoTopWidgets *m_topWid;
    VideoListWidgets *m_listWid;

    void initLayout();
    void initConnetion();

public slots:
    void slot_hideControlView();
    void slot_showControlView(bool mediaOn = false);
protected:
    void mousePressEvent(QMouseEvent *);

signals:
    void sig_sliderPositionChanged(int);
};

#endif // FULLSCREENCONTROLWIDGETS_H
