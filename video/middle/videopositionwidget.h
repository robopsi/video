#ifndef VIDEOPOSITIONWIDGET_H
#define VIDEOPOSITIONWIDGET_H

#include "basewidget.h"
#include "videoslider.h"
#include "fullscreencontrolwidgets.h"

#include <QLabel>

class FullScreenControlWidgets;

class VideoPositionWidget:public BaseWidget
{
public:
    VideoPositionWidget(QWidget *parent,bool fullScreenStyle = false);
    ~VideoPositionWidget();

    VideoSlider* getSlider(){return m_slider;}
private:
    FullScreenControlWidgets *m_parent;
    bool isFullScreenStyle;

    void initWidget();
public:
    VideoSlider *m_slider;
    QLabel *m_currentTime;
    QLabel *m_totalTime;

    void onMediaPositionChanged(qint64 position);
    void onDurationChanged(qint64 duration);
protected:
    void mousePressEvent(QMouseEvent*);
};

#endif // VIDEOPOSITIONWIDGET_H
