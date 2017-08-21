#include "videopositionwidget.h"
#include <QHBoxLayout>
#include <QTime>

#include "global_value.h"

#ifdef DEVICE_EVB
int video_position_height = 60;
#else
int video_position_height = 35;
#endif

VideoPositionWidget::VideoPositionWidget(QWidget *parent,bool fullScreenStyle):BaseWidget(parent)
{
    isFullScreenStyle = fullScreenStyle;
    if(isFullScreenStyle){
        m_parent = (FullScreenControlWidgets*)parent;
    }

    // Set background color.
    setFixedHeight(video_position_height);
    setStyleSheet("background:rgba(31,31,31)");

    initWidget();
}

void VideoPositionWidget::initWidget()
{
    QHBoxLayout *lyout = new QHBoxLayout;

    QFont font = this->font();
    font.setPixelSize(font_size_big);

    m_slider = new VideoSlider(Qt::Horizontal,this);
    m_slider->setRange(0,0);

    m_currentTime = new QLabel("00:00",this);
    m_currentTime->setStyleSheet("color:rgb(150,150,150);");
    m_currentTime->setFixedHeight(video_position_height);
    m_currentTime->setAlignment(Qt::AlignVCenter);
    m_currentTime->setFont(font);

    m_totalTime = new QLabel("00:00",this);
    m_totalTime->setStyleSheet("color:rgb(150,150,150);");
    m_totalTime->setFixedHeight(video_position_height);
    m_totalTime->setAlignment(Qt::AlignVCenter);
    m_currentTime->setFont(font);

    lyout->addSpacing(10);
    lyout->addWidget(m_currentTime);
    lyout->addWidget(m_slider);
    lyout->addWidget(m_totalTime);
    lyout->addSpacing(10);
    lyout->setSpacing(10);
    lyout->setContentsMargins(0,0,0,0);

    QVBoxLayout *mainlyout = new QVBoxLayout;

    mainlyout->addLayout(lyout);
    mainlyout->setContentsMargins(0,0,0,0);

    setLayout(mainlyout);
}

void VideoPositionWidget::onDurationChanged(qint64 duration)
{
    m_slider->setRange(0, duration);
    QTime totalTime((duration % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60),
                    (duration % (1000 * 60 * 60)) / (1000 * 60),
                    (duration % (1000 * 60)) / 1000);
    m_totalTime->setText(totalTime.toString("hh:mm:ss"));
}

void VideoPositionWidget::onMediaPositionChanged(qint64 position)
{
    m_slider->setValue(position);
    QTime currentTime((position % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60),
                      (position % (1000 * 60 * 60)) / (1000 * 60),
                      (position % (1000 * 60)) / 1000);
    m_currentTime->setText(currentTime.toString("hh:mm:ss"));
}

void VideoPositionWidget::mousePressEvent(QMouseEvent *event)
{
    if(m_slider->x() < event->x() && event->x() < (m_slider->x()+m_slider->width())){
        int dur = m_slider->maximum() - m_slider->minimum();
        int pos = m_slider->minimum() + dur * (((double)event->x()-m_slider->x())/ m_slider->width());
        if(pos != m_slider->sliderPosition()){
            m_slider->setValue(pos);
            emit m_slider->sig_sliderPositionChanged(pos);
        }
    }
    m_parent->slot_showControlView();
}

VideoPositionWidget::~VideoPositionWidget()
{
}
