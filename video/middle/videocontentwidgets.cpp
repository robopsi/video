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
    initConnection();
}

void VideoContentWidgets::initLayout()
{
    m_layout = new QVBoxLayout;

    m_surfaceWid = new VideoQuickContentWidget(this);
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
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);

    setLayout(m_layout);
}

void VideoContentWidgets::initConnection()
{
    connect(m_surfaceWid,SIGNAL(contentOneClick()),this,SIGNAL(surfaceOneClick()));
    connect(m_surfaceWid,SIGNAL(contentDoubleClick()),this,SIGNAL(surfaceDoubleClick()));
}

VideoContentWidgets::~VideoContentWidgets()
{
}
