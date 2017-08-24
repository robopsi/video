#include "baseslider.h"

BaseSlider::BaseSlider(Qt::Orientation orientation,QWidget *parent):QSlider(orientation,parent)
{  
    init();
}

void BaseSlider::init()
{
    setCursor(Qt::PointingHandCursor);
    setMouseTracking(true);

    setStyleSheet("QSlider::sub-page:horizontal{background:rgb(26,158,255);}"
                  "QSlider::add-page:horizontal{background:rgb(210,210,210);}"
                  "QSlider::handle:horizontal{background:rgb(255,255,255)}");

}


void BaseSlider::mousePressEvent(QMouseEvent *event)
{
    QSlider::mousePressEvent(event);
    // Position '-1' stands for update state.
    emit sig_sliderPositionChanged(-1);
}

void BaseSlider::mouseMoveEvent(QMouseEvent *event)
{
    QSlider::mouseMoveEvent(event);
    // Position '-1' stands for update state.
    emit sig_sliderPositionChanged(-1);
}

void BaseSlider::mouseReleaseEvent(QMouseEvent *event)
{
    QSlider::mouseReleaseEvent(event);
    int dur = maximum() - minimum();
    int pos = minimum() + dur * ((double)event->x() / width());
    if(pos != sliderPosition()){
        setValue(pos);
        emit sig_sliderPositionChanged(pos);
    }
}

