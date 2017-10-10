#include "videobottomwidgets.h"

#ifdef DEVICE_EVB
int video_bottom_height  = 160;
int video_playButton_size = 100;
int refresh_playmode_size = 70;
int change_surface_size = 55;
int layout_temp = 25;
#else
int video_bottom_height  = 70;
int video_playButton_size = 50;
int refresh_playmode_size = 40;
int change_surface_size = 35;
int layout_temp = 10;
#endif

VideoBottomWidgets::VideoBottomWidgets(QWidget *parent):BaseWidget(parent)
{
    m_parent = (FullScreenControlWidgets*)parent;

    // Set background color and bottom widget height.
    setObjectName("VideoBottomWidgets");
    setStyleSheet("#VideoBottomWidgets{background:rgb(89,92,102)}");
    setFixedHeight(video_bottom_height);

    initLayout();
    initConnection();
}

void VideoBottomWidgets::initLayout()
{
    QHBoxLayout *hmainlyout = new QHBoxLayout;

    m_btnPlayPause = new FlatButton(this);
    m_btnNext = new FlatButton(this);
    m_btnLast = new FlatButton(this);
    m_btnOpenFile = new FlatButton(this);
    m_VolWidget = new VolWidget(this);
    m_btnChangeSize = new FlatButton(this);
    m_btnRefresh = new FlatButton(this);
    m_btnPlayMode = new FlatButton(this);
    m_btnPlayList = new FlatButton(this);

    m_btnOpenFile->setVisible(false);
    m_btnRefresh->setVisible(false);
    m_btnChangeSize->setVisible(false);

    m_btnPlayPause->setFixedSize(video_playButton_size,video_playButton_size);
    m_btnNext->setFixedSize(video_playButton_size,video_playButton_size);
    m_btnLast->setFixedSize(video_playButton_size,video_playButton_size);
    m_btnOpenFile->setFixedSize(video_playButton_size,video_playButton_size);
    m_btnChangeSize->setFixedSize(change_surface_size,change_surface_size);
    m_btnRefresh->setFixedSize(refresh_playmode_size,refresh_playmode_size);
    m_btnPlayMode->setFixedSize(refresh_playmode_size,refresh_playmode_size-layout_temp);
    m_btnPlayList->setFixedSize(refresh_playmode_size,refresh_playmode_size);

    m_btnNext->setStyleSheet("QPushButton{border-image:url(:/image/video/btn_next (1).png);}"
                             "QPushButton::hover{border-image:url(:/image/video/btn_next (2).png);}\
                             QPushButton:pressed\
                             {\
                                 background-color:rgb(204 , 228 , 247);\
                                 border: 1px solid rgb(1 , 84 , 153);\
                                 padding-left:3px;\
                                 padding-top:3px;\
                             }");
    m_btnLast->setStyleSheet("QPushButton{border-image:url(:/image/video/btn_previous (1).png);}"
                             "QPushButton::hover{border-image:url(:/image/video/btn_previous (2).png);}\
                             QPushButton:pressed\
                             {\
                                 background-color:rgb(204 , 228 , 247);\
                                 border: 1px solid rgb(1 , 84 , 153);\
                                 padding-left:3px;\
                                 padding-top:3px;\
                             }");
    m_btnPlayPause->setStyleSheet("QPushButton{border-image:url(:/image/video/btn_play (1).png);}"
                                  "QPushButton::hover{border-image:url(:/image/video/btn_play (2).png);}\
                                  QPushButton:pressed\
                                  {\
                                      background-color:rgb(204 , 228 , 247);\
                                      border: 1px solid rgb(1 , 84 , 153);\
                                      padding-left:3px;\
                                      padding-top:3px;\
                                  }");
    m_btnOpenFile->setStyleSheet("QPushButton{border-image:url(:/image/video/video_open_file.png);}\
                                 QPushButton:pressed\
                                 {\
                                     background-color:rgb(204 , 228 , 247);\
                                     border: 1px solid rgb(1 , 84 , 153);\
                                     padding-left:3px;\
                                     padding-top:3px;\
                                 }");
    m_btnChangeSize->setStyleSheet("QPushButton{border-image:url(:/image/video/btn_fullscreen.png);}\
                                   QPushButton:pressed\
                                   {\
                                       background-color:rgb(204 , 228 , 247);\
                                       border: 1px solid rgb(1 , 84 , 153);\
                                       padding-left:3px;\
                                       padding-top:3px;\
                                   }");
    m_btnRefresh->setStyleSheet("QPushButton{border-image:url(:/image/video/video_refresh.png);}\
                                QPushButton:pressed\
                                {\
                                    background-color:rgb(204 , 228 , 247);\
                                    border: 1px solid rgb(1 , 84 , 153);\
                                    padding-left:3px;\
                                    padding-top:3px;\
                                }");
    m_btnPlayMode->setStyleSheet("QPushButton{border-image:url(:/image/video/btn_list.png);}\
                                 QPushButton:pressed\
                                 {\
                                     background-color:rgb(204 , 228 , 247);\
                                     border: 1px solid rgb(1 , 84 , 153);\
                                     padding-left:3px;\
                                     padding-top:3px;\
                                 }");
    m_btnPlayList->setStyleSheet("QPushButton{border-image:url(:/image/video/btn_play_list.png);}\
                                 QPushButton:pressed\
                                 {\
                                     background-color:rgb(204 , 228 , 247);\
                                     border: 1px solid rgb(1 , 84 , 153);\
                                     padding-left:3px;\
                                     padding-top:3px;\
                                 }");


    QHBoxLayout *hlyout1 = new QHBoxLayout;
    hlyout1->addStretch(0);
    hlyout1->addWidget(m_VolWidget);
    hlyout1->addStretch(0);

    QHBoxLayout *hlyout2 = new QHBoxLayout;
    hlyout2->addWidget(m_btnLast);
    hlyout2->addSpacing(20);
    hlyout2->addWidget(m_btnPlayPause);
    hlyout2->addSpacing(20);
    hlyout2->addWidget(m_btnNext);

    QHBoxLayout *hlyout3 = new QHBoxLayout;
    hlyout3->addStretch(0);
    hlyout3->addWidget(m_btnPlayMode);
    hlyout3->addSpacing(30);
    hlyout3->addWidget(m_btnPlayList);
    hlyout3->addStretch(0);

    hmainlyout->addLayout(hlyout1,1);
    hmainlyout->addLayout(hlyout2,1);
    hmainlyout->addLayout(hlyout3,1);
    hmainlyout->setContentsMargins(0,0,0,0);
    hmainlyout->setSpacing(0);

    setLayout(hmainlyout);
}

