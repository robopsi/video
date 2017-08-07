#ifndef VIDEOTOPWIDGETS_H
#define VIDEOTOPWIDGETS_H

#include <QLabel>
#include <QHBoxLayout>

#include "basepushbutton.h"
#include "basewidget.h"

class VideoTopWidgets:public BaseWidget
{
public:    
    VideoTopWidgets(QWidget *parent=0);
    ~VideoTopWidgets();

    void setPlayingVideoName(QString text){m_playingVideoName->setText(text);}
public:
    FourStateButton *m_btnreturn;
    FlatButton *m_btnicon;
    FlatButton *m_btnmobile;
    FlatButton *m_btnsetting;
    FlatButton *m_btnmini;
    FlatButton *m_btnexit;

private:
    void initLayout();

    QLabel *m_playingVideoName;
};
#endif // VIDEOTOPWIDGETS_H
