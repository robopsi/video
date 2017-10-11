#ifndef VOLWIDGET_H
#define VOLWIDGET_H

#include "base/baseslider.h"

#include <QWidget>
#include <QObject>
#include "basepushbutton.h"
#include "basewidget.h"

class VolWidget: public BaseWidget
{
    Q_OBJECT
public:
    VolWidget(QWidget *parent);
    ~VolWidget(){}

    void updateSlider(int value){m_volSlider->setValue(value);}
private:
    bool isMute;
    int valueBeforeMute;

    BaseSlider *m_volSlider;
    FlatButton *m_btnIcon;

    void init();
    void updateIconBySliderValue(int);
private slots:
    void slot_onIconClick();
    void slot_onSliderValueChanged(int);

signals:
    void sig_valueChanged(int);
};

#endif // VOLWIDGET_H