void VideoBottomWidgets::initConnection()
{
    connect(m_btnOpenFile,SIGNAL(clicked(bool)),this,SIGNAL(openFileClick()));
    connect(m_VolWidget,SIGNAL(sig_valueChanged(int)),this,SIGNAL(volumeValueChanged(int)));
    connect(m_btnPlayPause,SIGNAL(clicked(bool)),this,SIGNAL(playPauseClick()));
    connect(m_btnNext,SIGNAL(clicked(bool)),this,SIGNAL(nextClick()));
    connect(m_btnNext,SIGNAL(longPressedEvent()),this,SIGNAL(nextLongPressed()));
    connect(m_btnLast,SIGNAL(clicked(bool)),this,SIGNAL(lastClick()));
    connect(m_btnLast,SIGNAL(longPressedEvent()),this,SIGNAL(lastLongPressed()));
    connect(m_btnPlayMode,SIGNAL(clicked(bool)),this,SIGNAL(playModeClick()));
    connect(m_btnChangeSize,SIGNAL(clicked(bool)),this,SIGNAL(changeSizeClick()));
    connect(m_btnRefresh,SIGNAL(clicked(bool)),this,SIGNAL(refreshClick()));
    connect(m_btnPlayList,SIGNAL(clicked(bool)),this,SIGNAL(playListClick()));
}

void VideoBottomWidgets::setPlayingStyle()
{
    m_btnPlayPause->setStyleSheet("QPushButton{border-image:url(:/image/video/btn_pause (1).png);}"
                                  "QPushButton::hover{border-image:url(:/image/video/btn_pause (2).png);}\
                                  QPushButton:pressed\
                                  {\
                                      background-color:rgb(204 , 228 , 247);\
                                      border: 1px solid rgb(1 , 84 , 153);\
                                      padding-left:3px;\
                                      padding-top:3px;\
                                  }");

}

void VideoBottomWidgets::setPauseStyle()
{
    m_btnPlayPause->setStyleSheet("QPushButton{border-image:url(:/image/video/btn_play (1).png);}"
                                  "QPushButton::hover{border-image:url(:/image/video/btn_play (2).png);}\
                                  QPushButton:pressed\
                                  {\
                                      background-color:rgb(204 , 228 , 247);\
                                      border: 1px solid rgb(1 , 84 , 153);\
                                      padding-left:3px;\
                                      padding-top:3px;\
                                  }");
}

void VideoBottomWidgets::updatePlayModeIcon(PlayMode playMode)
{
    switch(playMode){
    case PlayRandom:
        m_btnPlayMode->setStyleSheet("QPushButton{border-image:url(:/image/video/btn_random.png);}\
                                     QPushButton:pressed\
                                     {\
                                         background-color:rgb(204 , 228 , 247);\
                                         border: 1px solid rgb(1 , 84 , 153);\
                                         padding-left:3px;\
                                         padding-top:3px;\
                                     }");
        break;
    case PlayOneCircle:
        m_btnPlayMode->setStyleSheet("QPushButton{border-image:url(:/image/video/btn_single.png);}\
                                     QPushButton:pressed\
                                     {\
                                         background-color:rgb(204 , 228 , 247);\
                                         border: 1px solid rgb(1 , 84 , 153);\
                                         padding-left:3px;\
                                         padding-top:3px;\
                                     }");
        break;
    case PlayInOrder:
        m_btnPlayMode->setStyleSheet("QPushButton{border-image:url(:/image/video/btn_list.png);}\
                                     QPushButton:pressed\
                                     {\
                                         background-color:rgb(204 , 228 , 247);\
                                         border: 1px solid rgb(1 , 84 , 153);\
                                         padding-left:3px;\
                                         padding-top:3px;\
                                     }");
        break;
    }
}

void VideoBottomWidgets::mousePressEvent(QMouseEvent *)
{
    m_parent->slot_showControlView();
}

void VideoBottomWidgets::updateVolumeSliderValue(int value)
{
    m_VolWidget->updateSlider(value);
}

VideoBottomWidgets::~VideoBottomWidgets()
{
}
