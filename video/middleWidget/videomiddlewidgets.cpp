#include "videomiddlewidgets.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

videoMiddleWidgets::videoMiddleWidgets(QWidget *parent):baseWidget(parent)
{
#ifndef DEVICE_EVB
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
#endif

    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    initLayout();
}

void videoMiddleWidgets::initLayout()
{
    QHBoxLayout *hmainlyout = new QHBoxLayout;

    m_rightWid = new videoMiddleWidgetRight(this);
    m_leftWid = new videoMiddleWidgetLeft(this);

    hmainlyout->addWidget(m_leftWid,9);
    hmainlyout->addWidget(m_rightWid,4);
    hmainlyout->setContentsMargins(0,0,0,0);
    hmainlyout->setSpacing(0);

    setLayout(hmainlyout);
}

