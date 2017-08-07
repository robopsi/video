#include "videotopwidgets.h"
#include "global_value.h"

VideoTopWidgets::VideoTopWidgets(QWidget *parent):BaseWidget(parent)
{
    // Set background color.
    setObjectName("VideoTopWidgets");
    setStyleSheet("#VideoTopWidgets{background:rgb(56,58,66)}");

    initLayout();
}

void VideoTopWidgets::initLayout()
{
    QHBoxLayout *hmainyout=new QHBoxLayout;

    m_btnreturn=new FourStateButton(return_resource_str,this);
    m_btnreturn->setFixedSize(return_icon_width,return_icon_height);

    m_btnicon=new FlatButton(this);
    m_btnicon->setFixedSize(top_icon_size,top_icon_size);
    m_btnicon->setStyleSheet("QPushButton{background:transparent;border-image:url(:/image/video/video_icon.png)}");

    m_btnexit=new FlatButton(this);
    m_btnmini=new FlatButton(this);
    m_btnmobile=new FlatButton(this);
    m_btnsetting=new FlatButton(this);

    m_btnexit->setFixedSize(18,18);
    m_btnmini->setFixedSize(16,16);
    m_btnmobile->setFixedSize(16,16);
    m_btnsetting->setFixedSize(16,16);

    m_btnexit->setStyleSheet("QPushButton{border-image:url(:/image/main/btn_close (1).png);}"
                             "QPushButton::hover{border-image:url(:/image/main/btn_close (2).png);}"
                             "QPushButton::pressed{border-image:url(:/image/main/btn_close (3).png);}");
    m_btnmini->setStyleSheet("QPushButton{border-image:url(:/image/main/btn_mini (1).png);}"
                             "QPushButton::hover{border-image:url(:/image/main/btn_mini (2).png);}"
                             "QPushButton::pressed{border-image:url(:/image/main/btn_mini (3).png);}");
    m_btnmobile->setStyleSheet("QPushButton{border-image:url(:/image/main/btn_mobile (1).png);}"
                               "QPushButton::hover{border-image:url(:/image/main/btn_mobile (2).png);}"
                               "QPushButton::pressed{border-image:url(:/image/main/btn_mobile (3).png);}");
    m_btnsetting->setStyleSheet("QPushButton{border-image:url(:/image/main/btn_setting (1).png);}"
                                "QPushButton::hover{border-image:url(:/image/main/btn_setting (2).png);}"
                                "QPushButton::pressed{border-image:url(:/image/main/btn_setting (3).png);}");

    QLabel *label1=new QLabel("丨",this);
    label1->setFixedSize(6,16);
    label1->adjustSize();

    m_playingVideoName=new QLabel(str_videoName_default,this);
    m_playingVideoName->setAlignment(Qt::AlignCenter);
    QFont font = m_playingVideoName->font();
    font.setPixelSize(font_size_big);
    m_playingVideoName->setFont(font);
    m_playingVideoName->adjustSize();

    QHBoxLayout *lyout1 = new QHBoxLayout;
    lyout1->addWidget(m_btnreturn);
    lyout1->addSpacing(30);
    lyout1->addWidget(m_btnicon);

    QHBoxLayout *lyout2 = new QHBoxLayout;
    lyout2->addWidget(label1);
    lyout2->addWidget(m_btnmobile);
    lyout2->addWidget(m_btnsetting);
    lyout2->addWidget(m_btnmini);
    lyout2->addWidget(m_btnexit);
    lyout2->addSpacing(10);
    lyout2->setSpacing(12);

    hmainyout->addLayout(lyout1,1);
    hmainyout->addWidget(m_playingVideoName,1);
    hmainyout->addLayout(lyout2,1);
    hmainyout->setContentsMargins(0,0,0,0);
    setLayout(hmainyout);

    m_btnmobile->setVisible(false);
    m_btnsetting->setVisible(false);
    m_btnmini->setVisible(false);
    m_btnexit->setVisible(false);
}

VideoTopWidgets::~VideoTopWidgets()
{
    delete m_btnreturn;
    delete m_btnicon;
    delete m_btnexit;
    delete m_btnmobile;
    delete m_btnmini;
    delete m_btnsetting;
}
