#include "fullscreencontrolwidgets.h"
#include <QHBoxLayout>

#include <QMessageBox>

FullScreenControlWidgets::FullScreenControlWidgets(QWidget *parent):BaseWidget(parent)
  ,m_mediaOn(false)
{ 
    initLayout();
    initConnetion();

    slot_hideControlView();
}

void FullScreenControlWidgets::initLayout()
{
    QVBoxLayout *vmainLayout = new QVBoxLayout;

    m_topWid = new VideoTopWidgets(this);
    m_positionWid = new VideoPositionWidget(this,true);
    m_controlWid = new VideoBottomWidgets(this);

    m_contentWid = new QWidget(this);
    m_contentWid->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_listWid = new VideoListWidgets(this);
    m_listWid->setVisible(false);

    QHBoxLayout *middleLayout = new QHBoxLayout;
    middleLayout->addWidget(m_contentWid,2);
    middleLayout->addWidget(m_listWid,1);

    vmainLayout->addWidget(m_topWid);
    vmainLayout->addLayout(middleLayout);
    vmainLayout->addWidget(m_positionWid);
    vmainLayout->addWidget(m_controlWid);
    vmainLayout->setContentsMargins(0,0,0,0);
    vmainLayout->setSpacing(0);

    setLayout(vmainLayout);
}

void FullScreenControlWidgets::initConnetion()
{
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()),this, SLOT(slot_hideControlView()));
    // Initialize connection of slider moving.
    connect(m_positionWid,SIGNAL(sliderValueChange(int)),this,SIGNAL(sig_sliderPositionChanged(int)));
}

void FullScreenControlWidgets::slot_hideControlView()
{
    if(m_controlWid->isVisible()){
        m_topWid->setVisible(false);
        m_positionWid->setVisible(false);
        m_controlWid->setVisible(false);
        m_listWid->setVisible(false);
    }
}

void FullScreenControlWidgets::slot_showControlView(bool mediaOn)
{
    m_mediaOn = mediaOn;
    if(!m_controlWid->isVisible()){
        m_topWid->setVisible(true);
        m_controlWid->setVisible(true);
    }

    m_timer->stop();
    if(mediaOn){
        m_timer->start(5000);
        m_positionWid->setVisible(true);
    }
}

void FullScreenControlWidgets::removePositionWidget()
{
    m_positionWid->setVisible(false);
}

void FullScreenControlWidgets::hidePlayList()
{
    m_listWid->setVisible(false);
}

void FullScreenControlWidgets::showPlayList()
{
    m_listWid->setVisible(true);
}

void FullScreenControlWidgets::addPositionWidget()
{
    m_positionWid->setVisible(true);
}

void FullScreenControlWidgets::hideOrShowPlayList()
{
    if(m_listWid->isVisible()){
        m_listWid->setVisible(false);
    }else{
        m_listWid->setVisible(true);
    }
}

void FullScreenControlWidgets::stopHideTimer()
{
    m_mediaOn = false;
    m_timer->stop();
}

void FullScreenControlWidgets::mousePressEvent(QMouseEvent *e)
{
    if(m_positionWid->isVisible() && m_controlWid->isVisible()){
        slot_hideControlView();
    }else{
        slot_showControlView(m_mediaOn);
    }
    QWidget::mousePressEvent(e);
}

FullScreenControlWidgets::~FullScreenControlWidgets()
{
}
