#include "videomiddlewidgets.h"

VideoMiddleWidgets::VideoMiddleWidgets(QWidget *parent):BaseWidget(parent)
{
    initLayout();
}

void VideoMiddleWidgets::initLayout()
{
    QHBoxLayout *hmainlayout = new QHBoxLayout;

    m_listWid = new VideoListWidgets(this);
    m_contentWid = new VideoContentWidgets(this);

    hmainlayout->addWidget(m_contentWid,2);
    hmainlayout->addWidget(m_listWid,1);
    hmainlayout->setContentsMargins(0,0,0,0);
    hmainlayout->setSpacing(0);

    setLayout(hmainlayout);
}


VideoMiddleWidgets::~VideoMiddleWidgets()
{
}

