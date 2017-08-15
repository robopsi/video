#ifndef VIDEOBOTTOMWIDGETS_H
#define VIDEOBOTTOMWIDGETS_H

#include <QHBoxLayout>

#include "basewidget.h"
#include "basepushbutton.h"
#include "bottom/volwidget.h"
#include "fullscreencontrolwidgets.h"
#include "player/videolist.h"

class FullScreenControlWidgets;

class VideoBottomWidgets:public BaseWidget
{
public:
    VideoBottomWidgets(QWidget *parent=0,bool fullScreenStyle = false);
    ~VideoBottomWidgets();
public:
    FlatButton *m_btnOpenFile;
    FlatButton *m_btnPlayPause;
    FlatButton *m_btnNext;
    FlatButton *m_btnLast;
    VolWidget *m_VolWidget;
    FlatButton *m_btnChangeSize;
    FlatButton *m_btnRefresh;
    FlatButton *m_btnPlayMode;

    void setPlayingStyle();
    void setPauseStyle();
    void updatePlayModeIcon(VideoList::PlayMode playMode);
private:
    FullScreenControlWidgets *m_parent;
    bool isFullScreenStyle;

    void initLayout();
protected:
    void mousePressEvent(QMouseEvent *);
};

#endif // VIDEOBOTTOMWIDGETS_H
