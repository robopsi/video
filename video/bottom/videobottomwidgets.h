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
    Q_OBJECT
public:
    VideoBottomWidgets(QWidget *parent=0,bool fullScreenStyle = false);
    ~VideoBottomWidgets();

    void setPlayingStyle();
    void setPauseStyle();
    void updatePlayModeIcon(PlayMode playMode);
    void updateVolumeSliderValue(int value);
private:
    FlatButton *m_btnOpenFile;
    FlatButton *m_btnPlayPause;
    FlatButton *m_btnNext;
    FlatButton *m_btnLast;
    VolWidget *m_VolWidget;
    FlatButton *m_btnChangeSize;
    FlatButton *m_btnRefresh;
    FlatButton *m_btnPlayMode;

    FullScreenControlWidgets *m_parent;
    bool isFullScreenStyle;

    void initLayout();
    void initConnection();
protected:
    void mousePressEvent(QMouseEvent *);
signals:
    void openFileClick();
    void volumeValueChanged(int);
    void nextClick();
    void lastClick();
    void nextLongPressed();
    void lastLongPressed();
    void playPauseClick();
    void playModeClick();
    void refreshClick();
    void changeSizeClick();
};

#endif // VIDEOBOTTOMWIDGETS_H
