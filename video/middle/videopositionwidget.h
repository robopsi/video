#ifndef VIDEOPOSITIONWIDGET_H
#define VIDEOPOSITIONWIDGET_H

#include "basewidget.h"
#include "videoslider.h"
#include "fullscreencontrolwidgets.h"

#include <QLabel>

class FullScreenControlWidgets;

class VideoPositionWidget:public BaseWidget
{
    Q_OBJECT
public:
    VideoPositionWidget(QWidget *parent,bool fullScreenStyle = false);
    ~VideoPositionWidget();

    VideoSlider* getSlider(){return m_slider;}
    void onMediaPositionChanged(qint64 position);
    void onDurationChanged(qint64 duration);
private:
    VideoSlider *m_slider;
    QLabel *m_currentTime;
    QLabel *m_totalTime;
    FullScreenControlWidgets *m_parent;
    bool isFullScreenStyle;

    void initWidget();
signals:
    void sliderValueChange(int);
};

#endif // VIDEOPOSITIONWIDGET_H
