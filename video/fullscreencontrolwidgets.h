#ifndef FULLSCREENCONTROLWIDGETS_H
#define FULLSCREENCONTROLWIDGETS_H

#include "basewidget.h"
#include "video/middle/videopositionwidget.h"
#include "bottom/videobottomwidgets.h"

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
private:
    // Hide control Widget if 3 seconds pass and no more action.
    QTimer *m_timer;

    QWidget *m_contentWid;
    VideoPositionWidget *m_positionWid;
    VideoBottomWidgets *m_controlWid;

    void initLayout();
    void initConnetion();

public slots:
    void slot_hideControlView();
    void slot_showControlView();
protected:
    void mousePressEvent(QMouseEvent *);
};

#endif // FULLSCREENCONTROLWIDGETS_H
