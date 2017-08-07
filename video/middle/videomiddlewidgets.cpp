#include "videomiddlewidgets.h"

VideoMiddleWidgets::VideoMiddleWidgets(QWidget *parent):BaseWidget(parent)
  ,isOnPlayMode(false)
{
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    initLayout();
}

void VideoMiddleWidgets::initLayout()
{
    QHBoxLayout *mainlayout = new QHBoxLayout;

    m_stackWid = new QStackedWidget();

    m_listWid = new VideoListWidgets(this);
    m_contentWid = new VideoContentWidgets(this);

    m_stackWid->addWidget(m_contentWid);
    m_stackWid->addWidget(m_listWid);
    m_stackWid->setCurrentWidget(m_listWid);

    mainlayout->addWidget(m_stackWid);
    mainlayout->setContentsMargins(0,0,0,0);
    mainlayout->setSpacing(0);
    setLayout(mainlayout);
}

void VideoMiddleWidgets::slot_playMode(){
    m_stackWid->setCurrentWidget(m_contentWid);
    isOnPlayMode = true;
}

void VideoMiddleWidgets::slot_listMode(){
    m_stackWid->setCurrentWidget(m_listWid);
    isOnPlayMode = false;
}

VideoMiddleWidgets::~VideoMiddleWidgets()
{
    delete m_stackWid;
    delete m_listWid;
    delete m_contentWid;
}

