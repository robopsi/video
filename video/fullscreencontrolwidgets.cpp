#include "fullscreencontrolwidgets.h"
#include <QHBoxLayout>

#include <QMessageBox>

FullScreenControlWidgets::FullScreenControlWidgets(QWidget *parent):BaseWidget(parent)
{ 
    initLayout();
    initConnetion();

    slot_hideControlView();
}

void FullScreenControlWidgets::initLayout()
{
    QVBoxLayout *vmainLayout = new QVBoxLayout;

    m_positionWid = new VideoPositionWidget(this,true);
    m_controlWid = new VideoBottomWidgets(this,true);
    m_contentWid = new QWidget(this);

    vmainLayout->addWidget(m_contentWid);
    vmainLayout->addWidget(m_positionWid);
    vmainLayout->setSpacing(0);
    vmainLayout->addWidget(m_controlWid);
    vmainLayout->setContentsMargins(0,0,0,0);
    vmainLayout->setSpacing(0);

    setLayout(vmainLayout);
}

void FullScreenControlWidgets::initConnetion()
{
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()),this, SLOT(slot_hideControlView()));
}

void FullScreenControlWidgets::slot_hideControlView()
{
    if(m_positionWid->isVisible()){
        m_positionWid->setVisible(false);
        m_controlWid->setVisible(false);
    }
}

void FullScreenControlWidgets::slot_showControlView()
{
    if(!m_positionWid->isVisible()){
        m_positionWid->setVisible(true);
        m_controlWid->setVisible(true);
    }
    m_timer->stop();
    m_timer->start(5000);
}

void FullScreenControlWidgets::mousePressEvent(QMouseEvent *e)
{
    if(!m_positionWid->isVisible()){
        slot_showControlView();
    }else{
        slot_hideControlView();
    }
    QWidget::mousePressEvent(e);
}

FullScreenControlWidgets::~FullScreenControlWidgets()
{
}
