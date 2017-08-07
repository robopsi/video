#include "videocontentwidget.h"
#include <QMouseEvent>
#include <QMimeData>
#include <QBitmap>

VideoContentWidget::VideoContentWidget(QWidget *parent):QVideoWidget(parent)
{
    init();

    // Initial timer in order to distinguish click and double click.
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onOneClick()));
}

void VideoContentWidget::init()
{
    this->setMouseTracking(true);
    this->setCursor(QCursor(Qt::ArrowCursor));
    this->setAutoFillBackground(true);
    setWindowOpacity(1);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowSystemMenuHint);
    this->setFocusPolicy(Qt::ClickFocus);
    this->setAcceptDrops(true);
}

void VideoContentWidget::mousePressEvent(QMouseEvent *){
    m_timer->start(300);
}

void VideoContentWidget::mouseDoubleClickEvent(QMouseEvent *){
    m_timer->stop();
    emit contentDoubleClick();
}

void VideoContentWidget::onOneClick(){
    m_timer->stop();
    emit contentOneClick();
}

