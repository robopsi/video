#include "videoquickcontentwidget.h"

#include <QMouseEvent>
#include <QMimeData>
#include <QBitmap>
#include <QDesktopWidget>
#include <QApplication>

VideoQuickContentWidget::VideoQuickContentWidget(QWidget *parent):QQuickWidget(parent)
{
    init();

    // Initial timer in order to distinguish click and double click.
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onOneClick()));
}

void VideoQuickContentWidget::init()
{
    this->setMouseTracking(true);
    this->setCursor(QCursor(Qt::ArrowCursor));
    this->setAutoFillBackground(true);
    setWindowOpacity(1);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowSystemMenuHint);
    this->setFocusPolicy(Qt::ClickFocus);
    this->setAcceptDrops(true);
}

void VideoQuickContentWidget::mousePressEvent(QMouseEvent *){
    m_timer->start(300);
}

void VideoQuickContentWidget::mouseDoubleClickEvent(QMouseEvent *){
    m_timer->stop();
    emit contentDoubleClick();
}

void VideoQuickContentWidget::onOneClick(){
    m_timer->stop();
    emit contentOneClick();
}

void VideoQuickContentWidget::setFullScreen(bool fullScreen)
{
    if(fullScreen){
        normalWidth = this->width();
        normalHeight=this->height();
        this->setFixedSize(QApplication::desktop()->screenGeometry().width(),QApplication::desktop()->screenGeometry().height());
    }else{
        this->setFixedSize(normalWidth,normalHeight);
    }
}


