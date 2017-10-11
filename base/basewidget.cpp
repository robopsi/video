#include "basewidget.h"
#include <QStyleOption>
#include <QPainter>

BaseWidget::BaseWidget(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);

    setStyleSheet("QPushButton:hover, QPushButton:focus{background: rgb(150,150,150);}"
                  "QPushButton::pressed{background-color:rgb(204,228,247);border: 1px solid rgb(1 , 84 , 153);padding-left:3px;padding-top:3px;}");
}

void BaseWidget::paintEvent(QPaintEvent *)
{
    /* Slove the problem which 'setStyleSheet' and 'Q_OBJECT' can co-exist
       The below code used to repaint widgets when change became. */
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void BaseWidget::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
}

void BaseWidget::mouseMoveEvent(QMouseEvent *e)
{
    QWidget::mouseMoveEvent(e);
}

void BaseWidget::mouseReleaseEvent(QMouseEvent *e)
{
    QWidget::mouseReleaseEvent(e);
}
