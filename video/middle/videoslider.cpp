#include "videoslider.h"

VideoSlider::VideoSlider(Qt::Orientation orientation,QWidget*parent):QSlider(orientation,parent)
{
#ifdef DEVICE_EVB
    setStyleSheet("QSlider::groove:horizontal{border-radius:2px;height:9px;}"
                  "QSlider::sub-page:horizontal{background:rgb(26,158,255);}"
                  "QSlider::add-page:horizontal{background:rgb(200,200,209);}"
                  "QSlider::handle:horizontal{background:rgb(255, 255, 160);width:8px;border-radius:4px;margin:-3px 0px -3px 0px;}");
#else
    setStyleSheet("QSlider::groove:horizontal{border-radius:1px;height:4px;}"
                  "QSlider::sub-page:horizontal{background:rgb(26,158,255);}"
                  "QSlider::add-page:horizontal{background:rgb(200,200,209);}"
                  "QSlider::handle:horizontal{background:rgb(255, 255, 160);width:8px;border-radius:4px;margin:-3px 0px -3px 0px;}");
#endif
    setCursor(Qt::PointingHandCursor);
    setMouseTracking(true);
}

void VideoSlider::mousePressEvent(QMouseEvent *event)
{
    QSlider::mousePressEvent(event);
    // Position '-1' stands for update state.
    emit sig_sliderPositionChanged(-1);
}

void VideoSlider::mouseMoveEvent(QMouseEvent *event)
{
    QSlider::mouseMoveEvent(event);
    // Position '-1' stands for update state.
    emit sig_sliderPositionChanged(-1);
}

void VideoSlider::mouseReleaseEvent(QMouseEvent *event)
{
    QSlider::mouseReleaseEvent(event);
    int dur = maximum() - minimum();
    int pos = minimum() + dur * ((double)event->x() / width());
    if(pos != sliderPosition()){
        setValue(pos);
        emit sig_sliderPositionChanged(pos);
    }
}
