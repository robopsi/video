#ifndef VIDEOPOSITIONWIDGET_H
#define VIDEOPOSITIONWIDGET_H

#include "basewidget.h"
#include "videoslider.h"

#include <QLabel>

class VideoPositionWidget:public BaseWidget
{
public:
    VideoPositionWidget(QWidget *parent);
    ~VideoPositionWidget();
private:
    void initWidget();
public:
    VideoSlider *m_slider;
    QLabel *m_currentTime;
    QLabel *m_totalTime;
protected:
    void mousePressEvent(QMouseEvent*);
};

#endif // VIDEOPOSITIONWIDGET_H
