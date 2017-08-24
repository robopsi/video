#include "videocontentwidgets.h"
#include <QHBoxLayout>
#include <QTime>
#include <QQuickItem>
#include "global_value.h"

VideoContentWidgets::VideoContentWidgets(QWidget *parent):BaseWidget(parent)
  ,m_currentSizeState(NormalSize)
{
    // Set background color.
    setObjectName("VideoContentWidgets");
    setStyleSheet("#VideoContentWidgets{background:rgb(10,10,10)}");

    initLayout();
    initConnection();
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

void VideoContentWidgets::initConnection()
{
    connect(m_surfaceWid,SIGNAL(contentOneClick()),this,SIGNAL(surfaceOneClick()));
    connect(m_surfaceWid,SIGNAL(contentDoubleClick()),this,SIGNAL(surfaceDoubleClick()));

    // Initialize connection of slider moving.
    connect(m_positionWid,SIGNAL(sliderValueChange(int)),this,SIGNAL(sig_sliderPositionChanged(int)));
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
    addPositionWidget();
    m_positionWid->onDurationChanged(duration);
    m_positionWid->setVisible(m_currentSizeState!=FullScreenSize);
}

void VideoContentWidgets::onMediaPositionChanged(qint64 position)
{
    m_positionWid->onMediaPositionChanged(position);
    m_positionWid->setVisible(m_currentSizeState!=FullScreenSize);
}

void VideoContentWidgets::fullScreenStyle()
{
    m_surfaceWid->setFullScreen(true);
    m_positionWid->setVisible(false);
    m_currentSizeState = FullScreenSize;
}

void VideoContentWidgets::normalSizeStyle()
{
    m_surfaceWid->setFullScreen(false);
    m_positionWid->setVisible(true);
    m_currentSizeState = NormalSize;
}

VideoContentWidgets::~VideoContentWidgets()
{
}
