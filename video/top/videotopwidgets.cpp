#include "videotopwidgets.h"
#include "global_value.h"
#include "focusswitchmanager.h"

VideoTopWidgets::VideoTopWidgets(QWidget *parent):BaseWidget(parent)
{
    // Set background color.
    setObjectName("VideoTopWidgets");
    setStyleSheet("#VideoTopWidgets{background:rgb(56,58,66)}");

    initLayout();
    initConnection();
}

void VideoTopWidgets::initLayout()
{
    QHBoxLayout *hmainyout=new QHBoxLayout;

    m_btnreturn=new FourStateButton(return_resource_str,this);
    m_btnreturn->setFixedSize(return_icon_width,return_icon_height);

    FocusSwitchManager::getInstance()->insertIntoMap("1,1",m_btnreturn);

    QHBoxLayout *lyout1 = new QHBoxLayout;
    lyout1->addWidget(m_btnreturn);
    lyout1->addStretch(0);
    lyout1->setContentsMargins(0,0,0,0);

    m_playingVideoName = new QLabel(tr("videoPlayer"),this);
    QFont font = m_playingVideoName->font();
    font.setPixelSize(font_size_big);
    m_playingVideoName->setFont(font);
    m_playingVideoName->setAlignment(Qt::AlignCenter);

    hmainyout->addLayout(lyout1,1);
    hmainyout->addWidget(m_playingVideoName,1);
    hmainyout->addStretch(1);
    hmainyout->setContentsMargins(0,0,0,0);
    hmainyout->setSpacing(0);
    setLayout(hmainyout);
}

void VideoTopWidgets::initConnection()
{
    connect(m_btnreturn,SIGNAL(clicked(bool)),this,SIGNAL(returnClick()));
}

VideoTopWidgets::~VideoTopWidgets()
{
}
