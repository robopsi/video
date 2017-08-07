#include "videocontentwidgets.h"
#include <QHBoxLayout>
#include <QTime>
#include <QQuickItem>
#include "global_value.h"

VideoContentWidgets::VideoContentWidgets(QWidget *parent):BaseWidget(parent)
{
    // Set background color.
    setObjectName("VideoContentWidgets");
    setStyleSheet("#VideoContentWidgets{background:rgb(10,10,10)}");

    initLayout();
    // Init connection of slider moving.
    connect(m_positionWid->m_slider,SIGNAL(sig_sliderPositionChanged(int)),this,SIGNAL(sig_sliderPositionChanged(int)));
}

void VideoContentWidgets::initLayout()
{
    m_layout = new QVBoxLayout;

    m_surfaceWid = new VideoQuickContentWidget(this);
    m_positionWid = new VideoPositionWidget(this);
#ifndef DEVICE_EVB
    // Set video surface transparent for px3se
    m_surfaceWid->setClearColor(QColor(Qt::transparent));
#endif
    m_surfaceWid->setResizeMode(QQuickWidget::SizeRootObjectToView);
#ifdef DEVICE_EVB
    m_surfaceWid->setSource(QUrl("qrc:/video3399.qml"));
#else
    m_surfaceWid->setSource(QUrl("qrc:/video.qml"));
#endif
    QObject* qmlMediaPlayer = m_surfaceWid->rootObject()->findChild<QObject*>("mediaPlayer");
    m_player = qvariant_cast<QMediaPlayer *>(qmlMediaPlayer->property("mediaObject"));

    m_layout->addWidget(m_surfaceWid);
    m_layout->addWidget(m_positionWid);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);

    removePositionWidget();
    setLayout(m_layout);
}

void VideoContentWidgets::addPositionWidget()
{
    m_positionWid->show();
    m_layout->addWidget(m_positionWid);
    update();
}

void VideoContentWidgets::removePositionWidget()
{
    m_positionWid->hide();
    m_layout->removeWidget(m_positionWid);
    update();
}

void VideoContentWidgets::onDurationChanged(qint64 duration)
{
    m_positionWid->m_slider->setRange(0, duration);
    QTime totalTime((duration % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60),
                    (duration % (1000 * 60 * 60)) / (1000 * 60),
                    (duration % (1000 * 60)) / 1000);
    m_positionWid->m_totalTime->setText(totalTime.toString("hh:mm:ss"));
}

void VideoContentWidgets::onMediaPositionChanged(qint64 position)
{
    m_positionWid->m_slider->setValue(position);
    QTime currentTime((position % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60),
                      (position % (1000 * 60 * 60)) / (1000 * 60),
                      (position % (1000 * 60)) / 1000);
    m_positionWid->m_currentTime->setText(currentTime.toString("hh:mm:ss"));
}

VideoContentWidgets::~VideoContentWidgets()
{
    delete m_layout;
    delete m_surfaceWid;
    delete m_positionWid;
}
