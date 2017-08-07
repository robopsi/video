#ifndef VIDEOBOTTOMWIDGETS_H
#define VIDEOBOTTOMWIDGETS_H

#include <QHBoxLayout>

#include "basewidget.h"
#include "basepushbutton.h"
#include "bottom/volwidget.h"

class VideoBottomWidgets:public BaseWidget
{
public:
    VideoBottomWidgets(QWidget *parent=0);
    ~VideoBottomWidgets();

    void setPlayingStyle();
    void setPauseStyle();

    FlatButton *m_btnOpenFile;
    FlatButton *m_btnPlayPause;
    FlatButton *m_btnNext;
    FlatButton *m_btnLast;
    VolWidget *m_VolWidget;
private:
    void initLayout();
};

#endif // VIDEOBOTTOMWIDGETS_H
