#ifndef VIDEOSLIDER_H
#define VIDEOSLIDER_H

#include <QSlider>

#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <QSlider>

class VideoSlider:public QSlider
{
    Q_OBJECT
public:
    VideoSlider(Qt::Orientation orientation,QWidget *parent);
protected:
    void mousePressEvent(QMouseEvent*);

signals:
    void sig_sliderPositionChanged(int);
};

#endif // VIDEOSLIDER_H
