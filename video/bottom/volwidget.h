#ifndef VOLWIDGET_H
#define VOLWIDGET_H

#include <QSlider>
#include "myslider.h"

class VolWidget :public QWidget
{
    Q_OBJECT
public:
    VolWidget(QWidget*parent);
    mySlider *m_slider_vol;
signals:
    void sig_valueChanged(int);
};

#endif // VOLWIDGET_H
