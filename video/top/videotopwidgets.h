#ifndef VIDEOTOPWIDGETS_H
#define VIDEOTOPWIDGETS_H

#include <QLabel>
#include <QHBoxLayout>

#include "basepushbutton.h"
#include "basewidget.h"

class VideoTopWidgets:public BaseWidget
{
    Q_OBJECT
public:    
    VideoTopWidgets(QWidget *parent=0);
    ~VideoTopWidgets();

    void setPlayingVideoName(QString text){m_playingVideoName->setText(text);}
private:
    FourStateButton *m_btnreturn;
    void initLayout();
    void initConnection();

    QLabel *m_playingVideoName;
signals:
    void returnClick();
};
#endif // VIDEOTOPWIDGETS_H
